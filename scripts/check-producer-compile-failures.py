#!/usr/bin/env python3
"""Verify the producer factory functions (range, repeat, cycle, iterate, repeatedly)
reject out-of-domain arguments with the targeted RejectionDiagnostic message
(REQ-DIAG-009) instead of a raw list of rejected concept candidates, for both
modular and single-header builds.

The message content is asserted, not just its stability: REQ-DIAG-009 requires
the fallback to name the operation and the violated producer-admission rule, so
the checks below pin the operation identity and its required rule text.
"""

from __future__ import annotations

import os
import re
from compile_probe import compile_probe

# Out-of-domain calls must fail to compile.
FAILURE_CASES = {
    "Range-empty": "(void)cljonic::core::range();",
    "Range-extra-arguments": "(void)cljonic::core::range(0, 5, 1, 2);",
    "Range-typed-extra-arguments": "(void)cljonic::core::range<int>(0, 5, 1, 2);",
    "Range-unsigned": "(void)cljonic::core::range(5U);",
    "Range-boolean": "(void)cljonic::core::range(true);",
    "Range-floating": "(void)cljonic::core::range(5.0);",
    "Range-mixed-types": "(void)cljonic::core::range(0, 5L);",
    "Range-mixed-step": "(void)cljonic::core::range(0, 5, 1L);",
    "Range-typed-unsigned": "(void)cljonic::core::range<unsigned int>(5U);",
    "Range-typed-void": "(void)cljonic::core::range<void>();",
    "Range-narrowing": "(void)cljonic::core::range<int>(5LL);",
    "Range-floating-conversion": "(void)cljonic::core::range<int>(5.0);",
    "Range-start-conversion": "(void)cljonic::core::range<int>(0LL, 5);",
    "Range-step-conversion": "(void)cljonic::core::range<int>(0, 5, 1LL);",
    "Cycle-non-source": "auto x = cljonic::core::cycle(42); (void)x;",
    "Repeatedly-non-callable": "auto x = cljonic::core::repeatedly(42); (void)x;",
    "Repeatedly-counted-non-callable": "auto x = cljonic::core::repeatedly(3, 42); (void)x;",
    "Iterate-non-callable": "auto x = cljonic::core::iterate(42, 1); (void)x;",
    "Repeat-non-storable": "auto x = cljonic::core::repeat(NonStorable{}); (void)x;",
    "Repeat-counted-non-storable": "auto x = cljonic::core::repeat(3U, NonStorable{}); (void)x;",
    "Repeat-value-first-factory": "(void)cljonic::core::repeat(cljonic::Vector{7}, 3);",
    "Repeat-value-first-constructor": "(void)cljonic::Repeat<cljonic::Vector<int, 1>>{cljonic::Vector{7}, 3};",
    "Repeat-value-first-deduction": "(void)cljonic::Repeat{cljonic::Vector{7}, 3};",
}

# In-domain calls must still compile.
PASS_CASES = {
    "Range-deduced": "static_assert(cljonic::core::range(5) == cljonic::Range{5});",
    "Range-bounds": "static_assert(cljonic::core::range(0, 5) == cljonic::Range{0, 5});",
    "Range-step": "static_assert(cljonic::core::range(5, 0, -2) == cljonic::Range{5, 0, -2});",
    "Range-typed-empty": "static_assert(cljonic::core::range<int>() == cljonic::Range<int>{});",
    "Range-typed-end": "static_assert(cljonic::core::range<long>(5) == cljonic::Range<long>{5});",
    "Range-typed-bounds": "static_assert(cljonic::core::range<long>(0, 5) == cljonic::Range<long>{0, 5});",
    "Range-typed-step": "static_assert(cljonic::core::range<long>(5, 0, -2) == cljonic::Range<long>{5, 0, -2});",
    "Repeat-value": "auto x = cljonic::core::repeat(7); (void)x;",
    "Repeat-counted": "auto x = cljonic::core::repeat(3U, 7); (void)x;",
    "Repeat-counted-constructor": "auto x = cljonic::Repeat<int>{3U, 7}; (void)x;",
    "Repeat-counted-deduction": "auto x = cljonic::Repeat{3U, 7}; (void)x;",
    "Repeat-signed-count": "static_assert(cljonic::core::repeat(3, 7).count() == 3);",
    "Repeat-negative-count": "static_assert(cljonic::Repeat{-2, 7}.count() == 0);",
    "Repeat-numeric-order": "static_assert(cljonic::core::repeat(7, 3U).count() == 7);",
    "Repeatedly-signed-count": "static_assert(cljonic::core::repeatedly(3, []() noexcept { return 7; }).count() == 3);",
    "Repeatedly-negative-count": "static_assert(cljonic::core::repeatedly(-2, []() noexcept { return 7; }).count() == 0);",
    "Cycle-collection": "auto x = cljonic::core::cycle(cljonic::Vector<int, 3>{1, 2, 3}); (void)x;",
    "Iterate-step": "auto x = cljonic::core::iterate([](int v) noexcept { return v + 1; }, 0); (void)x;",
    "Repeatedly-named": "constexpr auto five = []() noexcept { return 5; }; auto x = cljonic::core::repeatedly(five); (void)x;",
    "Repeatedly-counted": "constexpr auto five = []() noexcept { return 5; }; auto x = cljonic::core::repeatedly(3U, five); (void)x;",
}

