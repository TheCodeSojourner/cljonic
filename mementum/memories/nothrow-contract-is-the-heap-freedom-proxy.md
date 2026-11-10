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

The non-throwing contract is **not a proof of heap-freedom**: storage and
equality admission require non-throwing copy construction, copy assignment, and
comparison, but C++ permits an operation to allocate and still be `noexcept`
(for example, if it handles allocation failure internally). This can exclude
many allocating operations, but the independent REQ-PLAT-009 prohibition still
applies to every public path and its transitive dependencies. Heap-freedom is
necessary but not sufficient — `std::variant<int,long>` is heap-free and still
rejected because it is not a cljonic value type.

Corollary: do not add "heap-free standard structures" to the interop surface as
a guarantee. Keep the surface narrow (const non-owning views, span/range
sources) and let capability admission decide per type.
