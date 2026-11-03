---
type: Reference
title: cljonic Next Discussion Agenda
status: open
description: Current agenda for defining operation-level collection-shaping behavior before propagating it into architecture, tests, and implementation.
tags: [roadmap, requirements, specifications, architecture, collections, verification]
related: [collection-api-surface-discipline.md, architecture-integrity-curation.md]
depends-on: []
---

## Current baseline (2026-11-03)

The `filterv` alias work is complete and committed. The `first`, `next`, `not_empty`, and `seq` free functions are also active and verified; they are not deferred promotion candidates. `seq` was fully activated on 2026-10-30 under binding `REQ-SEQ-010`, `REQ-SEQ-011`, `REQ-SEQ-013`, and `REQ-SEQ-014`.

`take(count, source)` is active and verified under `REQ-FN-037`: it returns an owning Vector prefix with source-bound capacity over all cljonic collections and producers, using Repeat count admission and normalization.

`drop(count, source)` is active and verified under `REQ-FN-038`. It returns a take-style owning Vector suffix within the original bounded observation window; skipping does not replenish the producer observation budget. Requests covering the window count bound return empty without traversal or callbacks. All 32 obligations have exact test traces; modular/single-header tests, 100% drop/shared-helper coverage, targeted diagnostics, runtime no-heap probes, docs-example compilation/execution, and the repository fast gate passed. Product changes and regenerated rendered docs are committed; no further drop implementation or docs regeneration remains.

## Approval gate

1. Keep implementation changes subordinate to approved requirements and architecture; do not use existing code or tests to silently redefine behavioral truth.

## Next work after approval

1. Reconcile the operation-level behavioral specifications required by `REQ-SEQ-022`; the shared `collection-shaping.allium` remains deferred and does not make its listed operations implementation-ready.
2. `take` and `drop` are complete. Select another remaining operation only on human direction; neither should be suggested as pending implementation.
3. Propagate approved contracts into architecture, focused tests, traceability, and implementation in that order.
4. Re-run the relevant vocabulary, architecture, specification, traceability, build, no-heap, and coverage gates after each coherent increment.

## Deferred until their contracts exist

Do not promote unresolved operation families, collection return/capacity policies, key/value constraints, or runtime-only behavior into implementation-ready status. Record each decision in its owning behavioral specification first, then propagate it downstream.

## Retired agenda material

The former keyword-catalog, initial map milestone, and broad collection design-question list described an earlier planning stage. They are retained only in git history, not as current work items.
