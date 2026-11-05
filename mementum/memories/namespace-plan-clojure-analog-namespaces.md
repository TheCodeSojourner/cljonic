---
type: Decision
symbol: 🎯
title: namespace-plan-clojure-analog-namespaces
---
The library namespaces mirror Clojure's library organization:

- `cljonic::core`: houses every Core free function, including collection factories and core primitive operations (`conj`, `assoc`, `dissoc`, `disj`, `peek`, `pop`, `first`, `rest`, `count`, etc.).
- `cljonic::lib_set`: will house relational and set-theoretic operations equivalent to `clojure.set`; binary algebra functions use `set_` names (`set_union`, `set_intersection`, `set_difference`), while relation predicates retain `is_subset` and `is_superset`.
- `cljonic::lib_string`: will house string-specific utilities and transformations equivalent to `clojure.string` (e.g. `join`, `split`, `trim`, `upper_case`, `lower_case`, `replace`).

Core collection types (`Vector`, `Map`, `Set`, `Queue`, `String`, `MapEntry`, and `Variant`) remain in `cljonic::`; their factory functions belong to `cljonic::core`. The `cljonic::lib_set` and `cljonic::lib_string` namespaces are purely functional utility namespaces matching Clojure's library boundaries. Consumers can opt into short aliases in their own translation units: `namespace st = cljonic::lib_set;` and `namespace s = cljonic::lib_string;`.
