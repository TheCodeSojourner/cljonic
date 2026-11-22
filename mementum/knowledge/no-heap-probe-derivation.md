---
type: Reference
title: No-Heap Probe Derivation
status: active
category: verification
tags: [no-heap, probes, verification, derivation, evidence, contract]
related: [no-heap-probes-propagate-with-feature.md, no-heap-symbol-scan-gate.md, cljonic-test-is-a-probe-not-the-suite.md, verification-signal-discipline.md]
depends-on: []
---

Runtime no-heap probes must be derived from the contract, not from the existing tests. Reading the test file and mirroring its cases reproduces the test's blind spots; the probe then "passes" while leaving obligations unproven.

Derivation procedure:
- Enumerate the contract's obligation rows first (entity fields + invariant names from the spec snapshot), then build one table row per obligation before touching any probe or example.
- For each row decide independently whether it is covered by an ordinary assertion, a runtime no-heap probe assertion, a Doxygen demonstration, or a reasoned non-applicable. `exists(file)` and `pass(gate)` and `present(TRACE_ID)` are not sufficient evidence.
- Inspect the existing probe body even when it is unchanged: a registered probe that never exercises the new behavior is indistinguishable from a missing one at the gate level.

Recurring gap classes to check explicitly (each produced a real gap in `last`, 2026-11-21):
- Family coverage: adapters and source-shaping producers (`take_while`, `drop_while`, `cycle`, `repeat`, `iterate`) are separate source families; a probe covering only `vector`/`string` leaves them unproven.
- Callback-state distinction: an owned capture (`[value=1] mutable`) resets per traversal while a referenced capture (`[&shared]`) persists across traversals. These prove different obligations and both need a case.
- Element types without `operator==`: an element type lacking equality proves the "no additional element equality/ordering/assignment required" obligation; asserting only on `int` elements cannot.
- Whole-vs-projection: an element type returned by an inner producer stays whole, while the same type as a top-level argument may project. Probe both shapes.
- Vacuous boundary assertions: when a boundary obligation (zero count, zero capacity, empty source) yields the same value whether or not traversal happened, assert the observable side effect (a callback counter that must stay zero) instead of only the value.
- Input preservation: assert the source is unchanged after the call, per source family, including the container types whose "final element" is read without removal.

Runtime-derived inputs prevent constant folding; `noexcept`, `static_assert`, source scans, and symbol scans are not runtime probe coverage. A rejected form with no admitted runtime counterpart gets a table explanation and keeps its compile-fail test rather than a runnable probe case.
