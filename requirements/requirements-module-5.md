# cljonic Requirements - Module 5: Higher-Order Algorithms & Traversal

## Purpose and Scope

This module records higher-order sequence transformation requirements and specifies the collection-shaping/traversal inventory. The free-function API `comp` is explicitly deferred; this does not defer the `cljonic::Map` collection type or its existing operations. The `map`, `reduce`, `filter`, `filterv`, `take`, and `drop` free functions are binding under `REQ-FN-032`, `REQ-FN-034`, `REQ-FN-035`, `REQ-FN-036`, `REQ-FN-037`, and `REQ-FN-038`; `filterv` is an alias of `filter`.

The `take_while` and `drop_while` producers are active under binding `REQ-FN-039` and `REQ-FN-040`, with shared integration under `REQ-FN-041`. Each implementation was explicitly approved and independently verified; paired contract approval did not substitute for operation-level approval or verification.

## Deferred Higher-Order Free Functions (Non-binding)

> **Deferred future work — non-binding.** The public free function `comp` is not part of the current supported API. Its requirement below records intended future behavior only; the function is not implementation-ready until its lifecycle is approved and its behavioral specification, tests, and implementation are propagated. This deferral does not apply to `cljonic::Map` or existing Map operations, or to the multi-source `map`, `filter`, and `filterv` free functions, which are binding under `REQ-FN-032`, `REQ-FN-035`, and `REQ-FN-036`.

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

λ REQ-FN-035(x).
  ∀ `filter`: accepts a predicate followed by exactly one cljonic source
  ∧ ∀ source: admitted exactly when it is a cljonic source (a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer) ∧ ∀ standard-library range ∨ container ∨ non-cljonic source: rejected at compile time
  ∧ ∀ result: bounded owning Vector whose element type is the source element type and whose capacity is the source's observable-count bound (collection declared capacity ∨ producer configured observation cap), saturated at `CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE`
  ∧ ∀ runtime traversal: visits the source's observed elements in traversal order, evaluates the predicate once per visited element, and appends each matching element ∧ the result capacity equals the source's observable-count bound, so every observed element fits ∧ traversal therefore ends when the source is exhausted ∧ ¬∃ separate result-full termination condition
  ∧ ∀ zero-capacity result: returns empty without invoking the predicate
  ∧ ∀ source element type: satisfies the nothrow element storage contract
  ∧ ∀ predicate: should be a pure function with no side effects ∧ copy-constructible ∧ invoked without throwing on a const source element ∧ returns exactly `bool`
  ∧ ∀ predicate parameter type that is statically inspectable: accepts the source element only through a safe implicit conversion (an identical type ∨ a value-preserving arithmetic conversion) ∧ ∀ narrowing conversion ∨ crossing into or out of the Boolean, character, enumeration, pointer, or user-defined domains: rejected at compile time
  ∧ ∀ predicate whose parameter types are not statically inspectable (a generic lambda ∨ templated `operator()`): admitted only when nothrow-invocable with a const source element and its result type is exactly `bool`
  ∧ ∀ Map ∨ Set source: traversal order is semantically unordered ∧ result order has no semantic significance
  ∧ ∀ `filter` operation: constexpr ∧ non-throwing ∧ non-allocating ∧ non-mutating ∧ preserves every source value ∧ compile-time domain gating with targeted rejection diagnostics naming `filter` and its supported domains
  rationale: the human approved activating `filter` on 2026-11-02 for all cljonic collections and producers, returning a bounded owning Vector with source-derived capacity, an exact-bool nothrow predicate following map's safe parameter-conversion rule, and traversal through source exhaustion; the result capacity equals the source's observable-count bound, so every observed element fits
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-02, rationale_source: origin_artifact}

