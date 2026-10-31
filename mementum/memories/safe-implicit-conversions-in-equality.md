---
type: Decision
symbol: 🎯
title: safe-implicit-conversions-in-equality
related: [value-equality-domain.md, parameterized-wrapper-smuggling.md, nothrow-stable-equality-comparable-concept.md]
---
Human decision 2026-10-31 reverses the standing "cross-type numeric unification
permanently out of scope" rule for `equal`/`not_equal`: a pair is comparable when
its types are identical or related by a **value-preserving** implicit conversion
in either direction (`concepts::SafeImplicitConversion`), compared in the wider
type. `equal(1, 1L)` is true; sign crossings (`int`/`unsigned`), character, bool,
enumeration, pointer, and user-defined pairs stay rejected. Equality stays an
equivalence relation because admitted conversions are injective on values.

Scope: scalar operands and sequential element positions only. Identity-sensitive
positions keep identical types — `cljonic::Variant` alternative identity
(`Variant<int,long>{1}` != `{1L}`, still FINAL), Map keys and values, Set
elements, String content. `equal` is still not the C++ interop surface:
standard ranges/containers and every parameterized std wrapper stay rejected.

Recorded in REQ-FN-002G (decision footer with `rationale_approved_by: human`),
REQ-CAP-010, `specs/sequences/equal.allium`, and architecture
`λ S2_general_equality_function`.
