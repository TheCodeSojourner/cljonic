---
type: Decision
symbol: 🎯
title: seqable-capability-is-closed
---
`Seqable` is a closed capability (human decision, 2026-10-31). Third-party and new types must never be able to declare themselves `Seqable`: no concept specialization, trait hook, adopter trait, or structural-similarity admission exists, and none may be added.

- New requirement `REQ-CAP-014` carries the obligation; `REQ-CAP-003` and `REQ-CAP-008` were amended, and stale "deferred" wording in `REQ-COLL-020B`–`020E` / `020S` was replaced.
- Specs: `SeqableLifecycleIsDeferred` is renamed `SeqableCapabilityIsClosed` in the five collection entities; `SequenceableCollection` gained `ClosedToExternalDeclaration`.
- Closure is per capability. `Indexed`, `Lookup`, `Associative`, and `Conjable` remain unchanged and would need their own decision.
- `SequenceableCollection` is only a `count()` observation baseline and never confers `Seqable`.
