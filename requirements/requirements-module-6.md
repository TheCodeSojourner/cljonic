# cljonic Requirements - Module 6: Numeric & Callable Convenience

## Purpose and Scope

This module specifies the checked fixed-width `NumericPolicy`, scalar comparison/selection, bitwise math, conversion/`Parsing` restrictions, raw floating-point policies, closure storage, and callable convenience adapters (`partial`, `juxt`, `fnil`, `apply`, etc.). Module 6 ensures numeric and functional computations remain deterministic, non-allocating, and compile-time checked.

## Fixed-Width NumericPolicy

λ REQ-NUM-000(x).
  ∀ raw floating-point calculation ∧ comparison: treated as potentially non-deterministic across values, compiler settings, evaluation modes, and platforms because of rounding, representation, NaN, infinity, signed zero, and other IEEE-754 effects
  ∧ ∀ raw floating-point behavior: ¬∃ use for a stable semantic contract unless an explicit `NumericPolicy` defines and tests that behavior
  rationale: rounding, representation, NaN, infinity, and signed-zero effects make raw floating-point behavior non-deterministic across platforms
  {source: stakeholder_decided, decided_by: original_spec_author, rationale_source: origin_artifact}

λ REQ-NUM-001(x).
  ∃ permitted_path: raw floating-point types stored as scalar values, vector elements, or map values when the selected operation does not require semantic equality or ordering, because those storage operations do not claim a stable floating-point result or identity contract
  {source: stakeholder_decided, decided_by: original_spec_author}

λ REQ-NUM-002(x).
  ∀ raw floating-point type: ¬∃ acceptance as map key ∨ set element because rounding, representation, NaN, infinity, and signed-zero behavior can make equality unsuitable as a stable identity across supported environments
  rationale: floating-point equality is unsuitable as a stable identity across supported environments
  {source: stakeholder_decided, decided_by: original_spec_author, rationale_source: origin_artifact}

λ REQ-NUM-003(x).
  ∀ `range`: requires an integral or otherwise explicitly `DiscreteNumericType`
  ∧ ∀ raw floating-point bound ∨ step: rejected at compile time because rounding and evaluation differences can change termination and produced values
  rationale: rounding and evaluation differences can change termination and produced values, so range termination cannot be a stable contract over FP
  {source: stakeholder_decided, decided_by: original_spec_author, rationale_source: origin_artifact}

λ REQ-NUM-004(x).
  ∀ operation requiring equality ∨ ordering: expresses that requirement through a named capability or equivalent compile-time constraint
  ∧ ∀ raw floating-point type: ¬∃ satisfaction of the default stable-equality or total-order capabilities because their results and comparisons may vary across supported environments
  rationale: floating-point results and comparisons may vary across supported environments, so they cannot back default equality/order capabilities
  {source: stakeholder_decided, decided_by: original_spec_author, rationale_source: origin_artifact}

λ REQ-NUM-005(x).
  ∀ (sorting ∨ other ordered traversal) of raw floating-point values: fails at compile time because NaN, signed zero, rounding, and platform-dependent evaluation can prevent a stable total order
  ∧ ∃ permitted_path: a future explicitly configured total-order numeric wrapper provides the required capability ∧ ∀ raw `float` ∧ `double`: remain unsupported by the default policy
  rationale: NaN, signed zero, rounding, and platform-dependent evaluation prevent a stable total order for raw FP
  {source: stakeholder_decided, decided_by: original_spec_author, rationale_source: origin_artifact}

λ REQ-NUM-006(x).
  ∀ (equality ∨ distinctness ∨ frequency counting ∨ other identity operation) over raw floating-point values: rejected unless an explicitly approved numeric policy provides the required semantics, because rounding and special-value behavior can make those results non-deterministic
  ∧ ∀ context: ¬∃ implicit epsilon comparison ∨ NaN normalization policy provided
  rationale: rounding and special-value behavior make FP identity results non-deterministic without an explicit numeric policy
  {source: stakeholder_decided, decided_by: original_spec_author, rationale_source: origin_artifact}