λ REQ-FN-036(x).
  ∀ `filterv(predicate, source)`: a public alias of `filter(predicate, source)` ∧ accepts exactly the same predicate and one cljonic source under `REQ-FN-035`
  ∧ ∀ admitted call: returns exactly the same bounded owning Vector type, capacity, and logical values as `filter(predicate, source)`
  ∧ ∀ behavior: predicate evaluation, traversal, ordering, empty-source behavior, source preservation, constexpr, non-throwing, non-allocating, and non-mutating guarantees are identical to `filter` under `REQ-FN-035`
  ∧ ∀ rejected call: the same invalid domains and predicate forms as `filter` are rejected at compile time with a targeted diagnostic naming `filterv` and its supported domains
  rationale: the human approved `filterv` as a simple alias of `filter` on 2026-11-03; in cljonic, `filter` already returns an owning Vector, so both names expose the same operation and result contract
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-03, rationale_source: origin_artifact}

## Collection-Shaping and Traversal Family

λ REQ-FN-037(x).
  ∀ `take(count, source)`: accepts a count followed by exactly one cljonic source (a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer)
  ∧ ∀ count: signed or unsigned integer type other than Boolean or character types, whose nonnegative type range fits `std::size_t` ∧ nonpositive counts normalize to zero before unsigned conversion ∧ positive counts are preserved without wrapping or truncation, as for counted Repeat
  ∧ ∀ result: bounded owning Vector with the source element type and compile-time capacity equal to the source's observable-count bound (declared collection capacity ∨ producer configured observation cap), saturated at `CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE` ∧ the runtime count does not change the result type or capacity
  ∧ ∀ result elements: the first `min(normalized count, observable source traversal length, result capacity)` elements in source traversal order ∧ oversized counts return all observable elements without failure ∧ source count is an upper bound and does not replace checking traversal exhaustion
  ∧ ∀ traversal: terminates at the requested prefix or observable source exhaustion ∧ does not advance to an additional produced value beyond the requested prefix ∧ nonpositive count or zero capacity returns an empty typed Vector without beginning traversal ∧ empty source returns an empty typed Vector without invoking producer callbacks, including Cycle over an empty source despite its nonzero count bound
  ∧ ∀ producer: callback timing follows its existing traversal contract ∧ Iterate advances only between returned elements ∧ Repeatedly invokes its callback exactly once per returned element ∧ callback state stored by value is preserved in the source, while referenced or static state remains shared
  ∧ ∀ Map ∨ Set source: traversal order is semantically unordered ∧ Queue preserves FIFO order ∧ String yields characters ∧ Map yields value-semantic MapEntry elements
  ∧ ∀ source element: satisfies the nothrow element storage contract ∧ no additional equality or ordering capability is required
  ∧ ∀ result: independently owning bounded prefix, with no nested result, absence sentinel, status wrapper, preflight predicate, or transducer-only arity
  ∧ ∀ MapEntry input ∨ external C++ range ∨ non-cljonic source ∨ invalid count type ∨ wrong arity: rejected at compile time with a targeted diagnostic naming `take` and its supported count/source domains
  ∧ ∀ operation: constexpr ∧ non-throwing ∧ non-allocating ∧ non-mutating ∧ preserves the source value ∧ no RTTI
  rationale: the human approved activating `take(count, source)` on 2026-11-03 for all cljonic collections and producers, returning an owning Vector with source-bound capacity, reusing Repeat count admission and nonpositive normalization, and bounding oversized requests by the observable source count
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-03, rationale_source: origin_artifact}

