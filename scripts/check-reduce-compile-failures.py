#!/usr/bin/env python3
"""Verify that `reduce` rejects out-of-domain calls at compile time and admits
the supported domain, for both modular and single-header builds.

Covers the ReduceFunction spec invariants (REQ-FN-034):
- SourceIsCljonicDomainOnly
- NoInitialFormRequiresInspectableSignature
- ExplicitInitialFormAdmitsUninspectableFunction
- ReducingFunctionCopyConstructible
- ReducingFunctionInvokedNothrowAsAccumulatorAndElement
- ReducingFunctionResultTypeIsAccumulator
- AccumulatorSatisfiesNothrowElementContract
- InitialValueConvertsSafelyToAccumulator
- NarrowingAndDomainCrossingConversionsRejectedAtCompileTime
- StandardRangeContainerAndNonCljonicSourcesRejectedAtCompileTime
- ThrowingFunctionOrUnstorableAccumulatorRejectedAtCompileTime
"""

from __future__ import annotations

import os
import re
import subprocess

FAILURE_CASES = {
    # Sources outside the cljonic domain.
    "Reduce-std-vector-source": "(void)cljonic::reduce(cljonic_reduce_add, std::vector<int>{1});",
    "Reduce-std-array-source": "(void)cljonic::reduce(cljonic_reduce_add, std::array<int, 2>{1, 2});",
    "Reduce-std-span-source": "(void)cljonic::reduce(cljonic_reduce_add, std::span<int>{});",
    "Reduce-std-string-view-source": "(void)cljonic::reduce(cljonic_reduce_add, std::string_view{\"a\"});",
    "Reduce-std-map-source": "(void)cljonic::reduce(cljonic_reduce_add, std::map<int, int>{});",
    "Reduce-scalar-source": "(void)cljonic::reduce(cljonic_reduce_add, 1);",
    # The explicit-initial form rejects the same out-of-domain sources.
    "Reduce-std-vector-source-with-initial": "(void)cljonic::reduce(cljonic_reduce_add, 0, std::vector<int>{1});",
    # The form without an initial value requires an inspectable signature.
    "Reduce-uninspectable-without-initial": (
        "(void)cljonic::reduce([](auto lhs, auto rhs) noexcept { return lhs + rhs; }, cljonic::Vector<int, 2>{});"),
    # Reducing-function contracts.
    "Reduce-throwing-function": ("(void)cljonic::reduce(cljonic_reduce_throwing, cljonic::Vector<int, 2>{});"),
    "Reduce-non-copyable-function": (
        "(void)cljonic::reduce(cljonic_reduce_deleted_copy{}, cljonic::Vector<int, 2>{});"),
    "Reduce-result-is-not-accumulator": (
        "(void)cljonic::reduce(cljonic_reduce_returns_long, cljonic::Vector<int, 2>{});"),
    "Reduce-reference-result": (
        "(void)cljonic::reduce(&cljonic_reduce_reference_result, cljonic::Vector<int, 2>{});"),
    "Reduce-unstorable-accumulator": (
        "(void)cljonic::reduce(cljonic_reduce_text_accumulator, cljonic::Vector<int, 2>{});"),
    # Safe parameter conversion: narrowing and domain-crossing conversions.
    "Reduce-narrowing-long-to-int": ("(void)cljonic::reduce(cljonic_reduce_takes_int, cljonic::Vector<long, 2>{});"),
    # The no-initial seed conversion is rejected even though the callback's
    # element parameter safely accepts the long source element.
    "Reduce-no-initial-seed-narrowing": (
        "(void)cljonic::reduce(cljonic_reduce_int_acc_long_element, cljonic::Vector<long, 2>{});"),
    "Reduce-narrowing-double-to-float": (
        "(void)cljonic::reduce(cljonic_reduce_takes_float, cljonic::Vector<double, 2>{});"),
    "Reduce-domain-cross-int-to-char": (
        "(void)cljonic::reduce(cljonic_reduce_takes_char, cljonic::Vector<int, 2>{});"),
    "Reduce-domain-cross-char-to-int": (
        "(void)cljonic::reduce(cljonic_reduce_takes_int, cljonic::String<4>{});"),
    # The initial value must convert safely to the accumulator type.
    "Reduce-initial-narrowing": (
        "(void)cljonic::reduce(cljonic_reduce_takes_int, 1L, cljonic::Vector<int, 2>{});"),
    "Reduce-initial-domain-cross": (
        "(void)cljonic::reduce(cljonic_reduce_takes_int, true, cljonic::Vector<int, 2>{});"),
}

