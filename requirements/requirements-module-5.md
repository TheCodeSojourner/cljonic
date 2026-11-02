# cljonic Requirements - Module 5: Higher-Order Algorithms & Traversal

## Purpose and Scope

This module records higher-order sequence transformation requirements and specifies the collection-shaping/traversal inventory. The free-function API `comp` is explicitly deferred; this does not defer the `cljonic::Map` collection type or its existing operations.

## Deferred Higher-Order Free Functions (Non-binding)

> **Deferred future work — non-binding.** The public free function `comp` is not part of the current supported API. Its requirement below records intended future behavior only; the function is not implementation-ready until its lifecycle is approved and its behavioral specification, tests, and implementation are propagated. This deferral does not apply to `cljonic::Map` or existing Map operations, and does not apply to the multi-source `map` free function, which is binding under `REQ-FN-032`.

### Function Composition (`comp`) & Multi-Source `map`

λ REQ-FN-032(x).
  ∀ `map`: accepts its transforming function as the first argument, followed by one or more compatible sources
  ∧ ∀ source: admitted exactly when it is a cljonic source (a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer) ∧ ∀ standard-library range ∨ container ∨ non-cljonic source: rejected at compile time
  ∧ ∀ `map(f, source)`: returns a bounded owning Vector whose element type is the function's result type over one source element ∧ whose logical elements are `f` applied to each source element in traversal order
  ∧ ∀ multi-source `map(f, source₁, …, sourceₙ)`: invokes `f` with one corresponding element from each source per step ∧ terminates when the first source is exhausted ∧ returns a bounded owning Vector of the successive `f` results in step order
  ∧ ∀ result capacity: a compile-time minimum over sources of each source's observable-count bound, where a collection contributes its declared capacity ∧ any producer contributes the configured observable traversal cap ∧ each contribution saturated at `CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE` ∧ the statically derived capacity failing that bound: fails at compile time
  ∧ ∀ runtime mapped element count: the minimum of the sources' effective counts, where a collection contributes its element count ∧ a finite producer contributes its count ∧ an unbounded producer contributes the configured observable traversal cap
  ∧ ∀ callback `f`: copy-constructible ∧ invoked without throwing with one argument per source (a unary function over one source, a binary function over two sources, and so on) ∧ returns a storable element result satisfying the no-throwing element contract ∧ ∀ callback that can throw ∨ whose result cannot be stored: rejected at compile time
  ∧ ∀ callback parameter position: the declared parameter type accepts the corresponding source element type only through a safe implicit conversion — an identical type ∨ a value-preserving arithmetic conversion per the implicit-conversion rules ∧ ∀ narrowing conversion (for example `long` to `int` ∨ `double` to `float`) ∧ ∀ crossing into ∨ out of the Boolean, character, enumeration, pointer, or user-defined domains: rejected at compile time
  ∧ ∀ callback whose parameter types are not statically inspectable (a generic lambda ∨ a templated `operator()`): the safe-conversion rule cannot be evaluated ∧ such a callback is admitted only when the per-position argument correspondence is nothrow-invocable, and the inspectable-parameter rule applies whenever the parameter types are statically known
  ∧ ∀ Map ∨ Set source: traversal order is semantically unordered ∧ the mapped result order carries no semantic significance
  ∧ ∀ `map` operation: constexpr ∧ non-throwing ∧ non-allocating ∧ non-mutating ∧ preserves every source value ∧ compile-time domain gating with targeted rejection diagnostics naming `map` and its supported domains
  rationale: the human approved activating the variadic multi-source `map` free function on 2026-10-31 with a bounded owning Vector result, min-source capacity derivation saturated at the collection maximum, nothrow-invocable callbacks, and the cljonic-source domain, mirroring Clojure's termination and multi-source arity shape within the materialization-first model; the human further approved on 2026-10-31 that callback parameter conversion follows the library's safe implicit-conversion policy (narrowing and domain-crossing conversions rejected) rather than unrestricted overload resolution, with uninspectable callables falling back to nothrow invocability
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-10-31, rationale_source: origin_artifact}

λ REQ-FN-033(x).
  ∀ `comp`: accepts zero or more compatible element functions ∧ composes them into one callable value
  ∧ ∀ zero functions: produce an identity callable ∧ ∀ one function: produces an equivalent callable
  ∧ ∀ functions supplied as `comp(f, g, h, ...)`: compile-time checking verifies from right to left that each function accepts the return type of the function to its right ∧ the composed function has the rightmost function's input type and the leftmost function's return type
  ∧ ∀ resulting_callable: applies the functions from right to left, representing `f(g(h(value)))` for three functions
  ∧ ∀ incompatible parameter ∧ return types: rejected at compile time ∧ ∀ context: ¬∃ transducer composition ∨ destination-independent collection-processing semantics
  rationale: transducers and destination-independent processing sit outside the bounded, materialization-first model of the library
  {source: stakeholder_decided, decided_by: original_spec_author, rationale_source: origin_artifact}

