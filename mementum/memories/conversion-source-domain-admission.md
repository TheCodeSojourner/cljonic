---
type: Decision
symbol: 🎯
title: Explicit conversion source domains and character-byte boundaries
related: [explicit-conversion-api-surface.md, conversions-exact-default-explicit-loss-policy.md]
---

The human's 2026-11-11 approval in REQ-NUM-013 supersedes the earlier character-conversion admission decisions here. Supported non-Boolean integer and floating sources remain admitted for ordinary numeric destinations; plain `char` is additionally admitted for non-character integer destinations only, preserving its native signedness and numeric value after range checking. Char-to-floating and byte-to-numeric remain unsupported.

`to_char`/`to<char>` admit non-Boolean, non-character integer values, plain `char`, and `std::byte`. They check the native numeric value against `std::numeric_limits<char>` before casting; every native `char` value, including NUL and signed negative values, succeeds unchanged. `to_byte`/`to<std::byte>` admit integers in `0..UCHAR_MAX`, nonnegative native `char` values including NUL, and unchanged `std::byte`; negative `char` fails rather than being reinterpreted as an unsigned byte.

Character and byte conversions are exact-only. Unsupported source types and policies fail at compile time; admitted values outside the destination range produce a false preflight and the configured conversion failure (`out_of_range` for character/byte range failures). Named and generic forms share the same contract, and aliases follow their underlying types. Conversions preserve code values; they do not parse text or invoke arbitrary user-defined conversions. Low-level character conversion is independent of String validity: String's non-NUL ASCII policy, `character_is_valid`, and ordinary implicit character-domain restrictions remain unchanged.