PASS_CASES = {
    "Reduce-no-initial-vector": "(void)cljonic::reduce(cljonic_reduce_add, cljonic::Vector<int, 3>{});",
    "Reduce-no-initial-queue": "(void)cljonic::reduce(cljonic_reduce_add, cljonic::Queue<int, 3>{});",
    "Reduce-no-initial-set": "(void)cljonic::reduce(cljonic_reduce_add, cljonic::Set<int, 3>{});",
    "Reduce-no-initial-string": (
        "(void)cljonic::reduce(cljonic_reduce_max_char, cljonic::String<4>{});"),
    "Reduce-with-initial-map-entry": (
        "(void)cljonic::reduce(cljonic_reduce_entry_value, 0, "
        "cljonic::Map<int, int, 2>{cljonic::MapEntry<int, int>{1, 10}});"),
    "Reduce-no-initial-range": "(void)cljonic::reduce(cljonic_reduce_add, cljonic::Range<int>{1, 4, 1});",
    "Reduce-no-initial-repeat": "(void)cljonic::reduce(cljonic_reduce_add, cljonic::Repeat<int>{3U, 1});",
    "Reduce-no-initial-cycle": (
        "(void)cljonic::reduce(cljonic_reduce_add, cljonic::cycle(cljonic::Vector<int, 2>{}));"),
    "Reduce-no-initial-iterate": (
        "(void)cljonic::reduce(cljonic_reduce_add, cljonic::iterate(cljonic_reduce_increment, 0));"),
    "Reduce-no-initial-repeatedly": (
        "(void)cljonic::reduce(cljonic_reduce_add, cljonic::repeatedly(2U, cljonic_reduce_zero));"),
    "Reduce-with-initial-vector": "(void)cljonic::reduce(cljonic_reduce_add, 10, cljonic::Vector<int, 3>{});",
    # A generic callable is admitted by the explicit-initial form.
    "Reduce-uninspectable-with-initial": (
        "(void)cljonic::reduce([](auto lhs, auto rhs) noexcept { return lhs + rhs; }, 10, "
        "cljonic::Vector<int, 2>{});"),
    # A named function object and a function pointer are inspectable.
    "Reduce-named-function-object": (
        "(void)cljonic::reduce(cljonic_reduce_adder{}, cljonic::Vector<int, 2>{});"),
    "Reduce-function-pointer": (
        "(void)cljonic::reduce(&cljonic_reduce_named_function, cljonic::Vector<int, 2>{});"),
    # Value-preserving widening conversions are admitted for elements and initial.
    "Reduce-widening-int-to-long": ("(void)cljonic::reduce(cljonic_reduce_takes_long, cljonic::Vector<int, 2>{});"),
    "Reduce-widening-initial-int-to-long": (
        "(void)cljonic::reduce(cljonic_reduce_takes_long, 1, cljonic::Vector<int, 2>{});"),
    "Reduce-explicit-initial-accumulator-differs-from-element": (
        "(void)cljonic::reduce(cljonic_reduce_int_acc_long_element, 0, cljonic::Vector<long, 2>{});"),
    "Reduce-exact-char-parameter": (
        "(void)cljonic::reduce(cljonic_reduce_max_char, cljonic::String<4>{});"),
}

# Out-of-domain calls must report the targeted RejectionDiagnostic message
# (REQ-DIAG-009) rather than a raw list of rejected concept candidates. The
# message content is asserted, not just its stability.
DIAGNOSTIC_OPERATION = "cljonic::reduce:"
DIAGNOSTIC_SOURCE_RULE = ("The source must be a Vector, Map, Set, Queue, or String collection, or a Range, "
                          "Repeat, Cycle, Iterate, or Repeatedly producer")
DIAGNOSTIC_FUNCTION_RULE = ("The function must be copy-constructible, invoked without throwing as (accumulator, "
                            "element), and return the accumulator type")