λ REQ-NUM-007(x).
  ∀ collection operation that compares values: applies the same numeric capability constraints recursively to its element, key, and value types
  ∧ ∀ equality of collections containing raw floating-point values: rejected by default even when the collection itself can store those values, because nested floating-point comparisons may not be stable across supported environments
  ∧ ∀ recursion: traverses only the components the library can analyze — `Collection`, `Producer`, `MapEntry`, `cljonic::Variant` alternatives, and the scalars themselves — ∧ stops at a user-defined class boundary under `REQ-NUM-018`
  rationale: nested floating-point comparisons may not be stable across supported environments, even when storage is permitted
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-10-31, rationale_source: origin_artifact}

λ REQ-NUM-008(x).
  ∀ numeric_conversion_operation: has an explicit target type ∧ ¬∃ selection of its result type through a global configuration constant ∧ ¬∃ required external application configuration file
  ∧ ∀ named conversion: destination(`to_byte`) ≡ std::byte ∧ destination(`to_char`) ≡ char ∧ destination(`to_short`) ≡ short ∧ destination(`to_int`) ≡ int ∧ destination(`to_long`) ≡ long ∧ destination(`to_long_long`) ≡ long long
  ∧ ∀ unsigned named conversion: destination(`to_ushort`) ≡ unsigned short ∧ destination(`to_uint`) ≡ unsigned int ∧ destination(`to_ulong`) ≡ unsigned long ∧ destination(`to_ulong_long`) ≡ unsigned long long
  ∧ ∀ floating named conversion: destination(`to_float`) ≡ float ∧ destination(`to_double`) ≡ double ∧ destination(`to_long_double`) ≡ long double
  ∧ destination(`to_size`) ≡ destination(`to_count`) ≡ destination(`to_index`) ≡ std::size_t ∧ `to_count` and `to_index` are complete behavioral aliases of `to_size` ∧ their preflights match `can_to_size`
  ∧ ∀ these aliases: same source admission, policies, exactness, failures, fallback, diagnostics, and std::size_t representability check ∧ ¬∃ implied collection-specific bounds or capacity check
  ∧ ∀ generic conversion `to<T>(value)`: destination ≡ explicit T ∧ admits only source/destination combinations with defined cljonic conversion contracts ∧ ¬∃ arbitrary C++ cast facility
  ∧ generic destination `bool`, character types other than supported `char`, enum, pointer, or user-defined type: rejected at compile time ∧ typedef aliases follow their underlying type
  ∧ ∀ enum exclusion: `std::byte` is explicitly exempt under its approved byte contract ∧ ¬∃ blanket exemption for scoped enums
  ∧ admitted generic destinations include `char`, `std::byte`, and supported non-character integer and binary floating-point types subject to their operation contracts
  ∧ ∀ named conversion and generic conversion expressing the same contract: identical admission, result, and failure behavior
  ∧ ∀ typedef alias naming the same type: ¬∃ distinct conversion specialization solely because of the alias name
  ∧ ∀ conversion entry: API surface and destination approved ∧ source admission follows REQ-NUM-013 ∧ exactness and constant evaluation follow REQ-NUM-009 ∧ runtime result representation, preflight, and failure behavior follow REQ-NUM-010 and REQ-NUM-010A–010E ∧ loss policies follow REQ-NUM-012 ∧ an operation-level behavioral specification is required before implementation
  rationale: familiar native destinations and an explicit generic destination support application needs without globally configurable numeric aliases; count and index share the size destination type
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06, rationale_source: origin_artifact}

λ REQ-NUM-009(x).
  ∀ numeric conversion without an explicitly selected loss policy: exact by default ∧ succeeds only when the supplied value is exactly representable in the destination ∧ ¬∃ silent truncation, rounding, wrapping, or saturation
  ∧ ∀ supported conversion and corresponding preflight: usable in constant expressions when the supplied value, selected policy, and target environment support constant evaluation
  ∧ ∀ failed conversion required to be a constant expression: compilation fails ∧ ¬∃ runtime diagnostic recording ∨ handler invocation ∨ fallback return during constant evaluation
  ∧ ∀ failed preflight in a constant expression: returns false ∧ ¬∃ compile-time failure solely because the checked conversion would fail
  ∧ a conversion call not required to be constant-evaluated follows selected runtime mode even when its input is a literal ∧ ¬∃ guarantee of compile-time rejection merely because a value is statically knowable
  ∧ explicit conversion checks the supplied value ∧ ¬∃ requirement that every value of the source type fit the destination ∧ ordinary argument admission follows the revised type-based rule in REQ-CONV-001
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06}

