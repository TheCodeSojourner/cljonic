---
type: Reference
title: cljonic Next Discussion Agenda
status: open
description: Current agenda for defining operation-level collection-shaping behavior before propagating it into architecture, tests, and implementation.
tags: [roadmap, requirements, specifications, architecture, collections, verification]
related: [collection-api-surface-discipline.md, architecture-integrity-curation.md]
depends-on: []
---

## Current baseline (2026-11-04)

`first` now admits all seven cljonic producer families, including nested predicate
adapters and Cycles over them, under `REQ-FN-002A1`. The human approved the contract
and then explicitly invoked `/cljonic-api-complete` for implementation. Results
are owning element copies; empty or zero-cap observations yield DefaultElement.
First access does no exact-count prepass or advancement after finding the result;
DropWhile may scan its original bounded matching prefix. Cycle begin no longer
counts its source before traversal. Existing collection and MapEntry semantics,
callback-state rules are preserved. Producer-first product changes and rendered
HTML were committed by the human (dcbdf81a through f7d4bd94).

`next`/`rest` now also admit TakeWhile, DropWhile, and dependent Cycles, after
separate contract and implementation approvals. Adapter tails defer output skips
within the original window; finite adapter-source Cycle tails eagerly count
bounded output and rotate a compact scalar offset over the complete owned source.
Nested compositions are supported. Callback replay follows existing effect rules;
idiomatic callbacks should be pure. No large rotation metadata or materialization
is used. `has_next` remains collection-only.

The `filterv` alias work is complete and committed. The `first`, `next`, `not_empty`, and `seq` free functions are also active and verified; they are not deferred promotion candidates. `seq` was fully activated on 2026-10-30 under binding `REQ-SEQ-010`, `REQ-SEQ-011`, `REQ-SEQ-013`, and `REQ-SEQ-014`.

`take(count, source)` is active and verified under `REQ-FN-037`: it returns an owning Vector prefix with source-bound capacity over all cljonic collections and producers, using Repeat count admission and normalization.

`drop(count, source)` is active and verified under `REQ-FN-038`. It returns a take-style owning Vector suffix within the original bounded observation window; skipping does not replenish the producer observation budget. Requests covering the window count bound return empty without traversal or callbacks. All 32 obligations have exact test traces; modular/single-header tests, 100% drop/shared-helper coverage, targeted diagnostics, runtime no-heap probes, docs-example compilation/execution, and the repository fast gate passed. Product changes and regenerated rendered docs are committed; no further drop implementation or docs regeneration remains.

## Approval gate

1. Keep implementation changes subordinate to approved requirements and architecture; do not use existing code or tests to silently redefine behavioral truth.
2. `take_while` and `drop_while` contracts were approved together; implementation was then explicitly approved individually, take_while first and drop_while second. Each operation is implemented, independently verified, and committed (1fd49f29, e7f0a9ef, e0b92bd7, 1fe12f29, 716df1df). Contract approval did not substitute for implementation approval.

## Approved predicate-producer contracts (2026-11-03)

- `take_while(predicate, source)` yields the initial true prefix as an owning finite `TakeWhile` producer (`REQ-FN-039`). It examines but excludes the first false value and does not advance further.
- `drop_while(predicate, source)` skips only the initial true prefix as an owning finite `DropWhile` producer (`REQ-FN-040`), retains the first false value and remaining original window, and never retests the predicate after that first failure.
- Both own source/predicate copies without construction-time evaluation or a materialized cache. Original-window traversal is capped and endpoint-checked; skipping never replenishes the budget. All cljonic sources, including nested adapters and traversal-dependent Map/Set results, are admitted.
- `count` is exact by independent bounded traversal, not a cheap upper bound. It may repeat predicate/source callbacks before later consumption; `is_empty` remains `count == 0`. Fit/count predictions require equivalent values across independent traversals.
- Shared `REQ-FN-041` specifies no-throw ownership, filter-style predicate admission, conditional producer storage, consumer integration, and unavailable parameter equality. The approved and implemented `REQ-FN-002A4`/`REQ-FN-002A6` extension admits next/rest and dependent Cycle tails; has_next remains collection-only.
- The 2026-11-03 collection-only `first` decision was superseded on 2026-11-04 by the producer-first contract and explicit implementation approval described above.
- Requirements, vocabulary, architecture, dedicated specs, implementations, and behavioral tests are current. Old deferred sketches and superseded predicate placeholder tests were removed; the authoritative obligation snapshot is synchronized and strict traceability passes.
- Adapter specs now include tail admission, deferred output-skip/window semantics, and unchanged has_next rejection. Next/Rest/Cycle specs and executable traces include compact eager Cycle rotation, callback timing, nesting, empty boundaries, cap saturation, and unchanged first observation behavior.
- Runtime no-heap checks, caps 0/1/3 probes (zero cap uses isolated modular headers), final full 328-test suite, sanitizers, 100% affected-header line coverage, repository fast gate, and all 49 example compilations passed. Both new examples also executed. Follow-up fixed standard coverage capture to include only absolute-project-root src headers, avoiding GCC test-lambda metadata conflicts without ignoring errors or weakening the threshold. Full make coverage-cli now passes at 100% (996/996 lines), and narrowed predicate-header coverage passes.

## Next work after implementation

1. /gybis-spec-weed is the propagation handoff for checking spec-code alignment. Do not repeat completed operation-level implementation approval.
2. Producer-first work and its rendered HTML are committed. The new next/rest extension is not committed; source docs, cljonic.hpp, and the obligation snapshot are current. Its verification includes both header variants, ASan/UBSan, caps 0/1/3, runtime no-heap probes, the fast gate, 348 regression cases with 100% source line coverage, all 49 example compilations, and five executed affected examples. Rendered HTML for this extension remains for the human-owned final gate.
3. Select further API work only on human direction. Producer has_next and adapter parameter equality remain outside the approved tail extension.
4. `take`, `drop`, and responsive documentation are complete. Other `REQ-SEQ-022` operations remain unapproved until their individual contracts exist.

## Deferred until their contracts exist

Do not promote unresolved operation families, collection return/capacity policies, key/value constraints, or runtime-only behavior into implementation-ready status. Record each decision in its owning behavioral specification first, then propagate it downstream.

## Retired agenda material

The former keyword-catalog, initial map milestone, and broad collection design-question list described an earlier planning stage. They are retained only in git history, not as current work items.
