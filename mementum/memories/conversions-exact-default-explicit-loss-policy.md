---
type: Decision
symbol: 🎯
title: Conversions are exact by default with explicit loss policies
related: [explicit-conversion-api-surface.md]
---

Human approval 2026-11-06: conversions are exact by default for the supplied value, not necessarily the whole source type. A later human decision tightens implicit admission across integer and floating domains; see `implicit-integer-floating-domain-crossings.md`.

The same named/generic conversion functions may accept explicitly selected loss policies. Floating-to-integer truncation is toward zero, failing if the truncated result is out of range. Integer-to-floating and floating-to-floating nearest rounding permit precision loss but reject overflow, without infinity or finite clamping as overflow fallbacks. Wrapping and saturation are not approved.

Human approval: nearest halfway ties choose an even least-significant significand digit, independently of the ambient rounding mode. Exact conversion admits exactly representable subnormals; nearest admits a nonzero subnormal but fails if a nonzero source rounds to zero, for either sign. Destinations without subnormals use their actual representable values.

Human approval: all numeric conversions reject NaN and positive or negative infinity, regardless of destination or policy, including same-type conversions. Preflight returns false; conversion uses the configured failure behavior. This does not prohibit non-conversion storage or use of such values.

Human approval: floating-to-floating zero preserves sign when supported; negative zero fails if destination lacks signed zero. Both signed floating zeros convert exactly to integer zero; integer zero converts to positive floating zero. Nonzero-to-zero nearest rounding remains a failure.

Human approval: support compiler `float`, `double`, and `long double` with binary radix where the environment satisfies approved conversion contracts. Generic conversion may support additional binary floating types with defined, satisfied contracts. Non-binary formats remain unapproved; reject detectable unsupported types and document undetectable environment restrictions.

Human approval: numeric source/destination matrix uses exact by default for integer→integer, integer→floating, floating→integer, and floating→floating. Truncate is permitted only for floating→integer; nearest only for integer→floating and different-type floating→floating. Human approved same-type conversions as exact-default-only; explicit loss policies reject at compile time, while floating same-type values retain non-finite and signed-zero checks. Same-type uses actual C++ type identity: aliases of the same type are identical, distinct types stay different even with matching representations. Other policy pairings fail at compile time. Matching preflights use the same matrix.

Human approval: policy selection is an optional explicitly selected argument after the value; omission means exact. Preflight takes the same policy argument. Distinct compile-time tag objects `cljonic::core::conversion::truncate` and `cljonic::core::conversion::nearest` preserve that syntax and reject unsupported combinations at compile time. An internal enum is optional; no public runtime enum policy is required.

Human approval: exact and nearest floating-destination conversions reject finite values whose magnitude exceeds the destination's largest finite value, even when nearest would otherwise select that maximum. Human approval: supported conversions and preflights can be constant-evaluated where inputs and environment permit. Failed conversion required as a constant expression fails compilation without runtime diagnostic, handler, or fallback; failed constant preflight returns false. A call not required to be constant-evaluated follows runtime mode even for a literal. This supersedes the overly broad 'statically known' rejection wording in REQ-NUM-009. Human approved a narrow REQ-ERR-008 exception for explicit numeric conversions; other operations retain their general overflow contract.

Human approval: exact integer-to-unsigned conversion rejects negative input before conversion (including std::size_t) and never wraps; nonnegative values must fit. This does not alter floating truncation of -0.5 to unsigned zero.

Human approval: exact floating-to-integer checks finiteness, integrality, and mathematical destination range before conversion; never use an out-of-range cast to test safety. Truncation checks the mathematical truncated-toward-zero integer against the destination range before casting; an original outside the integer range may succeed if the truncated result fits (e.g. -0.5 to unsigned gives zero). Diagnostic precedence applies when failures overlap.

Range rules concern representable values, not exponent fields alone. Source admission and preflight/direct-return behavior are recorded separately.

REQ-NUM-009/012 and architecture record this first conversion-policy decision. Continue discussing one item at a time: recommend, iterate, obtain human approval, then advance. No implementation approved.
