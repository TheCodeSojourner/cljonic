---
type: Pattern
symbol: 🔁
title: cljonic-naming-convention
---
All src and test files use `cljonic-` prefix with dashes. Header patterns:
- `src/cljonic-collection-{name}.hpp` — collection type (e.g., `cljonic-collection-map.hpp`)
- `src/cljonic-producer-{name}.hpp` — producer type (e.g., `cljonic-producer-range.hpp`)
- `src/cljonic-{operation}.hpp` — standalone public operation (e.g., `cljonic-map.hpp`); omit `-function`
- `src/cljonic-concepts.hpp` — shared concept constraints
- `src/cljonic-core.hpp` — umbrella include; holds `\mainpage` and includes public headers
- `tests/cljonic-{entity}-spec-tests.cpp` — spec tests per entity
- `tests/no_heap/cljonic-{entity}-probes.cpp` — no-heap probes per entity

Infrastructure files keep no prefix: `harness_main.cpp`, `poison.hpp`, `probes.hpp`.

Collection and producer type headers use their family prefixes; cross-cutting operations live in operation-named headers, not collection headers.
Makefile `help` entries must be added in alphabetical order.

C++ identifier naming follows the C++ Core Guidelines:
- Types and concepts: PascalCase (concepts per `NL.17`)
- Template parameters (type and non-type): PascalCase, e.g. `KeyType`, `ValueType`, `CapacityValue`, `IndexType`. Member type aliases (`value_type`, `key_type`, `lookup_type`) stay snake_case per STL convention; only the template parameter itself is PascalCase.
- Namespaces: lowercase_with_underscores (Clojure-analog `cljonic::core`, `cljonic::lib_set`, `cljonic::lib_string`)
- Free and member functions: snake_case (Clojure parity: `count`, `is_empty`, `contains`)
- Nominal concepts keep `Cljonic` prefix; capability/value concepts do not require it

`cljonic-vector.hpp`'s primary class template previously used snake_case template parameters (`element_type`, `capacity_value`, `index_type`) while its own `collection_traits` specialization already used PascalCase (`ElementType`, `CapacityValue`) — an internal inconsistency, not an intentional distinction. Fixed to PascalCase throughout to match Map/Set/Queue/String/MapEntry.
