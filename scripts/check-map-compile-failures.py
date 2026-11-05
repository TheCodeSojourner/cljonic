#!/usr/bin/env python3
"""Verify that `map` rejects out-of-domain calls at compile time and admits the
supported domain, for both modular and single-header builds.

Covers the MapFunction spec invariants (REQ-FN-032):
- SourcesFromCljonicDomainOnly
- StandardRangeContainerAndNonCljonicSourcesRejectedAtCompileTime
- CallbackCopyConstructible
- CallbackInvokedNothrowWithOneArgumentPerSource
- CallbackResultNothrowStorable
- ThrowingCallbackOrUnstorableResultRejectedAtCompileTime
"""

from __future__ import annotations

import os
import re
import subprocess

FAILURE_CASES = {
    # Sources outside the cljonic domain.
    "Map-std-vector-source": ("(void)cljonic::core::map(cljonic_map_identity, std::vector<int>{1});"),
    "Map-std-string-source": ("(void)cljonic::core::map(cljonic_map_identity, std::string{\"a\"});"),
    "Map-std-span-source": ("(void)cljonic::core::map(cljonic_map_identity, std::span<int>{});"),
    "Map-std-string-view-source": ("(void)cljonic::core::map(cljonic_map_identity, std::string_view{\"a\"});"),
    "Map-std-map-source": ("(void)cljonic::core::map(cljonic_map_identity, std::map<int, int>{});"),
    "Map-scalar-source": "(void)cljonic::core::map(cljonic_map_identity, 1);",
    "Map-array-source": "int cljonic_map_array[3]{}; (void)cljonic::core::map(cljonic_map_identity, cljonic_map_array);",
    # The zero-source arity is outside the admitted domain.
    "Map-zero-sources": "(void)cljonic::core::map(cljonic_map_identity);",
    # Callback contracts.
    "Map-throwing-callback": ("(void)cljonic::core::map(cljonic_map_throwing, cljonic::Vector<int, 2>{});"),
    "Map-non-storable-result": ("(void)cljonic::core::map(cljonic_map_string_result, cljonic::Vector<int, 2>{});"),
    "Map-non-copyable-callback": ("(void)cljonic::core::map(cljonic_map_deleted_copy, cljonic::Vector<int, 2>{});"),
    # Mixed domain: a cljonic source followed by a standard-library source.
    "Map-mixed-source-domain": (
        "(void)cljonic::core::map(cljonic_map_add, cljonic::Vector<int, 2>{}, std::vector<int>{1});"),
    # Safe parameter conversion: narrowing and domain-crossing conversions.
    "Map-narrowing-long-to-int": (
        "(void)cljonic::core::map(cljonic_map_takes_int, cljonic::Vector<long, 2>{});"),
    "Map-narrowing-double-to-float": (
        "(void)cljonic::core::map(cljonic_map_takes_float, cljonic::Vector<double, 2>{});"),
    "Map-narrowing-second-position": (
        "(void)cljonic::core::map(cljonic_map_takes_int, cljonic::Vector<int, 2>{}, cljonic::Vector<long, 2>{});"),
    "Map-domain-cross-int-to-char": (
        "(void)cljonic::core::map(cljonic_map_takes_char, cljonic::Vector<int, 2>{});"),
    "Map-domain-cross-char-to-int": (
        "(void)cljonic::core::map(cljonic_map_takes_int, cljonic::String<4>{});"),
    "Map-too-few-parameters": (
        "(void)cljonic::core::map(cljonic_map_takes_int, cljonic::Vector<int, 2>{}, cljonic::Vector<int, 2>{});"),
}

