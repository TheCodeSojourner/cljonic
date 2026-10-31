---
type: Insight
symbol: 💡
title: parameterized-wrapper-smuggling
related: [generic-deleted-templates-over-type-whitelist.md, nothrow-stable-equality-comparable-concept.md, value-equality-domain.md]
---
A parameterized type from outside the library (`std::pair`, `std::optional`,
`std::tuple`, `std::variant`) hides its stored types from cljonic's walkers: the
walkers recurse through cljonic composites, `MapEntry`, and producer parameters,
not through arbitrary templates. Admitting such a wrapper therefore smuggles a
rejected component past the recursive gate — `std::optional<double>` behaves
like a plain value while carrying a floating-point equality that REQ-NUM-006
forbids.

The fix is not an exclusion list. Require a property the wrapper must itself
declare and that the concrete instantiation cannot fake: for equality positions,
a **non-throwing `operator==`**. The standard library declares no non-throwing
comparison on those wrappers, so they fail admission for every instantiation,
while user classes that declare `noexcept operator==` (REQ-NUM-018) still pass
and remain the author's contract. Same shape as
`generic-deleted-templates-over-type-whitelist`: constrain on a property of the
type, never enumerate the types to reject.
