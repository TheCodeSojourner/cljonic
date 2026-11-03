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

## Approval gate

1. Keep implementation changes subordinate to approved requirements and architecture; do not use existing code or tests to silently redefine behavioral truth.

## Next work after approval

1. Reconcile the operation-level behavioral specifications required by `REQ-SEQ-022`; the shared `collection-shaping.allium` remains deferred and does not make its listed operations implementation-ready.
2. Select one remaining operation contract and name its behavior before implementation readiness is claimed; `take` now has its own active contract.
3. Propagate approved contracts into architecture, focused tests, traceability, and implementation in that order.
4. Re-run the relevant vocabulary, architecture, specification, traceability, build, no-heap, and coverage gates after each coherent increment.

## Deferred until their contracts exist

Do not promote unresolved operation families, collection return/capacity policies, key/value constraints, or runtime-only behavior into implementation-ready status. Record each decision in its owning behavioral specification first, then propagate it downstream.

## Retired agenda material

The former keyword-catalog, initial map milestone, and broad collection design-question list described an earlier planning stage. They are retained only in git history, not as current work items.