λ REQ-NUM-010(x).
  ∀ runtime conversion: provides an idiomatic preflight that checks whether the supplied value can be converted under the selected policy ∧ conversion returns its destination value directly rather than requiring a checked-result wrapper
  ∧ ∀ named `to_*`: provides corresponding `can_to_*` ∧ generic `to<T>`: provides `can_to<T>`
  ∧ ∀ preflight: returns true exactly when the corresponding conversion succeeds for the same source value and selected policy ∧ accepts the same source types and policies ∧ rejects unsupported source types at compile time
  ∧ ∀ preflight: false return ¬∃ recording of diagnostic ∨ invocation of failure handler ∧ non-throwing ∧ non-allocating
  ∧ preflight result applies only to the checked value and policy ∧ a subsequent change of value or policy invalidates that guarantee
  ∧ a false preflight followed by conversion of the same value and policy produces the configured normal fallback or diagnostic stop ∧ diagnostic code reflects the same failed conditions under the approved precedence
  ∧ this equivalence applies to numeric, character, byte, and count/index/size conversions ∧ ¬∃ requirement for caching or a particular implementation
  | refinements: REQ-NUM-010A–010E
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06}

λ REQ-NUM-010A(x).
  ∀ application: may select normal mode instead of diagnostic mode ∧ normal-mode runtime conversion failure returns a documented destination value
  ∧ normal-mode integer destination failure: returns `std::numeric_limits<T>::max()` ∧ includes `std::size_t` for `to_count`, `to_index`, and `to_size`
  ∧ normal-mode floating destination failure: returns `std::numeric_limits<T>::max()` — the largest finite destination value
  ∧ normal-mode `std::byte` destination failure: returns a byte with all bits set ∧ normal-mode `char` destination failure: returns NUL (`'\0'`), outside cljonic's valid String-character domain
  ∧ fallback is not a universally distinguishable error code ∧ success may produce the same value ∧ callers requiring certainty use the preflight
  ∧ normal-mode fallback passed to another API is an ordinary destination value ∧ receiving API applies its own documented checks ∧ ¬∃ inference of prior conversion failure ∧ ¬∃ hidden failure flag
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06}

λ REQ-NUM-010B(x).
  ∀ supported implementation: provides a selectable diagnostic mode in which a runtime conversion failure records a compact error code in a static variable ∧ stops execution without returning a conversion value
  ∧ application selects conversion mode at compile time using `CLJONIC_CONVERSION_DIAGNOSTICS` ∧ undefined macro defaults to normal mode (`0`) ∧ `1` selects diagnostic mode ∧ every other value rejected at compile time
  ∧ configuration set before including cljonic ∧ consistent across all application translation units ∧ no external configuration file required
  ∧ normal mode: ¬∃ diagnostic-state storage, handler reference, registration, or initialization ∧ `conversion_error()` returns `ConversionError::none` ∧ ¬∃ claim of zero conversion checking or executable-code overhead
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06}

λ REQ-NUM-010C(x).
  ∀ diagnostic code: non-allocating ∧ suitable for inspection with a debugger or crash dump ∧ documented meaning ∧ ¬∃ runtime error-message construction requirement
  ∧ diagnostic code type: scoped enum `cljonic::core::ConversionError` with values `none`, `non_finite_input`, `invalid_character`, `unsupported_signed_zero`, `out_of_range`, `underflow_to_zero`, and `precision_loss`
  ∧ diagnostic mode: one library-managed static error code shared across the application and its translation units ∧ initially holds `ConversionError::none` before normal application execution
  ∧ successful conversions and all preflights leave the code unchanged ∧ diagnostic-mode failure sets the selected error before invoking the non-returning handler
  ∧ `cljonic::core::conversion_error()` provides non-throwing, non-allocating, read-only access to the recorded code ∧ handler can read a failure originating in any translation unit ∧ ¬∃ reset operation required
  ∧ debugger or crash-dump visibility depends on target and build configuration ∧ ¬∃ guarantee that a reset preserves diagnostic state
  ∧ diagnostic failure categories: out of range ∨ precision loss ∨ non-finite input ∨ nonzero underflow to zero ∨ invalid character ∨ unsupported signed zero
  ∧ failed preflight: ¬∃ diagnostic-state change ∧ unsupported source types and policy combinations: compile-time rejection, not runtime diagnostic categories
  ∧ if a failed conversion matches multiple categories: choose first applicable in order non-finite input → invalid character → unsupported signed zero → out of range → nonzero underflow to zero → precision loss
  ∧ determine failure and applicable categories under the selected policy before applying precedence ∧ precedence changes the diagnostic only, not conversion success
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06}