λ REQ-FN-034(x).
  ∀ `reduce`: accepts a reducing function followed by one cljonic source (the no-initial form) or by an initial value and one cljonic source (the explicit-initial form)
  ∧ ∀ source: admitted exactly when it is a cljonic source (a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer) ∧ ∀ standard-library range ∨ container ∨ non-cljonic source: rejected at compile time
  ∧ ∀ accumulator type: the reducing function's first declared parameter type when its parameter types are statically inspectable ∧ the initial value's type when they are not
  ∧ ∀ reducing function: copy-constructible ∧ invoked without throwing as `(accumulator, element)` with the accumulator first and the current source element second ∧ its result type is exactly the accumulator type
  ∧ ∀ accumulator: satisfies the nothrow element storage contract
  ∧ ∀ no-initial form `reduce(f, source)`: requires a statically inspectable function signature ∧ ∀ function whose parameter types cannot be inspected (a generic lambda ∨ a templated `operator()`): rejected at compile time in this form ∧ admitted only in the explicit-initial form
  ∧ ∀ no-initial form with at least one observed element: the first element in traversal order converts safely to and seeds the accumulator, and the remaining elements are folded left to right
  ∧ ∀ no-initial form over an empty source: the result is the accumulator type's default value
  ∧ ∀ explicit-initial form `reduce(f, initial, source)`: the initial value seeds the accumulator ∧ the initial value converts safely to the accumulator type ∧ each source element is passed to the callback's second parameter under the safe-conversion rule, without requiring the source element type to convert to the accumulator type ∧ ∀ empty source: the observed elements are folded left to right over the initial value
  ∧ ∀ initial value: converts safely to the accumulator type ∧ ∀ unsafe initial conversion: rejected at compile time
  ∧ ∀ consumed argument position: the declared parameter type accepts its argument through a safe implicit conversion ∧ ∀ narrowing conversion (for example `long` to `int`) ∧ ∀ crossing into ∨ out of the Boolean, character, enumeration, pointer, or user-defined domains: rejected at compile time
  ∧ ∀ function whose parameters are not statically inspectable: the per-position safe-conversion rule cannot be evaluated ∧ such a function is admitted in the explicit-initial form only when its accumulator/element correspondence is nothrow-invocable, and the inspectable-parameter rule applies whenever the parameter types are statically known
  ∧ ∀ runtime element count: `count(source)` saturated at `CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE` ∧ an unbounded producer is therefore folded over its configured observable traversal cap and `reduce` terminates
  ∧ ∀ Map ∨ Set source: traversal order is semantically unordered ∧ the folded result carries no order-dependent guarantee beyond the reducing function's own associativity and commutativity
  ∧ ∀ `reduce` operation: constexpr ∧ non-throwing ∧ non-allocating ∧ non-mutating ∧ preserves every source value ∧ compile-time domain gating with targeted rejection diagnostics naming `reduce` and its supported domains
  rationale: the human approved activating the `reduce` free function on 2026-11-02 in Clojure's two-arity shape (no initial value, and explicit initial value) over the cljonic source domain, with the accumulator type derived from the reducing function's first parameter (or the initial value when the function's parameters cannot be inspected), a left-to-right fold that always traverses the saturated observable count, the accumulator type's default value for an empty no-initial source, inspectable signatures required wherever no initial value can supply the accumulator type, and the library's safe implicit-conversion policy for every consumed argument position
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-02, rationale_source: origin_artifact}

## Collection-Shaping and Traversal Family

λ REQ-SEQ-022(x).
  ∀ context: the first-pass collection-shaping and traversal family includes bounded operations corresponding to `take`, `drop`, `take_while`, `drop_while`, `take_last`, `drop_last`, `take_nth`, `nth`, `nthnext`, `nthrest`, `butlast`, `map_indexed`, `rseq`, `second`, `ffirst`, `fnext`, `nfirst`, `nnext`, `some`, `is_every`, `not_any`, `not_every`, `distinct`, `dedupe`, `frequencies`, `reductions`, `split_at`, `split_with`, `mapcat`, `interleave`, `interpose`, `partition`, `partition_all`, `partition_by`, `partitionv`, `partitionv_all`, `group_by`, `flatten`, `tree_seq`, `keep`, `keep_indexed`, `remove`, `replace`, `mapv`, `filterv`, `subvec`, `find`, `reduce`, `reduce_kv`, `sort`, and `sort_by`
  ∧ ∀ listed_operation: an individually named downstream behavioral specification exists before it is treated as implementation-ready
  ∧ ∀ specification: defines the operation's public arity and argument roles, callback contract where applicable, termination rule, equality and ordering capabilities, nested-result representation, producer behavior, result capacity derivation, typed-absence behavior, failure/preflight behavior, and complete-versus-bounded-result classification
  ∧ ∀ operation_level_contract: ¬∃ inference only from this inventory
  ∧ policies:
    1. ∀ operation: preserves input values ∧ ¬∃ introduction of side effects or hidden mutable state independently of pure callbacks
    2. ∀ operation ∈ {`distinct`, `dedupe`, `frequencies`, `sort`, `sort_by`} and similar: requires the applicable `StableEqualityComparable` or `TotallyOrdered` capability for their elements/keys
    3. ∀ operation: produces bounded owning results when a useful finite capacity can be derived, or explicit producers when results are unbounded
    4. ∀ traversal over maps ∧ sets: treats iteration order as semantically unordered
    5. ∀ (transducer-only arities ∧ hidden lazy-sequence machinery): unsupported
  {source: stakeholder_decided, decided_by: original_spec_author}

## Traceability and Related Requirements

- **Downstream Artifact**: Free-function templates for sequence shaping, slicing, filtering, mapping, grouping, and ordering.
- **Governed REQs**: `REQ-SEQ-022`, `REQ-FN-032` (binding), `REQ-FN-033` (deferred), `REQ-FN-034` (binding).