---
type: Decision
symbol: 🎯
title: Explicit conversion for integer-floating domain crossings
related: [explicit-conversion-api-surface.md, safe-implicit-conversions-in-equality.md]
---

Human approval 2026-11-06: ordinary cljonic API boundaries reject implicit integer-to-floating and floating-to-integer conversion at compile time, even where an integer-to-floating conversion preserves all source values. Same-type admission and value-preserving widening within integer or floating domains remain subject to existing operation constraints. The rule adds no general runtime value checks; existing checked-normalization and operation-specific exceptions remain unchanged.

Explicit `to_*`/`to<T>` conversions and preflights receive the source in its original type and validate the particular value under their own contract. A conversion performed before a cljonic call is not detectable; normal-mode conversion fallback does not prove success.

REQ-CONV-001/002, REQ-NUM-009, callback requirements, architecture and vocabulary were updated. This is a breaking approved requirements change, not implemented; README distinguishes current behavior. Propagate to affected specifications, tests and implementation only with explicit human direction. Existing equality rejects raw floating values independently, and its identity-sensitive boundaries remain unchanged.