λ REQ-NUM-010D(x).
  ∀ diagnostic-mode failure: record the error code before invoking an application-provided non-returning failure handler ∧ the handler may inspect the recorded code ∧ ¬∃ returning to the failed conversion
  ∧ ∀ failure handler: non-throwing ∧ ¬∃ dynamic allocation or deallocation
  ∧ handler non-returning, non-throwing, non-allocating behavior: application responsibility verified by review and target-specific tests ∧ cljonic declares and invokes the handler according to its contract ∧ ¬∃ claim that cljonic detects a handler returning or allocating
  ∧ returning from the declared `[[noreturn]]` handler violates its C++ contract ∧ ¬∃ recovery as an ordinary conversion failure
  ∧ diagnostic-mode application defines `[[noreturn]] void cljonic::core::conversion_failure_handler() noexcept` matching cljonic's declaration ∧ ¬∃ library-provided default handler ∧ ¬∃ runtime registration ∧ ¬∃ external configuration file required
  ∧ normal-mode application: ¬∃ requirement to define the diagnostic failure handler ∧ diagnostic build with reachable failure path and missing handler definition fails to link
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06}

λ REQ-NUM-010E(x).
  ∀ conversion mode: non-throwing ∧ non-allocating ∧ avoids performing an invalid underlying C++ conversion
  ∧ diagnostic state follows the single-threaded and caller-confinement model of REQ-PLAT-012 and REQ-PLAT-013 ∧ ¬∃ required synchronization or thread safety
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06}

λ REQ-NUM-011(x).
  ∀ potentially lossy conversion from a runtime floating-point value to an integer: ¬∃ performance by an implicitly narrowing overload because rounding and representation differences can change the converted result ∧ requires an explicitly checked or explicitly lossy conversion operation named to make the policy visible at the call site
  rationale: the policy must be visible at the call site because rounding and representation differences can change the converted result
  {source: stakeholder_decided, decided_by: original_spec_author, rationale_source: origin_artifact}

