---
type: Decision
symbol: 🎯
title: nothrow-stable-equality-comparable-concept
related: [concept-naming-convention.md, two-level-concept-model.md, parameterized-wrapper-smuggling.md, value-equality-domain.md]
---
`NothrowStableEqualityComparable` in `cljonic-concepts.hpp` is the shared
admission contract for map keys and set elements:

```cpp
StableEqualityComparable<T> && NothrowCollectionElement<T> &&
concepts_detail::nothrow_equality_v<T>
```

The third term was added 2026-10-31 (human decision) and is the durable part of
this memory: the original concept required only nothrow *storage*, so
`hash_set(std::pair<double,double>{...})` and `hash_set(std::optional<double>{...})`
compiled — a floating-point equality smuggled through a standard wrapper that
the walkers cannot see inside. Requiring a non-throwing `operator==` rejects
those wrappers for every instantiation (the standard library declares no
non-throwing comparison on them) without any exclusion list, while user classes
with `noexcept operator==` still pass (REQ-NUM-018).

Set's element constraint uses the concept too, so the earlier "propagate to Set"
follow-up is closed. Propagated to `specs/capabilities/concepts.allium`
(`RequiresNothrowComparison`, `RejectsParameterizedStandardWrapperRegardlessOfInstantiation`),
`vocabulary.md`, the architecture concept snippet, and both factory harnesses.