PASS_CASES = {
    "Map-unary-vector": "(void)cljonic::core::map(cljonic_map_identity, cljonic::Vector<int, 3>{});",
    "Map-unary-scalar-callback": (
        "constexpr auto cljonic_map_inc = [](int value) noexcept { return value + 1; };"
        "(void)cljonic::core::map(cljonic_map_inc, cljonic::core::vector(1, 2, 3));"),
    "Map-unary-range": ("(void)cljonic::core::map(cljonic_map_identity, cljonic::Range<int>{1, 4, 1});"),
    "Map-unary-repeat": "(void)cljonic::core::map(cljonic_map_identity, cljonic::Repeat<int>{3U, 1});",
    "Map-unary-cycle": (
        "(void)cljonic::core::map(cljonic_map_identity, cljonic::core::cycle(cljonic::Vector<int, 2>{}));"),
    "Map-unary-iterate": (
        "(void)cljonic::core::map(cljonic_map_identity, cljonic::core::iterate(cljonic_map_identity, 1));"),
    "Map-unary-repeatedly": (
        "(void)cljonic::core::map(cljonic_map_identity, cljonic::core::repeatedly(2U, cljonic_map_zero));"),
    "Map-unary-map-collection": (
        "(void)cljonic::core::map(cljonic_map_entry_value, "
        "cljonic::Map<int, int, 2>{cljonic::MapEntry<int, int>{1, 10}});"),
    "Map-unary-set-collection": "(void)cljonic::core::map(cljonic_map_identity, cljonic::Set<int, 2>{});",
    "Map-unary-queue-collection": "(void)cljonic::core::map(cljonic_map_identity, cljonic::Queue<int, 2>{});",
    "Map-unary-string-collection": "(void)cljonic::core::map(cljonic_map_identity_char, cljonic::String<4>{});",
    "Map-binary-two-sources": (
        "(void)cljonic::core::map(cljonic_map_add, cljonic::Vector<int, 2>{}, cljonic::Queue<int, 2>{});"),
    "Map-ternary-three-sources": (
        "(void)cljonic::core::map(cljonic_map_add3, cljonic::Vector<int, 2>{}, cljonic::Range<int>{1, 3, 1}, "
        "cljonic::Repeat<int>{2U, 7});"),
    "Map-converting-result": (
        "constexpr auto cljonic_map_to_long = [](int value) noexcept { return static_cast<long>(value); };"
        "(void)cljonic::core::map(cljonic_map_to_long, cljonic::Vector<int, 2>{});"),
    "Map-named-function-object": (
        "(void)cljonic::core::map(cljonic_map_adder{}, cljonic::Vector<int, 2>{}, cljonic::Queue<int, 2>{});"),
    # Value-preserving widening conversions are admitted.
    "Map-widening-int-to-long": (
        "(void)cljonic::core::map(cljonic_map_takes_long, cljonic::Vector<int, 2>{});"),
    "Map-widening-int-to-double": (
        "(void)cljonic::core::map(cljonic_map_takes_double, cljonic::Vector<int, 2>{});"),
    "Map-exact-char-parameter": "(void)cljonic::core::map(cljonic_map_identity_char, cljonic::String<4>{});",
    # Uninspectable callables fall back to nothrow invocability.
    "Map-generic-lambda-fallback": (
        "(void)cljonic::core::map([](auto value) noexcept { return value; }, cljonic::Vector<long, 2>{});"),
}

# Out-of-domain calls must report the targeted RejectionDiagnostic message
# (REQ-DIAG-009) rather than a raw list of rejected concept candidates. The
# message content is asserted, not just its stability: REQ-DIAG-009 requires the
# fallback to name the operation and the violated domain rule.
DIAGNOSTIC_OPERATION = "cljonic::map:"
DIAGNOSTIC_ANCHOR = "expected a transforming function followed by one or more compatible cljonic sources"
DIAGNOSTIC_SOURCE_RULE = ("Each source must be a Vector, Map, Set, Queue, or String collection, or a Range, "
                          "Repeat, Cycle, Iterate, or Repeatedly producer")
DIAGNOSTIC_CALLBACK_RULE = ("The function must be copy-constructible, invoked without throwing with one argument "
                            "per source, and return a storable element result")
DIAGNOSTIC_CONVERSION_RULE = ("Each function parameter must accept the corresponding source element through a "
                             "safe implicit conversion")

# name -> (probe body, required substrings beyond the shared anchors)
DIAGNOSTIC_CASES = {
    "Map-std-vector-diagnostic": (
        "(void)cljonic::core::map(cljonic_map_identity, std::vector<int>{1});",
        (DIAGNOSTIC_SOURCE_RULE,)),
    "Map-throwing-callback-diagnostic": (
        "(void)cljonic::core::map(cljonic_map_throwing, cljonic::Vector<int, 2>{});",
        (DIAGNOSTIC_CALLBACK_RULE,)),
    "Map-zero-sources-diagnostic": (
        "(void)cljonic::core::map(cljonic_map_identity);",
        ("At least one source is required",)),
}