λ REQ-NUM-012(x).
  | scope:
    S and T denote the actual source and destination C++ types; s is the supplied source value.
    Integer excludes bool and all character types under REQ-NUM-013. Floating means a supported floating type under the environment rules below.
    Source/destination admission follows REQ-NUM-008 and REQ-NUM-013 independently of policy selection. The matrix applies only to admitted numeric pairs. Each permitted policy and result remains subject to every applicable value check below.
  | policy_matrix:

    | Source → destination | Exact default | Truncate | Nearest |
    | --- | --- | --- | --- |
    | Integer → integer | Value lies within destination range | Rejected | Rejected |
    | Integer → floating | Value is exactly representable | Rejected | Allowed |
    | Floating → integer | Value is finite, integral, and within destination range | Allowed | Rejected |
    | Different floating types | Value is exactly representable | Rejected | Allowed |
    | Same actual C++ type | Exact only; applicable validity checks remain | Rejected | Rejected |

    Same-type admission uses actual C++ type identity, not alias spelling or matching representations. Same-type floating conversions retain non-finite and signed-zero checks.
  | policy_interface:
    Omission selects exact. Explicit policy selection uses an argument after the value; matching preflight accepts the same policy argument.
    Policies are distinct, scoped compile-time tag objects: cljonic::core::conversion::truncate and cljonic::core::conversion::nearest. Unsupported source/destination/policy combinations are rejected at compile time, never silently ignored.
    An internal enum may support the tags. Neither a public runtime enum policy argument nor a separately named function for each policy is required.
  | integer_and_truncation_boundaries:
    Integer → integer succeeds exactly when the value fits the destination. Negative integer input to an unsigned destination, including std::size_t, fails as out_of_range before conversion; it never wraps modulo the destination range.
    For floating → integer, classify non-finite input before evaluating integrality, range, or truncation:
      exact → check finiteness, absence of a fractional part, and mathematical destination range before casting
      truncate → for finite input, truncate toward zero and check the mathematical truncated integer against destination range before casting; return it if it fits, otherwise fail as out_of_range
    Truncation can succeed when the original value lies outside integer range: -0.5 converts to unsigned zero. Never use an invalid cast as a safety test, wrap, or saturate.
  | floating_range_and_rounding:
    For either exact or nearest with a floating destination:
      finite input magnitude > destination's largest finite value → fail as out_of_range before rounding, even if unrestricted rounding would select that maximum
      exact → require exact representability
      nearest → select the nearest representable value, permitting precision loss; halfway ties select the even least-significant significand digit independently of ambient rounding mode
    The largest finite value may be a successful result when the source lies within destination range and the selected policy produces it, subject to all other checks. Infinity is never a successful result; clamping cannot replace overflow failure.
  | subnormal_and_underflow:
    For finite input within floating destination range, exact admits exactly representable subnormals; nearest admits a nonzero subnormal result.
    nearest ∧ nonzero source rounds to zero → fail as underflow_to_zero, for either sign
    Use the destination's actual representable values when it lacks subnormals; do not assume universal subnormal support.
  | nonfinite_and_zero:
    NaN and positive or negative infinity → fail as non_finite_input under every admitted numeric policy, including same-type conversion
    Use floating classification, not equality with NaN, before evaluating integrality, range, truncation, or rounding. Rejecting these values does not reject their source type or prohibit non-conversion storage or use.
    For floating zero → floating:
      destination supports signed zero → preserve the source zero's sign
      destination lacks signed zero ∧ source is positive zero → return positive zero
      destination lacks signed zero ∧ source is negative zero → fail as unsupported_signed_zero
    Both signs of floating zero convert exactly to integer zero. Integer zero converts to positive floating zero under every admitted policy.
  | failure_and_preflight:
    Failed value checks give false preflight and conversion failure under REQ-NUM-009 and REQ-NUM-010/010A–010E. Diagnostic classification and overlapping-failure precedence follow REQ-NUM-010C.
    Runtime mode behavior and required-constant-evaluation failure remain governed by those clauses. A normal-mode fallback is a failure result, not successful saturation.
  | supported_environments:
    Support float, double, and long double with binary radix where the environment satisfies these contracts. Generic conversion may additionally support compiler binary floating types with defined and satisfied contracts.
    Compiler options and runtime settings must not silently violate ties-to-even independent of ambient rounding mode, applicable subnormal and signed-zero rules, or non-finite detection where supported.
    Statically detectable unsupported floating types are rejected at compile time. Undetectable environmental restrictions are documented as unsupported configurations outside the contract. Non-binary formats remain unapproved.
  | boundaries:
    Range rules use representable values, not exponent-field comparisons alone. Wrapping, saturation, and other loss policies remain unapproved. Implementation requires an operation-level behavioral specification.
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06}