λ REQ-FN-038(x).
  ∀ `drop(count, source)`: accepts a count followed by exactly one cljonic source (a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer)
  ∧ ∀ count: signed or unsigned integer type other than Boolean or character types, whose nonnegative type range fits `std::size_t` ∧ nonpositive counts normalize to zero before unsigned conversion ∧ positive counts are preserved without wrapping or truncation, as for counted Repeat
  ∧ ∀ result: bounded owning Vector with the source element type and compile-time capacity equal to the source's declared collection capacity or producer configured observation cap, saturated at `CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE` ∧ runtime count does not change the result type or capacity
  ∧ ∀ observation window: the original source traversal bounded by `min(source.count(), result capacity)` and actual iterator exhaustion ∧ dropping consumes this window rather than beginning a fresh producer observation window
  ∧ ∀ result elements: the suffix remaining after removing the first normalized count elements from that original observation window, in source traversal order ∧ result count is `max(original observable traversal length - normalized count, 0)`, computed without unsigned underflow ∧ nonpositive counts return all elements of the original window
  ∧ ∀ normalized count at least the observation-window count bound ∨ zero capacity: return an empty typed Vector without beginning traversal or invoking producer callbacks ∧ actual empty traversal, including Cycle over an empty source despite its nonzero count bound: returns empty without callbacks
  ∧ ∀ nonempty-result traversal: visits the skipped prefix and retained suffix in one traversal of the original window ∧ never advances beyond the final required value or actual endpoint ∧ source count is an upper bound and does not replace checking traversal exhaustion
  ∧ ∀ producer: skipped and retained values follow existing iterator callback timing ∧ Repeatedly invokes its callback once per visited value, including skipped values ∧ Iterate invokes its step only between visited values ∧ callback state stored by value is preserved in the source, while referenced or static state remains shared
  ∧ ∀ Map ∨ Set source: traversal order is semantically unordered ∧ Queue preserves FIFO order ∧ String yields characters ∧ Map yields value-semantic MapEntry elements
  ∧ ∀ source element: satisfies the nothrow element storage contract ∧ no additional equality or ordering capability is required
  ∧ ∀ result: independently owning suffix of the original bounded observation window, complete within that window but not a fresh observation of an unbounded producer ∧ no nested result, absence sentinel, status wrapper, preflight predicate, or transducer-only arity
  ∧ ∀ MapEntry input ∨ external C++ range ∨ non-cljonic source ∨ invalid count type ∨ wrong arity: rejected at compile time with a targeted diagnostic naming `drop` and its supported count/source domains
  ∧ ∀ operation: constexpr ∧ non-throwing ∧ non-allocating ∧ non-mutating ∧ preserves the source value ∧ no RTTI
  rationale: the human approved the `drop(count, source)` contract on 2026-11-03 with a take-style owning Vector and runtime-independent source-bound capacity, original-window producer observation, and no traversal when the requested drop covers the observation-window count bound; this approved contract precedes public API publication, tests, and implementation
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-03, rationale_source: origin_artifact}

λ REQ-FN-039(x).
  ∀ `take_while(predicate, source)`: accepts exactly one predicate followed by one cljonic source ∧ returns an explicit owning `TakeWhile` producer with the source's element type, rather than a materialized collection
  ∧ ∀ source: a cljonic collection or producer admitted under `REQ-FN-041`, including nested `TakeWhile` and `DropWhile` sources ∧ external C++ ranges, containers, arrays, and a `MapEntry` value used as the source: rejected at compile time
  ∧ ∀ construction: owns independent copies of source and predicate ∧ invokes neither the predicate nor source producer callbacks ∧ ¬∃ borrowed source lifetime, materialized result cache, result-capacity template parameter, or transducer-only arity
  ∧ ∀ observation window: the original source traversal limited to at most `CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE` source elements and actual iterator exhaustion ∧ respects any smaller source traversal window ∧ determining a traversal budget does not require a preliminary exact-count traversal
  ∧ ∀ traversal: yields only the initial consecutive elements for which the predicate returns `true` ∧ evaluates the predicate exactly once per examined element ∧ evaluates but does not yield the first false element ∧ stops at that failure, source exhaustion, or the observation bound
  ∧ ∀ first false element: its production and predicate evaluation MAY occur to establish the boundary ∧ no subsequent source value is produced or examined ∧ no advance beyond the final source element of the observation window
  ∧ ∀ consumer that stops early: no further source advancement or predicate invocation occurs independently of an explicit subsequent observation
  ∧ ∀ empty source traversal ∨ zero observation cap: yields no elements and invokes neither predicate nor source producer callbacks
  ∧ ∀ result: `is_finite()` returns `true` for the explicitly bounded adapter result, including when the underlying source is unbounded ∧ ¬∃ claim of observing the underlying unbounded source completely
  ∧ ∀ `count(result)`: performs a fresh bounded traversal and returns the exact number of yielded elements ∧ invokes the predicate and source callbacks according to that traversal ∧ preserves the stored source and predicate ∧ shares no cache with subsequent traversal
  ∧ ∀ predicate, ownership, storage, ordering, consumer admission, and rejected calls: follows `REQ-FN-041`
  ∧ ∀ operation: constexpr ∧ non-throwing ∧ non-allocating ∧ non-mutating ∧ no RTTI ∧ preserves input values under their existing authorized external-effect contracts
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-03, compound_by_design: true}

