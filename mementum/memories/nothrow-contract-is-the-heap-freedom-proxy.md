---
type: Insight
symbol: 💡
title: nothrow-contract-is-the-heap-freedom-proxy
related: [no-heap-symbol-scan-gate.md, parameterized-wrapper-smuggling.md, value-equality-domain.md]
---
"No heap" cannot be queried in C++23: there is no reflection and no trait for
"does this type allocate". Heap use is a property of the operations exercised and
of the element types, not of the type (`std::optional<T>` is heap-free iff `T`
is). REQ-PLAT-009 already forbids allocation on any public path, "including
allocation performed indirectly by a standard-library function or other
transitive dependency".

The enforceable proxy is the **nothrow contract**: storage and equality
admission require non-throwing copy construction, copy assignment, and
comparison. Allocation can fail, so an allocating copy or comparison is not
`noexcept`; `std::string` and `std::vector` fall out for that reason while
`std::pair<int,int>` does not. Heap-freedom is therefore necessary but not
sufficient — `std::variant<int,long>` is heap-free and still rejected because it
is not a cljonic value type.

Corollary: do not add "heap-free standard structures" to the interop surface as
a guarantee. Keep the surface narrow (const non-owning views, span/range
sources) and let capability admission decide per type.