λ REQ-NUM-013(x).
  | local_definitions:
    s ≡ original source value ∧ S ≡ actual source type ∧ T ≡ destination type
    I ≡ supported built-in integer types excluding bool and all character types
    F ≡ supported floating types under REQ-NUM-012 ∧ N ≡ I ∪ F
    C ≡ char ∧ B ≡ std::byte
    code(s) ≡ character code for S = C, or unsigned byte value for S = B; neither denotes a parsed decimal digit
    ascii_valid(v) ≡ valid non-NUL cljonic ASCII code under existing character-validity rules, in 0x01–0x7F
    Type admission and value validation are separate. A matrix entry admits a source type; it does not establish that every value or policy succeeds.
  | boundaries:
    ∀ conversion_operation: distinct_from(Parsing) ∧ ¬implicitly_parse(text) ∧ ¬implicitly_invoke(user_defined_conversion)
  | source_destination_matrix:
    ∀ destination T supported by REQ-NUM-008:
      (T ∈ N → permitted_source_types = N)
      ∧ (T = C → permitted_source_types = I ∪ {C, B})
      ∧ (T = B → permitted_source_types = I ∪ {C, B})
    ∀ pair outside the matrix: compile_time_rejection unless a separate governing contract explicitly admits it
    ∀ numeric pair S ∈ N, T ∈ N: policy_admission_and_value_checks under REQ-NUM-012
    ∀ T ∈ {C, B}: exact_default_only ∧ compile_time_reject(truncate ∨ nearest)
    In particular, char and std::byte sources are not ordinary numeric sources. Other character types, bool, application enums, pointers, and user-defined types are rejected. The byte entries explicitly exempt std::byte from enum exclusions; they do not exempt application enums.
  | character_validation:
    ∀ s of admitted source type S, T = C:
      v ≡ s when S ∈ I; otherwise code(s)
      ∧ validate(ascii_valid(v)) before_cast
      ∧ (ascii_valid(v) → success(character_with_code(v)))
      ∧ (¬ascii_valid(v) → false_preflight ∧ configured_conversion_failure)
      ∧ ¬truncate ∧ ¬wrap ∧ ¬interpret_integer_as_printed_decimal_digit
    to_char(s) ≡ to<char>(s) under this same contract
  | byte_validation:
    ∀ s of S ∈ I, T = B:
      validate(0 ≤ s ≤ UCHAR_MAX) before_cast
      ∧ ((0 ≤ s ≤ UCHAR_MAX) → success(byte_with_value(s)))
      ∧ (¬(0 ≤ s ≤ UCHAR_MAX) → false_preflight ∧ configured_conversion_failure)
    ∀ s of S = C, T = B:
      validate(ascii_valid(code(s))) before_cast
      ∧ (ascii_valid(code(s)) → success(byte_with_value(code(s))))
      ∧ (¬ascii_valid(code(s)) → false_preflight ∧ configured_conversion_failure)
    ∀ s of S = B, T = B: success(s_unchanged)
    ∀ byte_conversion: ¬truncate ∧ ¬wrap ∧ byte_width_follows(UCHAR_MAX) ∧ ¬assume(eight_bit_byte)
    to_byte(s) ≡ to<std::byte>(s) under this same contract
  | type_identity_and_aliases:
    ∀ typedef_alias: admission_follows(actual_underlying_type) ∧ ¬distinct_type_from_alias_spelling
    ∀ character_type_alias: same_numeric_source_exclusion_as_underlying_character_type
    ∀ T ∈ {signed char, unsigned char} or aliases_of_those_types: generic_destination_rejected_at_compile_time
    ∀ supported_noncharacter_integer_destination: generic_destination_admitted_under_its_contract
    Examples: std::uint8_t aliases unsigned char on many platforms and inherits its exclusion; std::int8_t similarly inherits signed char exclusion. std::uint32_t is admitted when its underlying type is a supported non-character integer type such as unsigned int. Platform-dependent alias limitations are documented.
  | shared_contracts:
    ∀ supported T: same_type_source_admitted ∧ preserve(applicable_validity_and_special_value_checks)
    ∀ S ∈ F, T ∈ {C, B}: direct_call_rejected_at_compile_time, including_integral_valued_floating_sources_or_explicit_loss_policy
    Floating-to-character/byte use requires a successful explicit numeric-to-integer conversion first, followed by conversion of that integer. It does not introduce a direct floating-source overload.
    ∀ equivalent_named_and_generic_forms: identical_source_admission ∧ ¬admit(unlisted_character_or_byte_pair)
    ∀ unsupported_source_type: compile_time_rejection, not_runtime_value_failure
    ∀ admitted_source_value_failure: behavior_under(REQ-NUM-009 ∧ REQ-NUM-010 ∧ REQ-NUM-010A–010E)
    ∀ implementation: requires(operation_level_behavioral_specification)
  rationale: ordinary numeric conversion, character-code validation, raw-byte conversion, and parsing have distinct meanings; floating-to-character/byte conversion must expose the intermediate numeric policy and successful integer conversion
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06, rationale_source: origin_artifact}

λ REQ-NUM-014(x).
  ∀ `parse_float`: documents its accepted syntax, sign, decimal, exponent, infinity, NaN, range, rounding, and accuracy behavior
  ∧ ∀ parser: ¬∃ making of raw floating-point parsing suitable for stable equality, ordering, or identity contracts without the explicit numeric policy required elsewhere
  {source: stakeholder_decided, decided_by: original_spec_author}