ZERO_CAP_PASS_CASES = {
    "Iterate-next-does-not-invoke-step": """\
static_assert([] {
    auto calls = 0;
    const auto source = cljonic::core::iterate([&calls](int value) noexcept { ++calls; return value + 1; }, 7);
    const auto tail = cljonic::core::next(source);
    return calls == 0 && source.count() == 0 && tail.count() == 0;
}());
""",
}

PREDICATE_CAP_CASE = """\
static_assert([] {
    constexpr auto cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    int produced = 0;
    int tested = 0;
    const auto source = cljonic::core::repeatedly([&produced]() noexcept { return ++produced; });
    const auto prefix = cljonic::core::take_while([&tested](int) noexcept { ++tested; return true; }, source);
    if (produced != 0 || tested != 0 || prefix.count() != cap ||
        produced != static_cast<int>(cap) || tested != static_cast<int>(cap)) {
        return false;
    }
    produced = 0;
    tested = 0;
    const auto suffix = cljonic::core::drop_while([&tested](int n) noexcept { ++tested; return n < 2; }, source);
    if (produced != 0 || tested != 0 || suffix.count() != (cap == 0 ? 0 : cap - 1) ||
        produced != static_cast<int>(cap) || tested != static_cast<int>(cap < 2 ? cap : 2)) {
        return false;
    }
    const auto short_source = cljonic::core::repeatedly(2, []() noexcept { return 1; });
    if (cljonic::core::take_while([](int) noexcept { return true; }, short_source).count() != (cap < 2 ? cap : 2) ||
        cljonic::core::drop_while([](int) noexcept { return true; }, short_source).count() != 0) {
        return false;
    }
    produced = 0;
    tested = 0;
    const auto nested = cljonic::core::cycle(cljonic::core::cycle(prefix));
    if (cljonic::core::first(nested) != (cap == 0 ? 0 : 1) ||
        produced != (cap == 0 ? 0 : 1) || tested != (cap == 0 ? 0 : 1)) {
        return false;
    }
    produced = 0;
    tested = 0;
    if (cljonic::core::first(suffix) != (cap < 2 ? 0 : 2) ||
        produced != static_cast<int>(cap < 2 ? cap : 2) || tested != produced) {
        return false;
    }
    produced = 0;
    const auto iterative = cljonic::core::iterate([&produced](int n) noexcept { ++produced; return n + 1; }, 7);
    return cljonic::core::first(iterative) == (cap == 0 ? 0 : 7) && produced == 0;
}());
"""

PREDICATE_TAIL_CAP_CASE = """\
static_assert([] {
    constexpr auto cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    int calls = 0;
    const auto source = cljonic::core::repeatedly([&calls, n = 0]() mutable noexcept { ++calls; return ++n; });
    const auto prefix = cljonic::core::take_while([](int) noexcept { return true; }, source);
    const auto tail = cljonic::core::rest(prefix);
    const auto suffix = cljonic::core::drop_while([](int n) noexcept { return n < 2; }, source);
    const auto suffix_tail = cljonic::core::next(suffix);
    if (calls != 0 || tail.count() != (cap == 0 ? 0 : cap - 1) || calls != static_cast<int>(cap)) {
        return false;
    }
    calls = 0;
    if (suffix_tail.count() != (cap < 2 ? 0 : cap - 2) || calls != static_cast<int>(cap)) {
        return false;
    }
    calls = 0;
    const auto cycle_tail = cljonic::core::rest(cljonic::core::cycle(cljonic::core::cycle(prefix)));
    if (calls != static_cast<int>(cap)) {
        return false;
    }
    calls = 0;
    if (cljonic::core::first(cycle_tail) != (cap == 0 ? 0 : cap == 1 ? 1 : 2) ||
        calls != (cap == 0 ? 0 : cap == 1 ? 1 : 2)) {
        return false;
    }
    auto skipped = cljonic::core::take_while([](int) noexcept { return true; }, cljonic::core::repeat(7));
    for (unsigned n = 0; n < cap + 3; ++n) {
        if (skipped.count() != (n < cap ? cap - n : 0)) {
            return false;
        }
        skipped = cljonic::core::next(skipped);
    }
    return true;
}());
"""

PREAMBLE = """\
namespace {
struct NonStorable {
    NonStorable() = default;
    NonStorable(const NonStorable&) = delete;
};
} // namespace
"""