λ REQ-FN-040(x).
  ∀ `drop_while(predicate, source)`: accepts exactly one predicate followed by one cljonic source ∧ returns an explicit owning `DropWhile` producer with the source's element type, rather than a materialized collection
  ∧ ∀ source: a cljonic collection or producer admitted under `REQ-FN-041`, including nested `TakeWhile` and `DropWhile` sources ∧ external C++ ranges, containers, arrays, and a `MapEntry` value used as the source: rejected at compile time
  ∧ ∀ construction: owns independent copies of source and predicate ∧ invokes neither the predicate nor source producer callbacks ∧ ¬∃ borrowed source lifetime, materialized result cache, result-capacity template parameter, or transducer-only arity
  ∧ ∀ observation window: the original source traversal limited to at most `CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE` source elements and actual iterator exhaustion ∧ respects any smaller source traversal window ∧ determining a traversal budget does not require a preliminary exact-count traversal
  ∧ ∀ traversal: skips only the initial consecutive elements for which the predicate returns `true` ∧ evaluates the predicate exactly once per examined prefix element, including the first false element ∧ retains the first false element and all subsequent elements of the original observation window
  ∧ ∀ traversal after the first predicate failure: invokes no further predicate calls ∧ subsequent matching elements remain in the result ∧ obtaining the first yielded value MAY require examining the entire matching prefix
  ∧ ∀ skipped elements: consume the original source observation budget ∧ ¬∃ replenishment of that budget or restart of the source after the skipped prefix ∧ no advance beyond the final source element of the observation window
  ∧ ∀ consumer that stops early: no further source advancement or predicate invocation occurs independently of an explicit subsequent observation
  ∧ ∀ all-true observation window ∨ empty source traversal ∨ zero observation cap: yields no elements ∧ empty traversal and zero cap invoke neither predicate nor source producer callbacks
  ∧ ∀ result: `is_finite()` returns `true` for the explicitly bounded adapter result, including when the underlying source is unbounded ∧ ¬∃ claim of observing the underlying unbounded source completely
  ∧ ∀ `count(result)`: performs a fresh bounded traversal and returns the exact number of yielded elements ∧ invokes the predicate only through the first false element and source callbacks according to that traversal ∧ preserves the stored source and predicate ∧ shares no cache with subsequent traversal
  ∧ ∀ predicate, ownership, storage, ordering, consumer admission, and rejected calls: follows `REQ-FN-041`
  ∧ ∀ operation: constexpr ∧ non-throwing ∧ non-allocating ∧ non-mutating ∧ no RTTI ∧ preserves input values under their existing authorized external-effect contracts
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-03, compound_by_design: true}