λ REQ-NUM-015(x).
  ∀ fixed_width_arithmetic: checked behavior by default
  ∧ ∀ runtime arithmetic operation: provides corresponding preflight predicates such as `can_add`, `can_subtract`, and `can_multiply` ∧ when the predicate is false, the operation returns its documented default ∧ leaves its inputs unchanged
  ∧ ∀ compile_time_known arithmetic overflow: rejected during constant evaluation
  {source: stakeholder_decided, decided_by: original_spec_author}

λ REQ-NUM-016(x).
  ∃ permitted_path: wrapping and saturating arithmetic provided ∧ ∀ such_operation: separately named ∧ ¬∃ implicit behavior of the default arithmetic operations
  {source: stakeholder_decided, decided_by: original_spec_author}

λ REQ-NUM-017(x).
  ∀ context: the first-pass numeric convenience family includes checked fixed-width arithmetic corresponding to `add`, `sub`, `mult`, `div`, `quot`, `rem`, and `mod`, with `can_add`, `can_subtract`, and `can_multiply` preflights; scalar comparison and selection corresponding to `compare`, `min`, `max`, `min_key`, and `max_key`; integer-step operations corresponding to `inc`, `dec`, and `negate`; constrained numeric predicates corresponding to `is_zero`, `is_positive`, `is_negative`, `is_even`, and `is_odd`; explicit conversions corresponding to `to_byte`, `to_char`, `to_count`, `to_double`, `to_float`, `to_index`, `to_int`, `to_long`, `to_long_double`, `to_long_long`, `to_short`, `to_size`, `to_uint`, `to_ulong`, `to_ulong_long`, `to_ushort`, and `to<T>` under REQ-NUM-008; and fixed-width bitwise operations `bit_not`, `bit_and`, `bit_or`, `bit_xor`, `bit_and_not`, `bit_clear`, `bit_set`, `bit_flip`, `bit_test`, `bit_shift_left`, `bit_shift_right`, and `unsigned_bit_shift_right`
  ∧ ∀ such_operation: obeys the applicable compile-time capability, representability, overflow, division-by-zero, shift-count, floating-point, and preflight requirements
  ∧ ∀ raw floating-point value: remains excluded from operations that require the stable equality, identity, or total-order capabilities defined by this document
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-06}

λ REQ-NUM-018(x).
  ∀ user-defined class type — an `AggregateLikeStruct` ∨ a non-aggregate class — that provides `operator==` — defaulted ∨ user-written — and that comparison is non-throwing: admitted to the `StableEquality` domain, whether or not it contains floating-point members
  ∧ ∀ such type: the numeric capability recursion of `REQ-NUM-007` stops at this type boundary ∧ the library performs no member-level analysis of it ∧ the library never inspects, requires, or reasons about its individual members
  ∧ ∀ equality semantics inside such a type, including floating-point, NaN, and precision behavior: the responsibility of the type's author, not of cljonic; admitting the type is an explicit transfer of that burden to the user
  ∧ ∀ user-defined class type whose equality comparison is unavailable ∨ potentially throwing: outside the `StableEquality` domain ∧ rejected at compile time when used for equality; ∀ comparison: non-throwing status may be explicitly declared ∨ implicitly deduced, but a user-written comparison without a non-throwing exception specification is potentially throwing
  ∧ ∀ such rejection: does not itself prohibit storage when the applicable independent storage contract is satisfied
  ∧ ∀ non-aggregate user-defined class: admission requires `StableEquality` and the non-throwing comparison that the map-key and set-element contract also requires; that contract additionally requires non-throwing copy construction and assignment, which `equal` does not need because it never copies an operand
  ∧ ∀ parameterized standard-library wrapper (`std::pair`, `std::optional`, `std::tuple`, `std::variant`) ∧ ∀ other wrapper whose inner types the library cannot analyze: rejected however instantiated, because a floating-point or otherwise unsupported component could otherwise be smuggled in behind the wrapper; the standard library declares none of these comparisons non-throwing, so admission by the non-throwing-comparison rule already excludes them and no separate exclusion list is needed
  ∧ ∀ context: this boundary is an admitted limitation of the type system, not a defect — C++23 provides no standard reflection, so member-level analysis of an arbitrary user-defined class is unavailable; C++26 static reflection would be required to close it
  rationale: a type that states its own equality contract is a trustworthy boundary; refusing to admit it would remove ordinary C++ value types from the collection domain without making the equality any more verifiable
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-07, rationale_source: origin_artifact, rationale_approved_by: human}

