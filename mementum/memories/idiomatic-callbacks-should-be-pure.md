---
type: Decision
symbol: 🎯
title: Idiomatic cljonic callbacks should be pure functions
---
Human clarification (2026-11-04): idiomatic cljonic callbacks should be pure functions. When considering deferred producer tails or repeated observations, callback replay therefore ordinarily adds runtime cost, not a state-management problem.

Some callbacks, such as those supplied to `repeatedly`, are likely not pure; these are exceptions, not the default design assumption. Preserve their explicitly authorized callback-state and effect contracts rather than assuming every callback is pure.

Purity is a recommendation, not a universal enforced requirement. Discuss replay primarily as an evaluation-cost tradeoff, and address state/effect behavior separately where an operation's contract permits impure callbacks.