# Injected before main(): the named callbacks and function objects used by the
# cases above. A throwing callback is declared but only ever passed to map, so
# no translation unit actually throws.
PREAMBLE = """
constexpr auto cljonic_map_identity = [](int value) noexcept { return value; };
constexpr auto cljonic_map_zero = []() noexcept { return 0; };
constexpr auto cljonic_map_add = [](int lhs, int rhs) noexcept { return lhs + rhs; };
constexpr auto cljonic_map_add3 = [](int lhs, int mid, int rhs) noexcept { return lhs + mid + rhs; };
constexpr auto cljonic_map_entry_value = [](const auto& entry) noexcept { return entry.value; };
constexpr auto cljonic_map_takes_int = [](int value) noexcept { return value; };
constexpr auto cljonic_map_takes_long = [](long value) noexcept { return value; };
constexpr auto cljonic_map_takes_double = [](double value) noexcept { return value; };
constexpr auto cljonic_map_takes_float = [](float value) noexcept { return value; };
constexpr auto cljonic_map_takes_char = [](char value) noexcept { return value; };
constexpr auto cljonic_map_identity_char = [](char value) noexcept { return value; };
auto cljonic_map_throwing = [](int value) { return value; };
auto cljonic_map_string_result = [](int value) noexcept { return std::string{std::to_string(value)}; };
struct cljonic_map_deleted_copy {
    [[nodiscard]] constexpr auto operator()(int value) const noexcept -> int {
        return value;
    }

    cljonic_map_deleted_copy() = default;
    cljonic_map_deleted_copy(const cljonic_map_deleted_copy&) = delete;
};
struct cljonic_map_adder {
    [[nodiscard]] constexpr auto operator()(int lhs, int rhs) const noexcept -> int {
        return lhs + rhs;
    }
};
"""


def build_source(header: str, preamble: str, body: str) -> str:
    return "\n".join(
        (
            "#include <map>",
            "#include <span>",
            "#include <string>",
            "#include <string_view>",
            "#include <vector>",
            f'#include "{header}"',
            preamble,
            "int main() {",
            f"  {body}",
            "  return 0;",
            "}",
        )
    )


def compile_case(compiler: list[str], include_dir: str, header: str, body: str, preamble: str = "") -> bool:
    source = build_source(header, preamble, body)
    result = subprocess.run(
        [*compiler, "-std=c++23", "-fsyntax-only", "-I", include_dir, "-x", "c++", "-"],
        input=source,
        text=True,
        capture_output=True,
        check=False,
    )
    return result.returncode == 0


def _normalized(text: str) -> str:
    return re.sub(r"\s+", " ", text)


def diagnostic_message_reported(
    compiler: list[str], include_dir: str, header: str, body: str, required: tuple[str, ...]
) -> bool:
    source = build_source(header, PREAMBLE, body)
    result = subprocess.run(
        [*compiler, "-std=c++23", "-fsyntax-only", "-I", include_dir, "-x", "c++", "-"],
        input=source,
        text=True,
        capture_output=True,
        check=False,
    )
    if result.returncode == 0:
        return False
    output = _normalized(result.stdout + result.stderr)
    needles = (
        DIAGNOSTIC_OPERATION,
        DIAGNOSTIC_ANCHOR,
        DIAGNOSTIC_SOURCE_RULE,
        DIAGNOSTIC_CALLBACK_RULE,
        DIAGNOSTIC_CONVERSION_RULE,
        *required,
    )
    return all(_normalized(needle) in output for needle in needles)


def main() -> int:
    compiler = os.environ.get("CXX", "c++").split()
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    configurations = (("modular", os.path.join(root, "src"), "cljonic-core.hpp"), ("single-header", root, "cljonic.hpp"))
    failures: list[str] = []

    for configuration, include_dir, header in configurations:
        for name, body in FAILURE_CASES.items():
            if compile_case(compiler, include_dir, header, body, preamble=PREAMBLE):
                failures.append(f"{configuration}/{name}: expected compile failure, but compiled")

        for name, body in PASS_CASES.items():
            if not compile_case(compiler, include_dir, header, body, preamble=PREAMBLE):
                failures.append(f"{configuration}/{name}: expected to compile, but failed")

        for name, (body, required) in DIAGNOSTIC_CASES.items():
            if not diagnostic_message_reported(compiler, include_dir, header, body, required):
                failures.append(f"{configuration}/{name}: targeted rejection diagnostic content not reported")

    if failures:
        print("map-compile-fail:failures:")
        print("\n".join(f"  {failure}" for failure in failures))
        return 1

    print("map-compile-fail:ok")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
