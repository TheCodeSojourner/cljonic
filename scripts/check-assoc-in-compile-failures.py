#!/usr/bin/env python3
"""Verify assoc_in/can_assoc_in rejection diagnostics and admitted nested calls."""

from __future__ import annotations

import os
import re
import subprocess


DIAGNOSTIC_CASES = {
    "AssocIn-non-associative-root": (
        "(void)cljonic::core::assoc_in(cljonic::Set<int, 4>{}, cljonic::core::vector<int, 1>(1), 1);",
        ("cljonic::assoc_in:", "Map or Vector root"),
    ),
    "AssocIn-path-is-not-vector": (
        "(void)cljonic::core::assoc_in(cljonic::Map<int, int, 4>{}, 1, 10);",
        ("cljonic::assoc_in:", "path must be exactly one"),
    ),
    "AssocIn-empty-path": (
        "(void)cljonic::core::assoc_in(cljonic::Map<int, int, 4>{}, cljonic::core::vector<int>(), 10);",
        ("cljonic::assoc_in:", "nonempty Vector path"),
    ),
    "AssocIn-wrong-final-value": (
        "(void)cljonic::core::assoc_in(cljonic::Map<int, int, 4>{}, cljonic::core::vector<int, 1>(1), 1.5);",
        ("cljonic::assoc_in:", "final value"),
    ),
    "CanAssocIn-non-associative-root": (
        "(void)cljonic::core::can_assoc_in(cljonic::Set<int, 4>{}, cljonic::core::vector<int, 1>(1), 1);",
        ("cljonic::can_assoc_in:", "Map or Vector root"),
    ),
    "CanAssocIn-wrong-final-value": (
        "(void)cljonic::core::can_assoc_in(cljonic::Vector<int, 4>{10}, cljonic::core::vector<int, 1>(0), 1.5);",
        ("cljonic::can_assoc_in:", "final value"),
    ),
}

PASS_CASES = {
    "AssocIn-map": "(void)cljonic::core::assoc_in(cljonic::Map<int, int, 4>{}, cljonic::core::vector<int, 1>(1), 10);",
    "AssocIn-nested-map": "(void)cljonic::core::assoc_in(cljonic::Map<int, cljonic::Map<int, int, 4>, 4>{}, cljonic::core::vector<int, 2>(1, 2), 10);",
    "AssocIn-nested-vector": "(void)cljonic::core::assoc_in(cljonic::Vector<cljonic::Vector<int, 4>, 2>{}, cljonic::core::vector<int, 2>(0, 0), 10);",
    "CanAssocIn-map": "(void)cljonic::core::can_assoc_in(cljonic::Map<int, int, 4>{}, cljonic::core::vector<int, 1>(1), 10);",
    "CanAssocIn-nested-map": "(void)cljonic::core::can_assoc_in(cljonic::Map<int, cljonic::Map<int, int, 4>, 4>{}, cljonic::core::vector<int, 2>(1, 2), 10);",
}


def compile_source(compiler: list[str], include_dir: str, source: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [*compiler, "-std=c++23", "-fsyntax-only", "-I", include_dir, "-x", "c++", "-"],
        input=source,
        text=True,
        capture_output=True,
        check=False,
    )


def normalized(text: str) -> str:
    return re.sub(r"\s+", " ", text)


def main() -> int:
    compiler = os.environ.get("CXX", "c++").split()
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    failures: list[str] = []
    for name, include_dir, header in (
        ("modular", os.path.join(root, "src"), "cljonic-core.hpp"),
        ("single-header", root, "cljonic.hpp"),
    ):
        for case, (body, needles) in DIAGNOSTIC_CASES.items():
            source = f'#include "{header}"\nint main() {{ {body} return 0; }}\n'
            result = compile_source(compiler, include_dir, source)
            if result.returncode == 0:
                failures.append(f"{name}/{case}: expected compile failure")
                continue
            output = normalized(result.stdout + result.stderr)
            if not all(normalized(needle) in output for needle in needles):
                failures.append(f"{name}/{case}: targeted diagnostic not reported")
        for case, body in PASS_CASES.items():
            source = f'#include "{header}"\nint main() {{ {body} return 0; }}\n'
            result = compile_source(compiler, include_dir, source)
            if result.returncode != 0:
                failures.append(f"{name}/{case}: expected to compile")
    if failures:
        print("assoc-in-compile-fail:failures:")
        print("\n".join(f"  {failure}" for failure in failures))
        return 1
    print("assoc-in-compile-fail:ok")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
