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
2. `take_while` and `drop_while` contracts were approved together; implementation was then explicitly approved individually, take_while first and drop_while second. Each operation is implemented and independently verified. Contract approval did not substitute for implementation approval; no staging or commit is authorized.

## Approved predicate-producer contracts (2026-11-03)

- `take_while(predicate, source)` yields the initial true prefix as an owning finite `TakeWhile` producer (`REQ-FN-039`). It examines but excludes the first false value and does not advance further.
- `drop_while(predicate, source)` skips only the initial true prefix as an owning finite `DropWhile` producer (`REQ-FN-040`), retains the first false value and remaining original window, and never retests the predicate after that first failure.
- Both own source/predicate copies without construction-time evaluation or a materialized cache. Original-window traversal is capped and endpoint-checked; skipping never replenishes the budget. All cljonic sources, including nested adapters and traversal-dependent Map/Set results, are admitted.
- `count` is exact by independent bounded traversal, not a cheap upper bound. It may repeat predicate/source callbacks before later consumption; `is_empty` remains `count == 0`. Fit/count predictions require equivalent values across independent traversals.
- Shared `REQ-FN-041` specifies no-throw ownership, filter-style predicate admission, conditional producer storage, consumer integration, and unavailable parameter equality. `next`/`rest`/`has_next` and Cycle tails dependent on these adapters remain unsupported pending a separately approved tail contract.
- Human subsequently approved a uniform `first` boundary: collections and MapEntry values only, never producers. Adapter first support was removed across contracts and downstream artifacts; the first implementation and diagnostic match the pre-session version. Materialize a producer before applying first.
- Requirements, vocabulary, architecture, dedicated specs, implementations, and behavioral tests are current. Old deferred sketches and superseded predicate placeholder tests were removed; the authoritative obligation snapshot is synchronized and strict traceability passes.
- Revised `take_while`: 45/45 obligations, 89 modular assertions in three cases. Revised `drop_while`: 48/48 obligations, 87 modular assertions in three cases. Combined adapter/first tests pass 227 assertions in seven cases per header variant. First diagnostic probes reject all seven producer families and accept materialized collections; unsupported adapter/dependent Cycle tails remain exercised as compile failures.
- Runtime no-heap checks, caps 0/1/3 probes (zero cap uses isolated modular headers), final full 328-test suite, sanitizers, 100% affected-header line coverage, repository fast gate, and all 49 example compilations passed. Both new examples also executed. Follow-up fixed standard coverage capture to include only absolute-project-root src headers, avoiding GCC test-lambda metadata conflicts without ignoring errors or weakening the threshold. Full make coverage-cli now passes at 100% (996/996 lines), and narrowed predicate-header coverage passes.

## Next work after implementation

1. Review the uncommitted pair; /gybis-spec-weed is the propagation handoff for checking spec-code alignment. Do not repeat completed operation-level implementation approval.
2. Source documentation, rendered HTML, and cljonic.hpp are current after the uniform first-boundary update. Full primitive diagnostics, all 328 CTest cases, sanitizers, full coverage, Allium checks/analysis, and docs regeneration passed; staging and commits still require authorization.
3. Select further API work only on human direction. `next`/`rest`/`has_next` support for predicate producers and dependent Cycle tails needs a separate approved tail contract.
4. `take`, `drop`, and responsive documentation are complete. Other `REQ-SEQ-022` operations remain unapproved until their individual contracts exist.

## Deferred until their contracts exist

Do not promote unresolved operation families, collection return/capacity policies, key/value constraints, or runtime-only behavior into implementation-ready status. Record each decision in its owning behavioral specification first, then propagate it downstream.

## Retired agenda material

The former keyword-catalog, initial map milestone, and broad collection design-question list described an earlier planning stage. They are retained only in git history, not as current work items.
