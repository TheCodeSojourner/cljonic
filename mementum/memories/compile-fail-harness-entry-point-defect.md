---
type: Insight
symbol: ❌
title: compile-fail-harness-entry-point-defect
related: [verification-signal-discipline.md, spec-to-code-strict-fail-gate.md, gybis-fini-gates-before-commit.md]
---
A new compile-fail harness ended with `if __name__ == "__main__":
raise SystemExit(main)` — missing the call parentheses. Running it printed
`<function main at 0x…>` and exited 1, and the make target failed in 0.09s
during `make git`. Earlier in the same session the defect looked like a "heredoc
artifact" because verification went through `importlib` + an explicit `main()`
call, which bypassed the real entry point.

Two durable lessons:
1. Verify a gate through **the same entry point the gate uses** — run the make
   target (or `scripts/run-compile-fail.py`), not an ad-hoc import. A passing
   ad-hoc invocation proves nothing about the shipped path.
2. A compile-fail target that completes suspiciously fast is a failure signal,
   not a win. Real harnesses compile dozens of cases in both builds; seconds,
   not milliseconds.