λ REQ-NUM-019(x).
  ∀ floating-point equality comparison that is statically detectable at compile time: rejected at compile time ∧ never deferred to a runtime check
  ∧ ∀ raw floating-point operand of `equal` ∨ `not_equal` (any arity): rejected at compile time
  ∧ ∀ `Set` element type ∧ ∀ `Map` key type that is a raw floating-point type: rejected at compile time, because set membership and map key lookup are equality comparisons
  ∧ ∀ element, key, value, producer-parameter, or `Variant` alternative type of a `Collection`, `Producer`, `MapEntry`, or composite that is detectable as floating-point-bearing: rejected at compile time wherever that component participates in equality
  ∧ ∀ context: detectability is the criterion — every floating-point equality comparison the library can detect must be rejected ∧ ¬∃ detectable comparison that is silently admitted
  ∧ ∀ undetectable case (a floating-point member inside an opaque user-defined aggregate): governed by `REQ-NUM-018` ∧ ¬∃ claim that it was detected
  ∧ ∀ storage independent of equality: storing a floating-point value where the operation never compares it (for example `Vector<double, N>` element storage or `Map<int, double, N>` value storage) remains permitted ∧ every equality operation over such a collection remains rejected at compile time
  rationale: floating-point identity is not stable across supported environments; the defensible policy is to reject every detectable instance and to state honestly where detection stops
  {source: stakeholder_decided, decided_by: human, timestamp: 2026-11-07, rationale_source: origin_artifact, rationale_approved_by: human}

λ REQ-FN-002N(x).
  ∀ context: the supported free-function vocabulary includes `can_add`, `can_subtract`, and `can_multiply` for checked fixed-width arithmetic preflight
  ∧ ∀ corresponding arithmetic function: uses the documented checked default behavior ∧ ∀ wrapping ∨ saturating alternative: uses distinct names
  {source: stakeholder_decided, decided_by: original_spec_author}

## Stored Closures and Callable Convenience Family

λ REQ-VAL-021(x).
  ∀ stored closure whose closure type satisfies the required bounded representation, construction, copying, moving, destruction, and non-allocation capabilities: supported as a collection element and map value in the supported collection contract
  ∧ ∀ storing of a closure: ¬∃ granting of equality, ordering, map-key, set-element, or serialization capabilities
  {source: stakeholder_decided, decided_by: original_spec_author}

λ REQ-FN-025(x).
  ∀ context: the first-pass callable convenience family includes `identity`, `constantly`, `complement`, `partial`, `fnil`, `juxt`, `every_pred`, `some_fn`, and `apply`, in addition to the `comp` behavior defined by this document
  ∧ ∀ callable constructor ∨ adapter: expresses compatible parameter and return types through concepts or equivalent compile-time constraints ∧ returns a bounded callable representation when it returns a callable ∧ preserves the invocation and non-allocation requirements of its captured functions and values
  ∧ ∀ (`juxt` ∧ `every_pred` ∧ `some_fn`): define their short-circuit, result-shape, and invocation-order behavior ∧ ∀ (`partial` ∧ `fnil`): define their captured-argument and default-argument behavior ∧ ∀ `apply`: defines its supported final sequence argument and capacity behavior
  ∧ ∀ such_function: remains an ordinary callable operation ∧ ¬∃ (transducer semantics ∨ runtime type dispatch ∨ hidden mutable caching ∨ implicit callback retention by collection results)
  {source: stakeholder_decided, decided_by: original_spec_author}

## Traceability and Related Requirements

- **Downstream Artifact**: Numeric functions, checked arithmetic helpers, bitwise functions, and callable builder adapters (`partial`, `juxt`, `fnil`, `complement`, `apply`).
- **Governed REQs**: `REQ-NUM-000`–`019`, `REQ-NUM-010A`–`010E`, `REQ-FN-002N`, `REQ-VAL-021`, `REQ-FN-025`.