λ REQ-FN-041(x).
  ∀ `TakeWhile` ∨ `DropWhile`: admitted to the closed cljonic producer/source domain ∧ extends the source admission of `map`, `reduce`, `filter`, `filterv`, `take`, `drop`, `cycle`, `seq`, `into`, and `fits_into`, and the sequenceable domain of `count` and `is_empty`, without changing previously supported types' behavior
  ∧ ∀ nested adapters: admitted as sources without requiring their producer type to be default-constructible or copy-assignable merely for traversal ∧ source element types still satisfy `NothrowCollectionElement`
  ∧ ∀ owned source and predicate: copy-constructible without throwing ∧ captures that prevent predicate default construction or assignment do not by themselves reject a factory call
  ∧ ∀ predicate: follows the `filter` admission contract in `REQ-FN-035` (should be pure, must not allocate, nothrow-invocable on const source elements, exact `bool` result, safe inspectable parameter conversions, and generic-callable nothrow invocation fallback)
  ∧ ∀ traversal, including exact counting: uses fresh predicate state copied from the stored predicate and the source's existing traversal-copy semantics ∧ no cross-traversal cache ∧ callback state owned by value remains unchanged in the producer ∧ effects through references or static storage remain shared only as permitted by the underlying callback/source contract
  ∧ ∀ exact-count query followed by traversal or preflight followed by materialization: evaluations MAY repeat ∧ count/fit predictions apply when the independent traversals yield the same values ∧ ¬∃ promise of agreement when authorized external callback state changes those values
  ∧ ∀ producer default construction, assignment, or storage as a collection element: conditional on the corresponding existing nothrow storage capabilities ∧ ¬∃ relaxation of `NothrowCollectionElement`
  ∧ ∀ Map source: yields `MapEntry` values ∧ String yields characters ∧ Queue preserves FIFO order ∧ Map and Set prefix/suffix membership and result order depend on semantically unordered traversal
  ∧ ∀ producer creation and traversal: no additional element equality or ordering capability ∧ producer parameter equality and Map-key/Set-element admission remain unavailable because an owned component is callable
  ∧ ∀ `equal` ∨ `not_equal`: admits adapter observations in the sequential equality family subject to the existing element-comparison capability and safe-conversion rules, independently of producer parameter equality
  ∧ ∀ `is_empty`: retains `count(value) == 0` semantics ∧ `fits_into` retains finite-source exact-count preflight for Vector, Queue, and String and insertion-sensitive preflight for Map and Set ∧ these observations MAY now invoke the adapter predicate and source callbacks without changing existing types' contracts
  ∧ ∀ `next`, `rest`, and `has_next` applied to these adapters or a Cycle whose tail would require these adapters' tail support: unsupported in this slice ∧ rejected with targeted compile-time diagnostics until a separate tail-navigation contract is approved
  ∧ ∀ `first`: admits these adapters and Cycles over them under `REQ-FN-002A1`, with only-needed first-element evaluation, original-window bounds, default empty results, and existing callback-state rules ∧ producer-first support is active after separate explicit implementation approval and verification
  ∧ ∀ invalid source, predicate, or arity: rejected at compile time with a targeted diagnostic naming the operation and its supported source/predicate domain
  ∧ ∀ lifecycle: contracts are binding but implementation approval and verification remain explicit and separate for `take_while` and `drop_while` ∧ no publication or implementation claim follows from contract approval
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-04, compound_by_design: true}

λ REQ-SEQ-022(x).
  ∀ context: the first-pass collection-shaping and traversal family includes bounded operations corresponding to `take`, `drop`, `take_while`, `drop_while`, `take_last`, `drop_last`, `take_nth`, `nth`, `nthnext`, `nthrest`, `butlast`, `map_indexed`, `rseq`, `second`, `ffirst`, `fnext`, `nfirst`, `nnext`, `some`, `is_every`, `not_any`, `not_every`, `distinct`, `dedupe`, `frequencies`, `reductions`, `split_at`, `split_with`, `mapcat`, `interleave`, `interpose`, `partition`, `partition_all`, `partition_by`, `partitionv`, `partitionv_all`, `group_by`, `flatten`, `tree_seq`, `keep`, `keep_indexed`, `remove`, `replace`, `mapv`, `filter`, `filterv`, `subvec`, `find`, `reduce`, `reduce_kv`, `sort`, and `sort_by`
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
- **Governed REQs**: `REQ-FN-032` (binding), `REQ-FN-033` (deferred), `REQ-FN-034` (binding), `REQ-FN-035` (binding), `REQ-FN-036` (binding alias), `REQ-FN-037` (binding), `REQ-FN-038` (binding), `REQ-FN-039` (binding, active), `REQ-FN-040` (binding, active), `REQ-FN-041` (binding integration contract), `REQ-SEQ-022`.