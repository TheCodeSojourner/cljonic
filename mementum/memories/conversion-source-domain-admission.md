---
type: Decision
symbol: 🎯
title: Explicit conversion source domains and character-byte boundaries
related: [explicit-conversion-api-surface.md, conversions-exact-default-explicit-loss-policy.md]
---

Human approval 2026-11-06: ordinary numeric destinations accept built-in integer and floating sources, excluding Boolean and all character types. The human reaffirmed that char-to-integer and char-to-floating calls fail at compile time rather than interpreting a character code or parsing a digit. Byte, enum, pointer, and user-defined sources are rejected unless separately admitted. Conversions do not parse text or invoke arbitrary user-defined conversions.

Human approval: `to_char` and `to<char>` accept non-Boolean/non-character integer sources and `char` sources only when the original value is valid cljonic ASCII `0x01`–`0x7F`; validate before casting. Invalid admitted values fail. Floating, Boolean, other character, enum, pointer, and user-defined sources, and truncate/nearest policies, are compile-time rejected. Human later approved `std::byte` sources only for unchanged byte and validated non-NUL ASCII char destinations; ordinary integer and floating destinations reject byte sources at compile time. Integer-to-char interprets a code, not a printed digit. Human approval: `to_byte`/`to<std::byte>` accept non-Boolean/non-character integers in 0 through `UCHAR_MAX`, valid ASCII chars as their code, and unchanged `std::byte`. Invalid admitted values fail; floating, Boolean, other character, enum, pointer, user-defined sources and truncate/nearest policies are rejected at compile time. No eight-bit byte assumption. Same-type conversion is admitted for supported destinations, subject to destination validity/special-value contracts. No other character/byte pair was approved.

All floating sources to char/byte are rejected, even integral-valued ones or calls requesting a loss policy. Callers must first obtain a successful explicit integer conversion.

Human approval: fixed-width typedefs that alias character types, such as `std::uint8_t` when it is `unsigned char`, have the same numeric-source exclusion; C++ cannot distinguish alias spelling. Generic `signed char` and `unsigned char` destinations and their aliases are rejected; `to<char>` and `to<std::byte>` keep separate contracts, and supported non-character integer destinations remain admitted.

Named and generic forms agree. Unsupported source types are distinct from value-check failures. REQ-NUM-013 replaces the older general character-to-int allowance. Next discuss runtime failure reporting; implementation remains unapproved.
