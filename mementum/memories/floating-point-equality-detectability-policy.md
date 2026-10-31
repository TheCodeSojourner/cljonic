---
type: Decision
symbol: 🎯
title: floating-point-equality-detectability-policy
---
Floating-point equality is rejected at compile time **whenever it can be detected** (human decision, 2026-10-31, `REQ-NUM-019`). Detectability is the criterion, not a list of special cases.

Enforced surfaces, all verified against the current implementation:
- Raw floating-point operand of `equal` / `not_equal`, in every arity.
- `Set` element type: `Set<double, 4>` fails its template constraints.
- `Map` key type: `Map<double, int, 4>` fails its template constraints.
- Floating-point-bearing element, key, value, producer parameter, or `Variant` alternative visible to the recursive walkers.

Permitted, because storage is independent of equality:
- `Vector<double, N>` element storage, `Map<int, double, N>` value storage, and other non-compared storage positions. Equality over them is still rejected.

Detection stops at an opaque user-defined aggregate (`REQ-NUM-018`); that limit must be stated honestly, never claimed as detection.

The detection obligation is now enforced, not merely documented: every library-owned composite declares `concepts_detail::walker_coverage<Self>` in its own header, and `StableEqualityComparable` requires that declaration for nominal composites. Omitting it makes the type non-comparable at first equality use, so a detectable floating-point comparison can no longer be silently admitted. Verified by temporarily deleting Vector's declaration: `StableEqualityComparable<Vector<int, 2>>` failed.