# name -> (probe body, required operation identity, required rule substring)
DIAGNOSTIC_CASES = {
    "Range-empty-diagnostic": ("(void)cljonic::core::range();", "cljonic::range:", "explicit element type"),
    "Range-type-diagnostic": ("(void)cljonic::core::range(5U);", "cljonic::range:", "signed integral"),
    "Range-arity-diagnostic": ("(void)cljonic::core::range(0, 5, 1, 2);", "cljonic::range:", "zero through three"),
    "Range-mixed-diagnostic": ("(void)cljonic::core::range(0, 5L);", "cljonic::range:", "same signed integral type"),
    "Range-conversion-diagnostic": ("(void)cljonic::core::range<int>(5LL);", "cljonic::range:", "safely convert"),
    "Cycle-diagnostic": ("(void)cljonic::core::cycle(42);", "cljonic::cycle:", "cannot be used to build a Cycle"),
    "Repeatedly-diagnostic": (
        "(void)cljonic::core::repeatedly(42);",
        "cljonic::repeatedly:",
        "cannot be used to build a Repeatedly",
    ),
    "Iterate-diagnostic": ("(void)cljonic::core::iterate(42, 1);", "cljonic::iterate:", "cannot be used to build an Iterate"),
    "Repeat-diagnostic": ("(void)cljonic::core::repeat(NonStorable{});", "cljonic::repeat:", "cannot be repeated"),
}


def compile_case(
    compiler: list[str], include_dir: str, header: str, body: str, preamble: str = "", *, expect_success: bool = False
) -> bool:
    source = "\n".join(
        (
            f'#include "{header}"',
            preamble,
            "int main() {",
            f"  {body}",
            "  return 0;",
            "}",
        )
    )
    result = compile_probe(tuple(compiler), include_dir, source)
    if expect_success and result.returncode != 0:
        print(f"producer-compile-fail: compiler output for {header}:\n{result.stdout}{result.stderr}")
    return result.returncode == 0


def _normalized(text: str) -> str:
    return re.sub(r"\s+", " ", text)


def diagnostic_message_reported(
    compiler: list[str], include_dir: str, header: str, body: str, operation: str, required: str
) -> bool:
    source = "\n".join(
        (
            f'#include "{header}"',
            PREAMBLE,
            "int main() {",
            f"  {body}",
            "  return 0;",
            "}",
        )
    )
    result = compile_probe(tuple(compiler), include_dir, source)
    if result.returncode == 0:
        return False
    output = _normalized(result.stdout + result.stderr)
    return all(_normalized(needle) in output for needle in (operation, required))


def main() -> int:
    compiler = os.environ.get("CXX", "c++").split()
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    configurations = (
        ("modular", os.path.join(root, "src"), "cljonic-core.hpp"),
        ("single-header", root, "cljonic.hpp"),
    )
    failures: list[str] = []

    for configuration, include_dir, header in configurations:
        for name, body in FAILURE_CASES.items():
            if compile_case(compiler, include_dir, header, body, preamble=PREAMBLE):
                failures.append(f"{configuration}/{name}: expected compile failure, but compiled")

        for name, body in PASS_CASES.items():
            if not compile_case(compiler, include_dir, header, body, preamble=PREAMBLE, expect_success=True):
                failures.append(f"{configuration}/{name}: expected to compile, but failed")

        for name, (body, operation, required) in DIAGNOSTIC_CASES.items():
            if not diagnostic_message_reported(compiler, include_dir, header, body, operation, required):
                failures.append(f"{configuration}/{name}: targeted rejection diagnostic content not reported")

    # The complete core header requires String<1>; isolate the zero-cap producer contract.
    for name, body in ZERO_CAP_PASS_CASES.items():
        zero_cap_compiler = [*compiler, "-DCLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT=0"]
        if not compile_case(
            zero_cap_compiler, os.path.join(root, "src"), "cljonic-next.hpp", body, expect_success=True
        ):
            failures.append(f"modular/{name}: zero-cap behavior failed to compile")

    for cap in (0, 1, 3):
        cap_compiler = [*compiler, f"-DCLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT={cap}"]
        cap_configurations = configurations if cap != 0 else (
            ("modular", os.path.join(root, "src"), "cljonic-producer-drop-while.hpp"),
        )
        for configuration, include_dir, header in cap_configurations:
            if not compile_case(
                cap_compiler, include_dir, header, PREDICATE_CAP_CASE + PREDICATE_TAIL_CAP_CASE,
                preamble=(
                    '#include "cljonic-producer-repeatedly.hpp"\n'
                    '#include "cljonic-producer-iterate.hpp"\n'
                    '#include "cljonic-producer-cycle.hpp"\n'
                    '#include "cljonic-rest.hpp"\n'
                    '#include "cljonic-first.hpp"'
                ) if configuration == "modular" else "",
                expect_success=True,
            ):
                failures.append(f"{configuration}/predicate-cap-{cap}: bounded traversal contract failed")

    if failures:
        print("producer-compile-fail:failures:")
        print("\n".join(f"  {failure}" for failure in failures))
        return 1

    print("producer-compile-fail:ok")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
