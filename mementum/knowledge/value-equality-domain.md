---
type: Reference
title: Value Equality Domain
status: active
description: The unified stable-equality architecture across collections, composite values, and producers; admission contracts and the sequence-equality prohibition.
tags: [equality, producers, collections, composite-values, concepts, requirements]
related:
  - /mementum/knowledge/cpp-concept-layer.md
  - /mementum/knowledge/collection-api-surface-discipline.md
  - /mementum/memories/producer-parameter-equality.md
  - /mementum/memories/nothrow-stable-equality-comparable-concept.md
  - /mementum/memories/rejection-diagnostic-fallback.md
  - /mementum/memories/cljonic-design-notes.md
depends-on: [requirements/requirements-module-2.md, requirements/requirements-module-3.md, requirements/requirements-module-4.md]
---

# Value Equality Domain

This synthesis consolidates the durable equality architecture completed across
Streams A, B, and C (composite values, collection equality, producer storage,
producer parameter equality). It is written for future AI sessions: read this
before re-deriving how equality works in cljonic.

## The one equality engine

`StableEqualityComparable<T>` is the single recursive admission gate
(REQ-CAP-010, REQ-NUM-007):

```
std::equality_comparable<T>
&& concepts_detail::nothrow_equality_v<T>
&& !concepts_detail::contains_floating_point_v<T>
&& !concepts_detail::contains_callable_v<T>
&& concepts_detail::walker_coverage_ok_v<T>
```

- Raw floating-point types and detectable floating-point-bearing composites
  are rejected for equality (REQ-NUM-001); storability never implies equality.
- Detection policy (REQ-NUM-019, human 2026-10-31): every floating-point
  equality comparison that can be detected statically is rejected at compile
  time, and no detectable comparison may be silently admitted. Concretely this
  covers raw floating-point operands of `equal`/`not_equal`, a `Set` element
  type, a `Map` key type, and any floating-point-bearing component the walkers
  can see. Storage that is never compared stays permitted (`Vector<double, N>`
  elements, `Map<int, double, N>` values), while equality over such collections
  stays rejected. Detection stops at an opaque user-defined class, whether
  aggregate or non-aggregate (REQ-NUM-018), and that limit is stated rather than
  papered over.
- The detection obligation is enforced nominally, not by convention
  (`REQ-NUM-019`, `StableEqualityComparable.NominalWalkerCoverageIsGated`). The
  walkers are enumerative, so the unspecialized form answers "no floating point"
  for anything it does not know. Each library-owned composite therefore declares
  `concepts_detail::walker_coverage<Self>` in its own header. The primary
  template denies coverage, so a nominal composite that forgets the declaration
  is not `StableEqualityComparable` — the omission fails at first equality use
  instead of silently admitting a detectable floating-point comparison.
  User-defined classes outside the library's nominal composite domain are
  opaque: their members are unanalyzable, and a non-throwing `operator==` is
  trusted under REQ-NUM-018.
  Adding a library-owned composite now requires: nominal trait, the three walker
  specializations, and the coverage declaration.
- Callables are rejected as a class, including function pointers (a function
  pointer has `operator==` but admits only address-as-key semantics).
- The walkers (`contains_floating_point` / `contains_callable` /
  `contains_standard_range`) are specialized per header and recurse through
  `cljonic::Variant` alternatives, cljonic collections, `MapEntry`, and producer
  stored parameters. (`std::variant` specializations were removed 2026-10-02.)
- `NothrowEqualityComparable = StableEqualityComparable &&
  concepts_detail::nothrow_equality_v<T>` names the non-throwing comparison
  requirement for operations such as `equal` / `not_equal`. The term is
  currently logically redundant because `StableEqualityComparable` itself
  requires it.
- `NothrowStableEqualityComparable = StableEqualityComparable &&
  NothrowCollectionElement && concepts_detail::nothrow_equality_v<T>` is the
  map-key / set-element contract. The non-throwing-comparison term was added
  2026-10-31 (human decision) and is the wrapper-smuggling defence: with only
  nothrow *storage*, `hash_set(std::pair<double,double>{...})` and
  `hash_set(std::optional<double>{...})` compiled, smuggling a floating-point
  equality past the walkers. Every parameterized standard wrapper now fails
  admission for every instantiation because the standard library declares no
  non-throwing comparison on them — no exclusion list required, and user classes
  with `noexcept operator==` still pass (REQ-NUM-018).
- Heap-freedom is not queryable in C++23; the nothrow contract is its
  enforceable proxy (REQ-PLAT-009 forbids indirect allocation too). See
  `nothrow-contract-is-the-heap-freedom-proxy.md`.

## Fallthrough domain and rejection diagnostics

The non-cljonic `equal` operand fallthrough is the closed value domain:
arithmetic scalars, scoped enumerations, user-defined class types (aggregate
structs and non-aggregate classes alike) with a non-throwing `==`, and
`cljonic::Variant` composites. A non-aggregate class is admitted on
`NothrowEqualityComparable` — the stable-equality capability plus the non-throwing
comparison `equal` itself requires — which is the equality half of the storage
contract (storage additionally needs nothrow copy/assign, which `equal` does not
because it never copies an operand, so a throwing-copy type with `noexcept ==`
is comparable but not storable).

