#!/usr/bin/env python3
"""Verify filter admission and targeted rejections in modular and single-header builds."""

from __future__ import annotations

import os
import re
import subprocess

FAILURES = {
    "std-vector": "(void)cljonic::core::filter(cljonic_filter_positive, std::vector<int>{1});",
    "scalar-source": "(void)cljonic::core::filter(cljonic_filter_positive, 1);",
    "throwing-predicate": "(void)cljonic::core::filter(cljonic_filter_throwing, cljonic::Vector<int, 2>{});",
    "noncopyable-predicate": "(void)cljonic::core::filter(cljonic_filter_deleted_copy{}, cljonic::Vector<int, 2>{});",
    "non-bool-result": "(void)cljonic::core::filter(cljonic_filter_int_result, cljonic::Vector<int, 2>{});",
    "narrowing-parameter": "(void)cljonic::core::filter(cljonic_filter_short, cljonic::Vector<int, 2>{});",
    "domain-crossing-parameter": "(void)cljonic::core::filter(cljonic_filter_char, cljonic::Vector<int, 2>{});",
    "too-many-sources": "(void)cljonic::core::filter(cljonic_filter_positive, cljonic::Vector<int, 2>{}, cljonic::Vector<int, 2>{});",
}

PASSES = {
    "vector": "(void)cljonic::core::filter(cljonic_filter_positive, cljonic::Vector<int, 2>{});",
    "safe-widening": "(void)cljonic::core::filter(cljonic_filter_long, cljonic::Vector<int, 2>{});",
    "generic-lambda": "(void)cljonic::core::filter([](const auto& value) noexcept { return value > 0; }, cljonic::Vector<int, 2>{});",
    "function-pointer": "(void)cljonic::core::filter(&cljonic_filter_function, cljonic::Vector<int, 2>{});",
    "map-entry": "(void)cljonic::core::filter(cljonic_filter_entry, cljonic::Map<int, int, 2>{});",
    "producer": "(void)cljonic::core::filter(cljonic_filter_positive, cljonic::Range<int>{1, 3, 1});",
}

FILTERV_FAILURES = {
    "std-vector": "(void)cljonic::core::filterv(cljonic_filter_positive, std::vector<int>{1});",
    "scalar-source": "(void)cljonic::core::filterv(cljonic_filter_positive, 1);",
    "throwing-predicate": "(void)cljonic::core::filterv(cljonic_filter_throwing, cljonic::Vector<int, 2>{});",
    "noncopyable-predicate": "(void)cljonic::core::filterv(cljonic_filter_deleted_copy{}, cljonic::Vector<int, 2>{});",
    "non-bool-result": "(void)cljonic::core::filterv(cljonic_filter_int_result, cljonic::Vector<int, 2>{});",
    "narrowing-parameter": "(void)cljonic::core::filterv(cljonic_filter_short, cljonic::Vector<int, 2>{});",
    "domain-crossing-parameter": "(void)cljonic::core::filterv(cljonic_filter_char, cljonic::Vector<int, 2>{});",
    "too-many-sources": "(void)cljonic::core::filterv(cljonic_filter_positive, cljonic::Vector<int, 2>{}, cljonic::Vector<int, 2>{});",
}

FILTERV_PASSES = {
    "vector": "(void)cljonic::core::filterv(cljonic_filter_positive, cljonic::Vector<int, 2>{});",
    "safe-widening": "(void)cljonic::core::filterv(cljonic_filter_long, cljonic::Vector<int, 2>{});",
    "generic-lambda": "(void)cljonic::core::filterv([](const auto& value) noexcept { return value > 0; }, cljonic::Vector<int, 2>{});",
    "producer": "(void)cljonic::core::filterv(cljonic_filter_positive, cljonic::Range<int>{1, 3, 1});",
}

PREAMBLE = r"""
constexpr auto cljonic_filter_positive = [](int value) noexcept { return value > 0; };
constexpr auto cljonic_filter_long = [](long value) noexcept { return value > 0L; };
constexpr auto cljonic_filter_short = [](short value) noexcept { return value > 0; };
constexpr auto cljonic_filter_char = [](char value) noexcept { return value == 'a'; };
constexpr auto cljonic_filter_int_result = [](int) noexcept { return 1; };
auto cljonic_filter_throwing = [](int value) { return value > 0; };
constexpr auto cljonic_filter_entry = [](const auto& entry) noexcept { return entry.value > 0; };
bool cljonic_filter_function(int value) noexcept { return value > 0; }
struct cljonic_filter_deleted_copy {
    cljonic_filter_deleted_copy() = default;
    cljonic_filter_deleted_copy(const cljonic_filter_deleted_copy&) = delete;
    bool operator()(int value) const noexcept { return value > 0; }
};
"""

DIAGNOSTIC = "cljonic::filter: expected a copy-constructible predicate"
FILTERV_DIAGNOSTIC = "cljonic::filterv: expected a copy-constructible predicate"


def compile_case(compiler: list[str], include_dir: str, header: str, body: str) -> tuple[bool, str]:
    source = "\n".join(
        (
            "#include <vector>",
            f'#include "{header}"',
            PREAMBLE,
            "int main() {",
            f"  {body}",
            "  return 0;",
            "}",
        )
    )
    result = subprocess.run(
        [*compiler, "-std=c++23", "-fsyntax-only", "-I", include_dir, "-x", "c++", "-"],
        input=source,
        text=True,
        capture_output=True,
        check=False,
    )
    return result.returncode == 0, re.sub(r"\s+", " ", result.stdout + result.stderr)


def main() -> int:
    compiler = os.environ.get("CXX", "c++").split()
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    configurations = (("modular", os.path.join(root, "src"), "cljonic-core.hpp"),
                      ("single-header", root, "cljonic.hpp"))
    failures: list[str] = []
    for configuration, include_dir, header in configurations:
        for name, body in PASSES.items():
            succeeded, output = compile_case(compiler, include_dir, header, body)
            if not succeeded:
                failures.append(f"{configuration}/{name}: expected success: {output}")
        for name, body in FAILURES.items():
            succeeded, output = compile_case(compiler, include_dir, header, body)
            if succeeded:
                failures.append(f"{configuration}/{name}: expected compile failure")
            elif DIAGNOSTIC not in output:
                failures.append(f"{configuration}/{name}: targeted filter diagnostic missing: {output}")
        for name, body in FILTERV_PASSES.items():
            succeeded, output = compile_case(compiler, include_dir, header, body)
            if not succeeded:
                failures.append(f"{configuration}/filterv-{name}: expected success: {output}")
        for name, body in FILTERV_FAILURES.items():
            succeeded, output = compile_case(compiler, include_dir, header, body)
            if succeeded:
                failures.append(f"{configuration}/filterv-{name}: expected compile failure")
            elif FILTERV_DIAGNOSTIC not in output:
                failures.append(f"{configuration}/filterv-{name}: targeted filterv diagnostic missing: {output}")
    if failures:
        print("filter-compile-fail:failures:")
        print("\n".join(f"  {failure}" for failure in failures))
        return 1
    print("filter-compile-fail:ok")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
