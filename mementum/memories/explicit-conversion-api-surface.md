---
type: Decision
symbol: 🎯
title: Explicit conversion API surface uses native destinations
related: [drop-to-num-planned-conversion.md]
---

Human approval 2026-11-06: planned Core conversions are `to_byte`, `to_char`, `to_count`, `to_double`, `to_float`, `to_index`, `to_int`, `to_long`, `to_long_double`, `to_long_long`, `to_short`, `to_size`, `to_uint`, `to_ulong`, `to_ulong_long`, `to_ushort`, and `to<T>`.

Named destinations are native C++ types: `to_int` targets `int`, superseding the fixed-signed-32-bit provision. Byte targets `std::byte`. Human later approved count, index, and size as complete behavioral aliases targeting `std::size_t`, with matching preflights and no collection-specific bounds or capacity check. No external application configuration file or App* aliases are required.

Generic conversion is restricted to defined conversion contracts, not arbitrary casts. Human approval: generic destinations reject bool, character types other than char, enums, pointers, and user-defined types; aliases follow their underlying type. Approved char, std::byte, non-character integer and binary floating destinations remain admitted subject to their contracts. Equivalent named/generic contracts must agree. Conversion policy, source admission, runtime result representation, and failure behavior remain unspecified at operation level; implementation is not approved. A later human decision tightens implicit integer/floating domain crossings; see `implicit-integer-floating-domain-crossings.md`. Equality's existing exclusion of raw floating-point values remains unchanged.

Recorded in REQ-NUM-008/017, architecture's planned conversion surface, and the Core cheatsheet.