Safe implicit conversions (human decision 2026-10-31, REQ-FN-002G) relax the
identity rule for **scalar operands and sequential element positions**: a pair
whose types are related by a value-preserving conversion in either direction
compares in the wider type (`equal(1, 1L)` is true). Sign crossings, character,
bool, enumeration, pointer, and distinct-user-defined pairs stay rejected, and
`equal` remains outside the C++ interop surface. Unscoped enums, pointers,
standard range/container types, and `std::variant` are rejected at any depth
(2026-10-02). Out-of-domain operands produce one targeted `RejectionDiagnostic`
(REQ-DIAG-009) instead of a list of rejected concept candidates; domain support
is detected through the admission concepts, never `requires { call(...) }`.

## Three equality surfaces, one rule shape

| Surface             | operator== semantics                                 | Named operation       | Constraint                                |
| ------------------- | ---------------------------------------------------- | --------------------- | ----------------------------------------- |
| Vector/String/Queue | order-sensitive (count + logical order)              | `equal` (REQ-FN-002G) | stable element equality                   |
| Map/Set             | order-insensitive (count + matching entries)         | `equal` (REQ-FN-002G) | stable key/value equality                 |
| Range/Repeat/Cycle  | producer parameter equality (stored parameters only) | none (native `==` only) | stable parameter equality                 |
| Iterate/Repeatedly  | none                                                 | none                  | callable step → excluded at type boundary |

Producer parameter equality (REQ-FN-014B) is normative for every producer whose
stored parameters all admit stable equality: provide only the native `==`, with
producer parameter equality semantics, O(1), constexpr, noexcept, non-allocating,
never traversing the produced sequence. There is no separately named comparison
free function (`parameters_equal` was removed 2026-10-16). Distinct parameters
compare unequal even when produced sequences coincide
(`Range{0,5,0} != Range{0,7,0}`; `Repeat{7} != Repeat{7,0U}`).

## The sequence-equality prohibition (REQ-SEQ-016 / REQ-FN-014A)

Statically-unbounded-capable values MUST NOT provide sequence equality —
equality that compares a materialized or observed sequence fails at compile
time. The prohibition covers produced sequences only; parameter-structural
producer equality is explicitly exempt because it is O(1) and cannot hang.

## Composite values (cljonic::Variant)

`cljonic::Variant` is the nominal cljonic composite (REQ-CAP-011); `std::variant`
is retired from the cljonic value domain — it is rejected as an `equal` operand
and as a Map key / Set element, and is permitted only as the internal backend of
`cljonic::Variant`. Storage admission requires every alternative to satisfy
`NothrowCollectionElement`; alternative-strict equality (`==` and `equal`) is
provided only when every alternative satisfies `ComparableVariantAlternative`
(`NothrowStableEqualityComparable && NothrowEqualityComparable`), so a
float-bearing `Variant` is storable but not comparable. Same alternative AND
that alternative's values compare equal. No cross-type numeric unification in
alternative identity (`Variant<int,long>{1}` is not `Variant<int,long>{1L}` —
still FINAL; the 2026-10-31 safe-conversion rule for scalar operands and
sequential elements does not reach variant alternatives, Map keys/values, Set
elements, or String content), no hash-based equality.
`Variant` has no valueless state.

**This decision is final (human decision, 2026-09-25):** alternative-strict is
the only supported composite equality semantics. Cross-type numeric unification
is permanently out of scope — there is no deferred "Stream D" and no reopen
path; changing this would require a fresh spec-tend with full downstream
propagation.

## Known limitations (do not rediscover these)

1. Opaque user-defined classes are a deliberate boundary, not a defect (human
   decision, `REQ-NUM-018`). An aggregate or non-aggregate class providing a
   non-throwing `operator==` — defaulted or user-written — is admitted to the
   stable equality domain even when it stores floats. The recursion of
   `REQ-NUM-007` stops at the class boundary; cljonic does not inspect or verify
   its members or equality semantics, which remain the author's responsibility.
   C++23 has no standard reflection for analyzing arbitrary user-defined class
   members. A class without an available non-throwing `operator==` remains
   rejected for equality, though it may still be storable when it satisfies the
   independent storage contract.
2. Set duplicate insertion at compile time is rejected by a declared-never-
   defined trap (`rejected_duplicate_value_at_compile_time()`), so the diagnostic
   reads as a non-constexpr call rather than `'abort()' is not a constant
   expression` (fixed in 70674bf). Runtime pack construction deduplicates
   (`Set{1, 2, 2, 3}`, `Set<int, 4>{1, 1}` under REQ-COLL-019); use distinct
   elements in constexpr contexts, because a constant-evaluated duplicate is a
   deliberate compile-time error.
3. Single range-typed constructor argument selects the source constructor, not
   the pack constructor; tests use multi-element packs (pending design
   decision, see state.md carry-forward).
4. `equal_by` / `identical` and the remaining REQ-FN-002C comparison family
   (`less`, `less_equal`, `greater`, `greater_equal`) remain deferred. `equal`
   is implemented (REQ-FN-002G) with the three Clojure `=` arities (2026-09-26,
   commit 93231aa): unary admits a single domain-admitted operand and returns
   true, the variadic form conjoins adjacent-pair equality left to right with
   short-circuit, and every adjacent pair is individually compile-time gated by
   the named family concepts and `EqualPairAdmissible` (SFINAE-safe via
   void-fallback detection traits). Cross-family and
   mixed cljonic-to-non-cljonic pairs fail at compile time in every arity;
   termination (bounded-prefix, configured traversal cap) is uniform across
   arities. `not_equal` is implemented (REQ-FN-002H, 2026-09-29) as the thin
   negation of `equal`: same three arities, same compile-time gating (reusing
   `EqualPairAdmissible` and `all_adjacent_pairs_admissible_v`), unary
   returns false for an admitted operand, the variadic form negates the
   adjacent-pair conjunction, and it is distinct from the native `operator!=`.