DIAGNOSTIC_ACCUMULATOR_RULE = "The accumulator must be a storable element type"

# name -> (probe body, required substrings beyond the shared anchors)
DIAGNOSTIC_CASES = {
    "Reduce-no-initial-std-vector-diagnostic": (
        "(void)cljonic::reduce(cljonic_reduce_add, std::vector<int>{1});",
        ("expected a reducing function followed by one cljonic source",
         "The form without an initial value requires a function whose parameter types can be inspected")),
    "Reduce-uninspectable-diagnostic": (
        "(void)cljonic::reduce([](auto lhs, auto rhs) noexcept { return lhs + rhs; }, cljonic::Vector<int, 2>{});",
        ("The form without an initial value requires a function whose parameter types can be inspected",)),
    "Reduce-with-initial-std-vector-diagnostic": (
        "(void)cljonic::reduce(cljonic_reduce_add, 0, std::vector<int>{1});",
        ("expected a reducing function, an initial value, and one cljonic source",
         "The initial value must convert safely to the accumulator type")),
}

# Injected before main(): the named callables and function objects used by the
# cases above. A throwing function is declared but only ever passed to reduce, so
# no translation unit actually throws.
PREAMBLE = """
constexpr auto cljonic_reduce_add = [](int lhs, int rhs) noexcept { return lhs + rhs; };
constexpr auto cljonic_reduce_zero = []() noexcept { return 0; };
constexpr auto cljonic_reduce_increment = [](int value) noexcept { return value + 1; };
constexpr auto cljonic_reduce_max_char = [](char lhs, char rhs) noexcept { return lhs > rhs ? lhs : rhs; };
constexpr auto cljonic_reduce_entry_value = [](int total, const auto& entry) noexcept { return total + entry.value; };
constexpr auto cljonic_reduce_takes_int = [](int lhs, int rhs) noexcept { return lhs + rhs; };
constexpr auto cljonic_reduce_int_acc_long_element = [](int accumulator, long element) noexcept {
    return accumulator + (element > 0L ? 1 : 0);
};
constexpr auto cljonic_reduce_takes_long = [](long lhs, long rhs) noexcept { return lhs + rhs; };
constexpr auto cljonic_reduce_takes_float = [](float lhs, float rhs) noexcept { return lhs + rhs; };
constexpr auto cljonic_reduce_takes_char = [](char lhs, char rhs) noexcept { return lhs > rhs ? lhs : rhs; };
constexpr auto cljonic_reduce_returns_long = [](int lhs, int rhs) noexcept { return static_cast<long>(lhs) + rhs; };
constexpr auto cljonic_reduce_text_accumulator = [](std::string lhs, int rhs) noexcept { return lhs; };
auto cljonic_reduce_throwing = [](int lhs, int rhs) { return lhs + rhs; };
constexpr auto cljonic_reduce_named_add = [](int lhs, int rhs) noexcept { return lhs + rhs; };
struct cljonic_reduce_deleted_copy {
    [[nodiscard]] constexpr auto operator()(int lhs, int rhs) const noexcept -> int {
        return lhs + rhs;
    }

    cljonic_reduce_deleted_copy() = default;
    cljonic_reduce_deleted_copy(const cljonic_reduce_deleted_copy&) = delete;
};
struct cljonic_reduce_adder {
    [[nodiscard]] constexpr auto operator()(int lhs, int rhs) const noexcept -> int {
        return lhs + rhs;
    }
};
int cljonic_reduce_named_function(int lhs, int rhs) noexcept { return lhs + rhs; }
int& cljonic_reduce_reference_result(int& accumulator, int element) noexcept {
    accumulator += element;
    return accumulator;
}
"""


def build_source(header: str, preamble: str, body: str) -> str:
    return "\n".join(
        (
            "#include <array>",
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
        DIAGNOSTIC_SOURCE_RULE,
        DIAGNOSTIC_FUNCTION_RULE,
        DIAGNOSTIC_ACCUMULATOR_RULE,
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
        print("reduce-compile-fail:failures:")
        print("\n".join(f"  {failure}" for failure in failures))
        return 1

    print("reduce-compile-fail:ok")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
