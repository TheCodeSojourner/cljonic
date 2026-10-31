---
type: Design
title: Capability Closure Policy
status: active
tags: [capabilities, concepts, seqable, extension-points]
related: [requirements/requirements-module-2.md, architecture.md, vocabulary.md, specs/capabilities/concepts.allium, specs/collections/vector.allium]
depends-on: [collection-api-surface-discipline]
---

Cljonic's canonical capability names (`Indexed`, `Lookup`, `Seqable`, `Associative`, `Conjable`) are semantic capability identities, not extension points. Capability membership is decided by the library.

Closure decision (human, 2026-10-31): `Seqable` is **closed**. No third-party or new type may declare itself `Seqable` — there is no concept-specialization point, trait hook, adopter-supplied trait, or structural-similarity admission. This is `REQ-CAP-014`; it replaced the earlier "lifecycle deferred" wording, which described a wait-for-implementation state rather than an extensibility promise.

Propagation:
- Requirements: `REQ-CAP-003`, `REQ-CAP-008`, `REQ-CAP-014`, `REQ-COLL-020B`–`020E`, `REQ-COLL-020S`.
- Vocabulary: `Seqable`, plus the new `ClosedCapability` term.
- Architecture: `S2_canonical_capabilities` and `S2_capability_participation` state closure instead of deferral.
- Specs: each collection entity carries `seqable_capability_is_closed` / `SeqableCapabilityIsClosed`; `SequenceableCollection` carries `ClosedToExternalDeclaration`.

The closure is stated per capability. It does not by itself close `Indexed`, `Lookup`, `Associative`, or `Conjable`; extending it requires a separate decision and its own propagation.

Note: `SequenceableCollection` is a structural observation baseline (non-throwing `count()`), not `Seqable`. Structural satisfaction of `SequenceableCollection` never confers `Seqable`.
