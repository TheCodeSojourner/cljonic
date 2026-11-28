#!/ usr / bin / env python3
"""Verify identity rejects invalid arity and non-nothrow-copyable arguments."""

from __future__ import annotations

import os
import re
import subprocess


DIAGNOSTIC_CASES = {
    "zero-arguments": ("(void)cljonic::core::identity();", "expected exactly one argument"),
    "multiple-arguments": ("(void)cljonic::core::identity(1, 2);", "expected exactly one argument"),
    "throwing-copy": (
        "struct ThrowingCopy { ThrowingCopy() noexcept = default; ThrowingCopy(const ThrowingCopy&) {} }; "
        "(void)cljonic::core::identity(ThrowingCopy{});",
        "argument must be nothrow copy constructible",
    ),
}


def build_source(header: str, body: str) -> str:
    return "\n".join(
        (
            f'#include "{header}"',
            "int main() {",
            f"  {body}",
            "  return 0;",
            "}",
        )
    )


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
    configurations = (
        ("modular", os.path.join(root, "src"), "cljonic-core.hpp"),
        ("single-header", root, "cljonic.hpp"),
    )
    failures: list[str] = []

    for configuration, include_dir, header in configurations:
        for name, (body, message) in DIAGNOSTIC_CASES.items():
            result = compile_source(compiler, include_dir, build_source(header, body))
            if result.returncode == 0:
                failures.append(f"{configuration}/{name}: expected compile failure, but compiled")
                continue
            output = normalized(result.stdout + result.stderr)
            if "cljonic::identity:" not in output or message not in output:
                failures.append(f"{configuration}/{name}: targeted rejection diagnostic content not reported")

    if failures:
        print("identity-compile-fail:failures:")
        print("\n".join(f"  {failure}" for failure in failures))
        return 1

    print("identity-compile-fail:ok")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
