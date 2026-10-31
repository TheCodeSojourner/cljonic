---
type: Decision
symbol: 🎯
title: aggregate-equality-boundary-is-caller-owned
---
A user-defined aggregate struct that provides a non-throwing `operator==` — defaulted or user-written — is admitted to the stable equality domain even when it stores floating-point members (human decision, 2026-10-31, `REQ-NUM-018`).

- The numeric recursion of `REQ-NUM-007` stops at a user-defined aggregate boundary; the library performs no member-level analysis of that type.
- The burden of semantically correct equality — including NaN, precision, and rounding behavior — transfers to the type's author. Admission is the explicit transfer.
- An aggregate without an available non-throwing `operator==` remains outside the domain and is rejected at compile time.
- The boundary is a limitation of the type system, not a defect: C++23 has no standard reflection, and the aggregate-tie / field-count approximations are aggregate-only, arity-bounded, and blind to private and base members. C++26 static reflection (P2996) would be needed to close it.
- Observable consequence: `Vector<double, 2>` is rejected, while `Vector<MyStruct, 2>` is admitted when `MyStruct::operator==` is non-throwing, even if `MyStruct` holds a `double`.

Locked by `tests/cljonic-concepts-spec-tests.cpp` and `tests/cljonic-collection-equality-spec-tests.cpp`.
