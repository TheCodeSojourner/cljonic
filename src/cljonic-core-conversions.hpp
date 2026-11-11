#pragma once

#include <climits>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

/** \file cljonic-core-conversions.hpp
 * \anchor ExplicitConversions
 * Explicit numeric, character, and byte conversions.
 *
 * A conversion policy is an optional second argument that explicitly permits a particular kind of precision loss. For
 * example, \c to_int(3.75, conversion::truncate) permits discarding the fractional part and returns 3. The available
 * policies are \c cljonic::core::conversion::truncate and \c cljonic::core::conversion::nearest; their permitted
 * conversion directions are described below. Matching preflights accept the same second argument, for example \c
 * can_to_int(3.75, conversion::truncate) returns true because the truncated value, 3, fits in int.
 *
 * When only the value is supplied, as in \c to_int(3.0), a numeric conversion succeeds only if the destination can
 * represent the supplied value exactly, without rounding, discarding a fractional part, or wrapping. This checks the
 * particular value, not whether every value of its source type would fit:
 * - **Integer-To-Integer:** the value must fit the destination's range; negative values cannot convert to unsigned
 * types.
 * - **Floating-To-Integer:** the value must be finite, have no fractional part, and fit the destination's range.
 *   For example, \c to_int(3.0) succeeds, while \c can_to_int(3.5) returns false.
 * - **Integer-To-Floating and Floating-To-Floating:** the destination must preserve the value without rounding.
 *   For example, \c to_float(1.5) succeeds; on a float with 24 binary precision digits, \c can_to_float(16777217)
 *   returns false because that integer cannot be represented exactly.
 *
 * NaN and infinity are rejected, including when source and destination have the same type. **Floating-To-Floating**
 * conversions preserve the sign of zero when the destination supports it. For example, with an IEEE 754 float
 * destination, \c to_float(-0.0) returns negative float zero, and \c std::signbit(to_float(-0.0)) returns true. If the
 * destination cannot represent negative zero, converting negative zero fails. Both signs of floating zero convert to
 * integer zero. Integer zero converts to positive floating zero. A failed exact conversion follows the failure
 * behavior below; it does not silently choose a rounded result.
 *
 * Select \c conversion::truncate only for **Floating-To-Integer** truncation toward zero. Select \c
 * conversion::nearest only for **Integer-To-Floating** or different-floating-type **Floating-To-Floating** conversion;
 * it rounds to nearest with ties to even. Omit the policy for exact-same-type conversions and **Integer-To-Integer**
 * conversions. Neither policy is accepted for character or byte conversions.
 *
 * The matching \c can_to* functions return true when the corresponding conversion would succeed, otherwise false, even
 * during constant evaluation.
 *
 * Unsupported types or policy combinations fail compilation with a cljonic-specific error message naming the operation
 * and explaining its permitted inputs and policies. This applies to both conversions and preflights: \c
 * can_to_float('A') does not return false; like \c to_float('A'), it fails compilation because char sources are not
 * supported for floating destinations. Likewise, \c to_int(3.5, conversion::nearest) fails compilation because nearest
 * rounding is not supported for integer destinations; use \c conversion::truncate to deliberately discard the
 * fractional part.
 *
 * **Supported Source and Destination Types**
 *
 * Here, "non-character integer" means short, int, long, long long, or their unsigned counterparts, including aliases
 * such as std::size_t. Plain char has its own conversion rules below. In C++, char, signed char, and unsigned char are
 * three distinct types: a platform where plain char is signed does not make it the type signed char.
 *
 * - **Non-Character Integer Destinations** (such as \c to_int and \c to_uint): accept non-character integers,
 *   supported binary floating-point values, and plain char. Every successful result preserves the supplied value
 *   unless an allowed floating-to-integer truncation policy is selected, and must fit the destination's range.
 *   For a char source, omit the policy: \c to_int('A') returns its native numeric character code (65 on
 *   ASCII-compatible systems), and \c to_int('\0') returns 0. This is not digit parsing: \c to_int('7') returns the
 *   character code, not the number 7. Negative plain-char values remain negative when converted to a signed integer;
 *   conversion to an unsigned integer fails rather than wrapping.
 * - **Floating Destinations** (such as \c to_float): accept non-character integers and supported binary
 *   floating-point values, but not char or std::byte.
 * - **Plain Char and Byte Destinations** (\c to_char and \c to_byte): accept non-character integers, plain char,
 *   and std::byte, subject to the value checks below; floating-point sources are not accepted.
 *
 * Bool, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t, application enums, pointers, and
 * user-defined types are not supported as sources or destinations by these conversion APIs. This exclusion does not
 * include plain char or std::byte, which are supported exactly as described above. Type aliases follow their
 * underlying type; for example, std::uint8_t is unsupported when it aliases unsigned char.
 *
 * The \c to_char free function accepts non-character integers, char, or std::byte and checks the value against \c
 * std::numeric_limits<char>::min() through \c std::numeric_limits<char>::max() before conversion. NUL and all
 * representable non-ASCII values are allowed; every char source is returned unchanged. An out-of-range value fails
 * rather than wrapping. The \c to_byte free function accepts non-character integers or plain char whose numeric value
 * is in 0..UCHAR_MAX, including zero, and returns the corresponding std::byte. Negative char values fail rather than
 * being reinterpreted as unsigned byte codes. A std::byte source is returned unchanged.
 *
 * These are low-level conversions, independent of cljonic's String character-validity policy. \c to_char(0) succeeds
 * with NUL, but \ref CharacterIsValid "character_is_valid" still returns false for NUL, and String still enforces its
 * non-NUL ASCII content range (1..127). Character and byte range failures use \c ConversionError::out_of_range.
 * Matching preflights check the same input rules, and generic \c to<T> and \c can_to<T> follow the same rules for
 * their selected destination.
 *
 * An invalid value of a supported type is different from an unsupported type or policy: \c can_to_int(3.5) compiles
 * and returns false because exact conversion would discard a fractional part; \c to_int(3.5) follows the conversion
 * failure behavior below.
 *
 * **Runtime Conversion Failure Modes**
 *
 * These conversion APIs have a compile-time configuration that selects what happens when a supported conversion fails
 * its value checks at runtime. This is not a general cljonic build mode or a per-call rounding policy: it controls
 * conversion failure handling only. Successful conversions and the input-validation rules are the same in both modes.
 *
 * - **Normal Mode (default):** leave \c CLJONIC_CONVERSION_DIAGNOSTICS undefined or define it as 0. A failed runtime
 *   conversion returns a fallback: the destination's maximum integer value, maximum finite floating value,
 *   all-bits-set std::byte, or NUL char. A fallback can also be a valid conversion result, so use the matching
 *   \c can_to* preflight when failure must be distinguished from success. No diagnostic error is stored, and
 *   \c cljonic::core::conversion_error() returns \c cljonic::core::ConversionError::none.
 * - **Diagnostic Mode:** define \c CLJONIC_CONVERSION_DIAGNOSTICS as 1 before including cljonic, or use the compiler
 *   definition \c -DCLJONIC_CONVERSION_DIAGNOSTICS=1. Use the same setting in every translation unit in the program.
 *   Your application must define \c [[noreturn]] \c void \c cljonic::core::conversion_failure_handler() \c noexcept.
 *   A failed runtime conversion records its \c cljonic::core::ConversionError, then calls this handler instead of
 *   returning a fallback. The handler must not return or throw; the application decides how to stop execution.
 *   The handler can inspect the recorded error through \c cljonic::core::conversion_error(). The library provides
 *   no default handler or error-reset operation; successful conversions do not clear a recorded error.
 *
 * Other values of \c CLJONIC_CONVERSION_DIAGNOSTICS are rejected at compile time. In either mode, preflights return
 * true or false without recording an error or invoking the handler; any previously recorded error is preserved.
 * Unsupported source types, destinations, or policies still fail compilation in either mode.
 *
 * These modes apply only to runtime evaluation. If a failed conversion is required to be evaluated at compile time,
 * for example in a constexpr variable initializer, compilation fails: no fallback is returned, no error is recorded,
 * and no handler is called. A failed value preflight in a constant expression simply returns false. Supplying a
 * literal argument alone does not require compile-time evaluation.
 *
 \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   static_assert(to<double>(42) == 42.0);
   static_assert(to_int(3.75, conversion::truncate) == 3);
   static_assert(to_float(16777217, conversion::nearest) == 16777216.0F);
   static_assert(to_char(65) == 'A');
   static_assert(to_int('A') == static_cast<int>('A'));
   static_assert(to_int('\0') == 0);
   static_assert(to_char(0) == '\0');
   static_assert(!can_to_int(3.75));

   const double value = 12.0;
   if (!can_to_int(value)) {
     return 1;
   }
   return to_int(value) == 12 ? 0 : 1;
 }
 ~~~~~
 *
 * This separate diagnostic-mode program deliberately attempts an inexact conversion. Its application-defined handler
 * checks the recorded error and stops the program with exit code 2 for precision loss, or 1 for another conversion
 * error. The call never returns a fallback. For a multi-file application, enable the same diagnostic setting in every
 * translation unit and define the handler in exactly one.
 ~~~~~{.cpp}
 #define CLJONIC_CONVERSION_DIAGNOSTICS 1
 #include "cljonic.hpp"
 #include <cstdlib>
 using namespace cljonic;
 using namespace cljonic::core;

 namespace cljonic::core {
 [[noreturn]] void conversion_failure_handler() noexcept {
   if (conversion_error() == ConversionError::precision_loss) {
     std::_Exit(2);
   }
   std::_Exit(1);
 }
 } // namespace cljonic::core

 int main() {
   const double measured_count = 3.75;
   const int count = to_int(measured_count);
   return count == 3 ? 0 : 1;
 }
 ~~~~~
 */

#ifndef CLJONIC_CONVERSION_DIAGNOSTICS
#define CLJONIC_CONVERSION_DIAGNOSTICS 0
#endif

#if CLJONIC_CONVERSION_DIAGNOSTICS != 0 && CLJONIC_CONVERSION_DIAGNOSTICS != 1
#error "cljonic: CLJONIC_CONVERSION_DIAGNOSTICS must be 0 or 1"
#endif

namespace cljonic::core {

enum class ConversionError {
    none,
    non_finite_input,
    invalid_character,
    unsupported_signed_zero,
    out_of_range,
    underflow_to_zero,
    precision_loss,
};

#if CLJONIC_CONVERSION_DIAGNOSTICS == 1
namespace conversion_detail {
inline ConversionError conversion_error_state = ConversionError::none;
}

[[noreturn]] void conversion_failure_handler() noexcept;
#endif

/** Return the last diagnostic conversion failure, or none in normal mode. */
[[nodiscard]] constexpr auto conversion_error() noexcept -> ConversionError {
#if CLJONIC_CONVERSION_DIAGNOSTICS == 1
    return conversion_detail::conversion_error_state;
#else
    return ConversionError::none;
#endif
}

namespace conversion {

struct TruncateTag final {};
struct NearestTag final {};

inline constexpr TruncateTag truncate{};
inline constexpr NearestTag nearest{};

} // namespace conversion

namespace conversion_detail {

struct ExactTag final {};

inline constexpr ExactTag exact{};

template <typename T>
using ValueType = std::remove_cvref_t<T>;

template <typename T>
inline constexpr bool is_character_v =
    std::same_as<ValueType<T>, char> || std::same_as<ValueType<T>, signed char> ||
    std::same_as<ValueType<T>, unsigned char> || std::same_as<ValueType<T>, wchar_t> ||
    std::same_as<ValueType<T>, char8_t> || std::same_as<ValueType<T>, char16_t> || std::same_as<ValueType<T>, char32_t>;

template <typename T>
concept Integer = std::is_integral_v<ValueType<T>> && !std::same_as<ValueType<T>, bool> && !is_character_v<T>;

template <typename T>
concept Floating = std::is_floating_point_v<ValueType<T>> && std::numeric_limits<ValueType<T>>::radix == 2 &&
                   std::numeric_limits<ValueType<T>>::has_infinity && std::numeric_limits<ValueType<T>>::has_quiet_NaN;

template <typename T>
concept Numeric = Integer<T> || Floating<T>;

template <Floating T>
struct BinaryMagnitude final {
    T significand;
    int exponent;
};

template <Floating T>
[[nodiscard]] constexpr auto binary_magnitude(T value) noexcept -> BinaryMagnitude<T> {
    int exponent{};
    T scaled = value;
    while (scaled >= T{2}) {
        scaled /= T{2};
        ++exponent;
    }
    while (scaled < T{1}) {
        scaled *= T{2};
        --exponent;
    }
    return {scaled, exponent};
}

template <typename T>
concept NumericDestination =
    Numeric<T> && !(std::same_as<ValueType<T>, signed char> || std::same_as<ValueType<T>, unsigned char>);

template <typename Destination, typename SourceType>
concept AdmittedSource =
    (NumericDestination<Destination> && Numeric<SourceType>) ||
    (Integer<Destination> && std::same_as<ValueType<SourceType>, char>) ||
    ((std::same_as<ValueType<Destination>, char> || std::same_as<ValueType<Destination>, std::byte>) &&
     (Integer<SourceType> || std::same_as<ValueType<SourceType>, char> ||
      std::same_as<ValueType<SourceType>, std::byte>));

template <typename Destination, typename SourceType, typename Policy>
inline constexpr bool policy_admitted_v = [] {
    using D = ValueType<Destination>;
    using S = ValueType<SourceType>;

    if constexpr (std::same_as<Policy, ExactTag>) {
        return AdmittedSource<D, S>;
    } else {
        return AdmittedSource<D, S> && Numeric<D> && Numeric<S> && !std::same_as<D, S> &&
               ((Integer<D> && Floating<S> && std::same_as<Policy, conversion::TruncateTag>) ||
                (Floating<D> && std::same_as<Policy, conversion::NearestTag>));
    }
}();

[[nodiscard]] constexpr auto power_of_two(int exponent) noexcept -> long double {
    long double result = 1.0L;
    for (int bit = 0; bit < exponent; ++bit) {
        result *= 2.0L;
    }
    return result;
}

template <typename Destination, std::integral SourceType>
[[nodiscard]] constexpr auto integer_in_range(SourceType value) noexcept -> bool {
    using CharInteger = std::conditional_t<std::is_signed_v<char>, signed char, unsigned char>;
    using RangeDestination = std::conditional_t<std::same_as<ValueType<Destination>, char>, CharInteger, Destination>;
    using RangeSource = std::conditional_t<std::same_as<ValueType<SourceType>, char>, CharInteger, SourceType>;
    return std::in_range<RangeDestination>(static_cast<RangeSource>(value));
}

template <typename T>
[[nodiscard]] constexpr auto is_finite(T value) noexcept -> bool {
    return value == value && value <= std::numeric_limits<T>::max() && value >= std::numeric_limits<T>::lowest();
}

template <typename T>
[[nodiscard]] constexpr auto signed_magnitude(T value) noexcept -> std::uintmax_t {
    using Unsigned = std::make_unsigned_t<T>;
    const auto unsigned_value = static_cast<Unsigned>(value);
    if constexpr (std::is_signed_v<T>) {
        if (value < 0) {
            return static_cast<std::uintmax_t>(Unsigned{0} - unsigned_value);
        }
    }
    return static_cast<std::uintmax_t>(unsigned_value);
}

template <typename IntegerType>
[[nodiscard]] constexpr auto compare_integer_values(IntegerType integer, IntegerType converted) noexcept -> int {
    if (integer < converted) {
        return -1;
    }
    if (integer > converted) {
        return 1;
    }
    return 0;
}

template <typename IntegerType>
[[nodiscard]] constexpr auto float_below_integer_lower_bound(long double candidate) noexcept -> bool {
    using I = ValueType<IntegerType>;
    const long double bound = power_of_two(std::numeric_limits<I>::digits);
    if constexpr (std::is_signed_v<I>) {
        return candidate < -bound;
    }
    return candidate < 0.0L;
}

template <typename IntegerType>
[[nodiscard]] constexpr auto compare_float_to_integer_bounds(long double candidate) noexcept -> int {
    using I = ValueType<IntegerType>;
    if (candidate >= power_of_two(std::numeric_limits<I>::digits)) {
        return -1;
    }
    return float_below_integer_lower_bound<I>(candidate) ? 1 : 0;
}

template <typename IntegerType, Floating FloatType>
[[nodiscard]] constexpr auto compare_integer_to_float(IntegerType integer, FloatType floating) noexcept -> int {
    const auto candidate = static_cast<long double>(floating);
    const int bounds_comparison = compare_float_to_integer_bounds<IntegerType>(candidate);
    if (bounds_comparison != 0) {
        return bounds_comparison;
    }
    return compare_integer_values(integer, static_cast<ValueType<IntegerType>>(floating));
}

[[nodiscard]] constexpr auto unsigned_distance_from_bound(std::uintmax_t magnitude, int digits) noexcept
    -> std::uintmax_t {
    if (digits < std::numeric_limits<std::uintmax_t>::digits) {
        return (std::uintmax_t{1} << static_cast<unsigned>(digits)) - magnitude;
    }
    const auto distance_minus_one = std::numeric_limits<std::uintmax_t>::max() - magnitude;
    return distance_minus_one == std::numeric_limits<std::uintmax_t>::max() ? distance_minus_one
                                                                            : distance_minus_one + 1U;
}

template <typename IntegerType>
[[nodiscard]] constexpr auto distance_to_upper_bound(IntegerType integer) noexcept -> std::uintmax_t {
    using I = ValueType<IntegerType>;
    if constexpr (std::is_signed_v<I>) {
        if (integer < 0) {
            return std::numeric_limits<std::uintmax_t>::max();
        }
    }
    return unsigned_distance_from_bound(signed_magnitude(integer), std::numeric_limits<I>::digits);
}

[[nodiscard]] constexpr auto unsigned_integer_distance(std::uintmax_t lhs, std::uintmax_t rhs) noexcept
    -> std::uintmax_t {
    return lhs > rhs ? lhs - rhs : rhs - lhs;
}

template <typename IntegerType>
[[nodiscard]] constexpr auto distance_between_integers(IntegerType lhs, IntegerType rhs) noexcept -> std::uintmax_t {
    using I = ValueType<IntegerType>;
    if constexpr (std::is_signed_v<I>) {
        const auto lhs_negative = lhs < 0;
        const auto rhs_negative = rhs < 0;
        const auto lhs_magnitude = signed_magnitude(lhs);
        const auto rhs_magnitude = signed_magnitude(rhs);
        if (lhs_negative != rhs_negative) {
            return lhs_magnitude + rhs_magnitude;
        }
        return unsigned_integer_distance(lhs_magnitude, rhs_magnitude);
    }
    return unsigned_integer_distance(static_cast<std::uintmax_t>(lhs), static_cast<std::uintmax_t>(rhs));
}

template <typename IntegerType>
[[nodiscard]] constexpr auto integer_candidate_below_range(long double candidate) noexcept -> bool {
    using I = ValueType<IntegerType>;
    if constexpr (std::is_signed_v<I>) {
        return candidate < -power_of_two(std::numeric_limits<I>::digits);
    }
    return candidate < 0.0L;
}

template <typename IntegerType, Floating FloatType>
[[nodiscard]] constexpr auto integer_distance(IntegerType integer, FloatType floating) noexcept -> std::uintmax_t {
    using I = ValueType<IntegerType>;
    const auto candidate = static_cast<long double>(floating);
    const long double bound = power_of_two(std::numeric_limits<I>::digits);

    if (candidate >= bound) {
        return distance_to_upper_bound(integer);
    }
    if (integer_candidate_below_range<IntegerType>(candidate)) {
        return std::numeric_limits<std::uintmax_t>::max();
    }

    return distance_between_integers(integer, static_cast<I>(floating));
}

template <Floating T>
[[nodiscard]] constexpr auto binary_spacing(T value) noexcept -> T {
    const auto magnitude = binary_magnitude(value);
    T spacing{1};
    const int shift = magnitude.exponent - (std::numeric_limits<T>::digits - 1);
    for (int bit = 0; bit < shift; ++bit) {
        spacing *= T{2};
    }
    for (int bit = 0; bit > shift; --bit) {
        spacing /= T{2};
    }
    return spacing;
}

template <Floating T>
[[nodiscard]] constexpr auto is_power_of_two(T value) noexcept -> bool {
    return binary_magnitude(value).significand == T{1};
}

template <Floating T>
[[nodiscard]] constexpr auto next_positive(T value) noexcept -> T {
    if (value == std::numeric_limits<T>::max()) {
        return std::numeric_limits<T>::infinity();
    }
    if (value < std::numeric_limits<T>::min()) {
        if constexpr (std::numeric_limits<T>::has_denorm == std::denorm_present) {
            return value + std::numeric_limits<T>::denorm_min();
        } else {
            return std::numeric_limits<T>::min();
        }
    }
    return value + binary_spacing(value);
}

template <Floating T>
[[nodiscard]] constexpr auto previous_positive(T value) noexcept -> T {
    if (value <= std::numeric_limits<T>::min()) {
        if constexpr (std::numeric_limits<T>::has_denorm == std::denorm_present) {
            return value - std::numeric_limits<T>::denorm_min();
        } else {
            return T{0};
        }
    }
    T spacing = binary_spacing(value);
    if (is_power_of_two(value)) {
        spacing /= T{2};
    }
    return value - spacing;
}

template <Floating T>
[[nodiscard]] constexpr auto next_up(T value) noexcept -> T {
    if (value < T{0}) {
        return -previous_positive(-value);
    }
    if (value == T{0}) {
        if constexpr (std::numeric_limits<T>::has_denorm == std::denorm_present) {
            return std::numeric_limits<T>::denorm_min();
        } else {
            return std::numeric_limits<T>::min();
        }
    }
    return next_positive(value);
}

template <Floating T>
[[nodiscard]] constexpr auto next_down(T value) noexcept -> T {
    if (value > T{0}) {
        return previous_positive(value);
    }
    if (value == T{0}) {
        if constexpr (std::numeric_limits<T>::has_denorm == std::denorm_present) {
            return -std::numeric_limits<T>::denorm_min();
        } else {
            return -std::numeric_limits<T>::min();
        }
    }
    return -next_positive(-value);
}

template <Floating T>
[[nodiscard]] constexpr auto even_significand(T value) noexcept -> bool {
    if (value == T{0}) {
        return true;
    }

    const T magnitude = value < T{0} ? -value : value;
    T adjacent = next_up(magnitude);
    if (!is_finite(adjacent)) {
        adjacent = previous_positive(magnitude);
    }
    const long double spacing = static_cast<long double>(adjacent) - static_cast<long double>(magnitude);
    const long double significand = static_cast<long double>(magnitude) / spacing;
    return std::fmod(significand, 2.0L) == 0.0L;
}

template <typename T>
struct Result {
    bool succeeds{};
    T value{};
    ConversionError error{ConversionError::none};
};

template <typename T>
[[nodiscard]] constexpr auto success(T value) noexcept -> Result<T> {
    return {true, value, ConversionError::none};
}

template <typename T>
[[nodiscard]] constexpr auto failure(ConversionError error) noexcept -> Result<T> {
    return {false, T{}, error};
}

template <Integer Destination>
[[nodiscard]] constexpr auto exact_float_in_integer_range(long double value) noexcept -> bool {
    const long double bound = power_of_two(std::numeric_limits<Destination>::digits);
    if constexpr (std::is_signed_v<Destination>) {
        return value >= -bound && value < bound;
    }
    return value >= 0.0L && value < bound;
}

[[nodiscard]] constexpr auto signed_truncated_value_in_range(long double value, long double bound) noexcept -> bool {
    if (value >= bound) {
        return false;
    }
    if (value >= -bound) {
        return true;
    }
    return value + bound > -1.0L;
}

template <Integer Destination>
[[nodiscard]] constexpr auto truncated_float_in_integer_range(long double value) noexcept -> bool {
    const long double bound = power_of_two(std::numeric_limits<Destination>::digits);
    if constexpr (std::is_signed_v<Destination>) {
        return signed_truncated_value_in_range(value, bound);
    }
    return value > -1.0L && value < bound;
}

template <Floating T>
struct FloatingBracket final {
    T lower;
    T upper;
};

template <Floating T>
[[nodiscard]] constexpr auto integer_float_bracket(T candidate, int comparison) noexcept -> FloatingBracket<T> {
    const T lower = comparison > 0 ? candidate : next_down(candidate);
    const T upper = comparison < 0 ? candidate : next_up(candidate);
    return {lower, upper};
}

template <Integer IntegerType, Floating FloatType>
[[nodiscard]] constexpr auto nearest_integer_float(IntegerType source, FloatType lower, FloatType upper) noexcept
    -> FloatType {
    const auto lower_distance = integer_distance(source, lower);
    const auto upper_distance = integer_distance(source, upper);
    if (lower_distance < upper_distance) {
        return lower;
    }
    if (upper_distance < lower_distance) {
        return upper;
    }
    return even_significand(lower) ? lower : upper;
}

template <typename Destination, std::integral SourceType>
[[nodiscard]] constexpr auto convert_integer_to_integer(SourceType source) noexcept -> Result<Destination> {
    if (!integer_in_range<Destination>(source)) {
        return failure<Destination>(ConversionError::out_of_range);
    }
    return success(static_cast<Destination>(source));
}

template <Floating Destination, Integer SourceType>
[[nodiscard]] constexpr auto convert_integer_to_floating(SourceType source, ExactTag policy) noexcept
    -> Result<Destination> {
    static_cast<void>(policy);
    const auto converted = static_cast<Destination>(source);
    if (compare_integer_to_float(source, converted) != 0) {
        return failure<Destination>(ConversionError::precision_loss);
    }
    return success(converted);
}

template <Floating Destination, Integer SourceType>
[[nodiscard]] constexpr auto convert_integer_to_floating(SourceType source, conversion::NearestTag policy) noexcept
    -> Result<Destination> {
    static_cast<void>(policy);
    const auto candidate = static_cast<Destination>(source);
    const int comparison = compare_integer_to_float(source, candidate);
    if (comparison == 0) {
        return success(candidate);
    }
    const auto bracket = integer_float_bracket(candidate, comparison);
    return success(nearest_integer_float(source, bracket.lower, bracket.upper));
}

template <Integer Destination, Floating SourceType>
[[nodiscard]] constexpr auto convert_floating_to_integer(SourceType source, ExactTag policy) noexcept
    -> Result<Destination> {
    static_cast<void>(policy);
    if (!is_finite(source)) {
        return failure<Destination>(ConversionError::non_finite_input);
    }

    const auto value = static_cast<long double>(source);
    if (!exact_float_in_integer_range<Destination>(value)) {
        return failure<Destination>(ConversionError::out_of_range);
    }
    if (std::trunc(source) != source) {
        return failure<Destination>(ConversionError::precision_loss);
    }

    const auto converted = static_cast<Destination>(source);
    return success(converted);
}

template <Integer Destination, Floating SourceType>
[[nodiscard]] constexpr auto convert_floating_to_integer(SourceType source, conversion::TruncateTag policy) noexcept
    -> Result<Destination> {
    static_cast<void>(policy);
    if (!is_finite(source)) {
        return failure<Destination>(ConversionError::non_finite_input);
    }

    const auto value = static_cast<long double>(source);
    if (!truncated_float_in_integer_range<Destination>(value)) {
        return failure<Destination>(ConversionError::out_of_range);
    }
    return success(static_cast<Destination>(source));
}

template <Floating Destination, Floating SourceType>
[[nodiscard]] constexpr auto convert_floating_zero(SourceType source) noexcept -> Result<Destination> {
    if constexpr (!std::signbit(-Destination{0})) {
        if (std::signbit(source)) {
            return failure<Destination>(ConversionError::unsupported_signed_zero);
        }
    }
    return success(std::signbit(source) ? -Destination{0} : Destination{0});
}

template <Floating Destination>
[[nodiscard]] constexpr auto floating_value_in_range(long double value) noexcept -> bool {
    return value <= static_cast<long double>(std::numeric_limits<Destination>::max()) &&
           value >= static_cast<long double>(std::numeric_limits<Destination>::lowest());
}

template <Floating Destination, Floating SourceType>
[[nodiscard]] constexpr auto convert_nonzero_floating_exact(SourceType source, long double value) noexcept
    -> Result<Destination> {
    if (!floating_value_in_range<Destination>(value)) {
        return failure<Destination>(ConversionError::out_of_range);
    }
    const auto converted = static_cast<Destination>(source);
    if (static_cast<SourceType>(converted) != source) {
        return failure<Destination>(ConversionError::precision_loss);
    }
    return success(converted);
}

template <Floating T>
struct FloatingNeighbors final {
    T lower;
    T upper;
    bool exact;
};

template <Floating Destination>
[[nodiscard]] constexpr auto floating_neighbors(Destination candidate, long double value) noexcept
    -> FloatingNeighbors<Destination> {
    const auto rounded = static_cast<long double>(candidate);
    if (rounded < value) {
        return {candidate, next_up(candidate), false};
    }
    if (rounded > value) {
        return {next_down(candidate), candidate, false};
    }
    return {candidate, candidate, true};
}

template <Floating Destination>
[[nodiscard]] constexpr auto nearest_floating_value(Destination lower, Destination upper, long double value) noexcept
    -> Destination {
    const long double lower_distance = value - static_cast<long double>(lower);
    const long double upper_distance = static_cast<long double>(upper) - value;
    if (lower_distance < upper_distance) {
        return lower;
    }
    if (upper_distance < lower_distance) {
        return upper;
    }
    return even_significand(lower) ? lower : upper;
}

template <Floating Destination, Floating SourceType>
[[nodiscard]] constexpr auto convert_nonzero_floating_nearest(SourceType source, long double value) noexcept
    -> Result<Destination> {
    if (!floating_value_in_range<Destination>(value)) {
        return failure<Destination>(ConversionError::out_of_range);
    }
    const auto candidate = static_cast<Destination>(source);
    const auto neighbors = floating_neighbors(candidate, value);
    const auto nearest =
        neighbors.exact ? neighbors.lower : nearest_floating_value(neighbors.lower, neighbors.upper, value);
    if (nearest == Destination{0}) {
        return failure<Destination>(ConversionError::underflow_to_zero);
    }
    return success(nearest);
}

template <Floating Destination, Floating SourceType>
[[nodiscard]] constexpr auto convert_floating_to_floating(SourceType source, ExactTag policy) noexcept
    -> Result<Destination> {
    static_cast<void>(policy);
    if (!is_finite(source)) {
        return failure<Destination>(ConversionError::non_finite_input);
    }
    if (source == SourceType{0}) {
        return convert_floating_zero<Destination>(source);
    }
    return convert_nonzero_floating_exact<Destination>(source, static_cast<long double>(source));
}

template <Floating Destination, Floating SourceType>
[[nodiscard]] constexpr auto convert_floating_to_floating(SourceType source, conversion::NearestTag policy) noexcept
    -> Result<Destination> {
    static_cast<void>(policy);
    if (!is_finite(source)) {
        return failure<Destination>(ConversionError::non_finite_input);
    }
    if (source == SourceType{0}) {
        return convert_floating_zero<Destination>(source);
    }
    return convert_nonzero_floating_nearest<Destination>(source, static_cast<long double>(source));
}

template <std::integral SourceType>
[[nodiscard]] constexpr auto convert_integer_to_char(SourceType source) noexcept -> Result<char> {
    return convert_integer_to_integer<char>(source);
}

template <typename SourceType>
[[nodiscard]] constexpr auto convert_character_to_char(SourceType source) noexcept -> Result<char> {
    if constexpr (std::same_as<ValueType<SourceType>, char>) {
        return success(source);
    } else {
        return convert_integer_to_char(std::to_integer<unsigned int>(source));
    }
}

template <std::integral SourceType>
[[nodiscard]] constexpr auto convert_integer_to_byte(SourceType source) noexcept -> Result<std::byte> {
    if constexpr (std::is_signed_v<SourceType>) {
        if (source < 0) {
            return failure<std::byte>(ConversionError::out_of_range);
        }
    }
    if (static_cast<std::uintmax_t>(source) > UCHAR_MAX) {
        return failure<std::byte>(ConversionError::out_of_range);
    }
    return success(static_cast<std::byte>(static_cast<unsigned char>(source)));
}

template <typename SourceType>
[[nodiscard]] constexpr auto convert_character_to_byte(SourceType source) noexcept -> Result<std::byte> {
    return convert_integer_to_byte(source);
}

template <typename SourceType>
[[nodiscard]] constexpr auto convert_to_char(SourceType source) noexcept -> Result<char> {
    if constexpr (Integer<SourceType>) {
        return convert_integer_to_char(source);
    } else {
        return convert_character_to_char(source);
    }
}

template <typename SourceType>
[[nodiscard]] constexpr auto convert_to_byte(SourceType source) noexcept -> Result<std::byte> {
    if constexpr (Integer<SourceType>) {
        return convert_integer_to_byte(source);
    } else if constexpr (std::same_as<ValueType<SourceType>, char>) {
        return convert_character_to_byte(source);
    } else {
        return success(source);
    }
}

template <typename Destination, typename SourceType>
[[nodiscard]] constexpr auto convert_character_or_byte(SourceType source) noexcept -> Result<Destination> {
    if constexpr (std::same_as<ValueType<Destination>, char>) {
        return convert_to_char(source);
    } else {
        return convert_to_byte(source);
    }
}

template <Integer Destination, typename SourceType, typename Policy>
[[nodiscard]] constexpr auto convert_to_integer(SourceType source, Policy policy) noexcept -> Result<Destination> {
    if constexpr (std::integral<SourceType>) {
        return convert_integer_to_integer<Destination>(source);
    } else {
        return convert_floating_to_integer<Destination>(source, policy);
    }
}

template <Floating Destination, typename SourceType, typename Policy>
[[nodiscard]] constexpr auto convert_to_floating(SourceType source, Policy policy) noexcept -> Result<Destination> {
    if constexpr (Integer<SourceType>) {
        return convert_integer_to_floating<Destination>(source, policy);
    } else {
        return convert_floating_to_floating<Destination>(source, policy);
    }
}

template <typename Destination, typename SourceType, typename Policy>
[[nodiscard]] constexpr auto convert_numeric(SourceType source, Policy policy) noexcept -> Result<Destination> {
    if constexpr (Integer<Destination>) {
        return convert_to_integer<Destination>(source, policy);
    } else {
        return convert_to_floating<Destination>(source, policy);
    }
}

template <typename Destination, typename SourceType, typename Policy>
[[nodiscard]] constexpr auto convert(SourceType source, Policy policy) noexcept -> Result<ValueType<Destination>> {
    using D = ValueType<Destination>;
    if constexpr (std::same_as<D, char> || std::same_as<D, std::byte>) {
        return convert_character_or_byte<D>(source);
    } else {
        return convert_numeric<D>(source, policy);
    }
}

[[noreturn]] void reject_constant_conversion_failure() noexcept;

template <typename T>
[[nodiscard]] constexpr auto fallback() noexcept -> T {
    if constexpr (std::same_as<T, char>) {
        return '\0';
    } else if constexpr (std::same_as<T, std::byte>) {
        return static_cast<std::byte>(UCHAR_MAX);
    } else {
        return std::numeric_limits<T>::max();
    }
}

inline auto runtime_failure(ConversionError error) noexcept -> void {
#if CLJONIC_CONVERSION_DIAGNOSTICS == 1
    conversion_detail::conversion_error_state = error;
    conversion_failure_handler();
#else
    static_cast<void>(error);
#endif
}

template <typename Destination, typename SourceType, typename Policy>
[[nodiscard]] constexpr auto convert_or_fallback(SourceType source, Policy policy) noexcept -> ValueType<Destination> {
    const auto result = convert<Destination>(source, policy);
    if (result.succeeds) {
        return result.value;
    }

    if consteval {
        reject_constant_conversion_failure();
    } else {
        runtime_failure(result.error);
        return fallback<ValueType<Destination>>();
    }
}

template <typename Destination, typename SourceType, typename Policy>
concept ValidConversion = AdmittedSource<Destination, SourceType> && policy_admitted_v<Destination, SourceType, Policy>;

template <typename...>
inline constexpr bool dependent_false = false;

} // namespace conversion_detail

template <typename Destination, typename SourceType, typename Policy = conversion_detail::ExactTag>
    requires conversion_detail::ValidConversion<Destination, SourceType, Policy>
[[nodiscard]] constexpr auto to(SourceType&& source, Policy policy = {}) noexcept
    -> conversion_detail::ValueType<Destination> {
    return conversion_detail::convert_or_fallback<Destination>(std::forward<SourceType>(source), policy);
}

template <typename Destination, typename SourceType, typename Policy = conversion_detail::ExactTag>
    requires conversion_detail::ValidConversion<Destination, SourceType, Policy>
[[nodiscard]] constexpr auto can_to(SourceType&& source, Policy policy = {}) noexcept -> bool {
    return conversion_detail::convert<Destination>(std::forward<SourceType>(source), policy).succeeds;
}

template <typename Destination, typename SourceType, typename Policy = conversion_detail::ExactTag>
    requires(!conversion_detail::ValidConversion<Destination, SourceType, Policy>)
constexpr auto to([[maybe_unused]] SourceType&& source, [[maybe_unused]] Policy policy = {}) noexcept -> void {
    static_assert(conversion_detail::dependent_false<Destination, SourceType, Policy>,
                  "cljonic::to: expected a supported numeric, char, or std::byte destination and source. "
                  "Numeric conversions exclude bool and character sources except plain char to integer; char and byte "
                  "conversions accept only "
                  "non-character integers, char, or std::byte. Use truncate only for floating-to-integer and nearest "
                  "only for integer-to-floating or different floating types; otherwise omit the policy.");
}

template <typename Destination, typename SourceType, typename Policy = conversion_detail::ExactTag>
    requires(!conversion_detail::ValidConversion<Destination, SourceType, Policy>)
constexpr auto can_to([[maybe_unused]] SourceType&& source, [[maybe_unused]] Policy policy = {}) noexcept -> void {
    static_assert(conversion_detail::dependent_false<Destination, SourceType, Policy>,
                  "cljonic::can_to: expected the same supported destination, source, and policy as to<T>. "
                  "Numeric conversions exclude bool and character sources except plain char to integer; char and byte "
                  "conversions accept only "
                  "non-character integers, char, or std::byte. Use truncate only for floating-to-integer and nearest "
                  "only for integer-to-floating or different floating types; otherwise omit the policy.");
}

#define CLJONIC_NAMED_CONVERSION(name, destination, label, preflight_label)                                            \
    template <typename SourceType, typename Policy = conversion_detail::ExactTag>                                      \
        requires conversion_detail::ValidConversion<destination, SourceType, Policy>                                   \
    [[nodiscard]] constexpr auto name(SourceType&& source, Policy policy = {}) noexcept -> destination {               \
        return conversion_detail::convert_or_fallback<destination>(std::forward<SourceType>(source), policy);          \
    }                                                                                                                  \
    template <typename SourceType, typename Policy = conversion_detail::ExactTag>                                      \
        requires conversion_detail::ValidConversion<destination, SourceType, Policy>                                   \
    [[nodiscard]] constexpr auto can_##name(SourceType&& source, Policy policy = {}) noexcept -> bool {                \
        return conversion_detail::convert<destination>(std::forward<SourceType>(source), policy).succeeds;             \
    }                                                                                                                  \
    template <typename SourceType, typename Policy = conversion_detail::ExactTag>                                      \
        requires(!conversion_detail::ValidConversion<destination, SourceType, Policy>)                                 \
    constexpr auto name([[maybe_unused]] SourceType&& source, [[maybe_unused]] Policy policy = {}) noexcept -> void {  \
        static_assert(                                                                                                 \
            conversion_detail::dependent_false<SourceType, Policy>,                                                    \
            label "expected a source and policy supported by to<" #destination ">. "                                   \
                  "Numeric conversions exclude bool and character sources except plain char to integer; char and "     \
                  "byte conversions accept "                                                                           \
                  "only non-character integers, char, or std::byte. Use truncate only for floating-to-integer "        \
                  "and nearest only for integer-to-floating or different floating types; otherwise omit the policy."); \
    }                                                                                                                  \
    template <typename SourceType, typename Policy = conversion_detail::ExactTag>                                      \
        requires(!conversion_detail::ValidConversion<destination, SourceType, Policy>)                                 \
    constexpr auto can_##name([[maybe_unused]] SourceType&& source, [[maybe_unused]] Policy policy = {}) noexcept      \
        -> void {                                                                                                      \
        static_assert(                                                                                                 \
            conversion_detail::dependent_false<SourceType, Policy>, preflight_label                                    \
            "expected the same source and policy as " #name ". "                                                       \
            "Numeric conversions exclude bool and character sources except plain char to integer; char and byte "      \
            "conversions accept "                                                                                      \
            "only non-character integers, char, or std::byte. Use truncate only for floating-to-integer "              \
            "and nearest only for integer-to-floating or different floating types; otherwise omit the policy.");       \
    }

CLJONIC_NAMED_CONVERSION(to_byte, std::byte, "cljonic::to_byte: ", "cljonic::can_to_byte: ")
CLJONIC_NAMED_CONVERSION(to_char, char, "cljonic::to_char: ", "cljonic::can_to_char: ")
CLJONIC_NAMED_CONVERSION(to_count, std::size_t, "cljonic::to_count: ", "cljonic::can_to_count: ")
CLJONIC_NAMED_CONVERSION(to_double, double, "cljonic::to_double: ", "cljonic::can_to_double: ")
CLJONIC_NAMED_CONVERSION(to_float, float, "cljonic::to_float: ", "cljonic::can_to_float: ")
CLJONIC_NAMED_CONVERSION(to_index, std::size_t, "cljonic::to_index: ", "cljonic::can_to_index: ")
CLJONIC_NAMED_CONVERSION(to_int, int, "cljonic::to_int: ", "cljonic::can_to_int: ")
CLJONIC_NAMED_CONVERSION(to_long, long, "cljonic::to_long: ", "cljonic::can_to_long: ")
CLJONIC_NAMED_CONVERSION(to_long_double, long double, "cljonic::to_long_double: ", "cljonic::can_to_long_double: ")
CLJONIC_NAMED_CONVERSION(to_long_long, long long, "cljonic::to_long_long: ", "cljonic::can_to_long_long: ")
CLJONIC_NAMED_CONVERSION(to_short, short, "cljonic::to_short: ", "cljonic::can_to_short: ")
CLJONIC_NAMED_CONVERSION(to_size, std::size_t, "cljonic::to_size: ", "cljonic::can_to_size: ")
CLJONIC_NAMED_CONVERSION(to_uint, unsigned int, "cljonic::to_uint: ", "cljonic::can_to_uint: ")
CLJONIC_NAMED_CONVERSION(to_ulong, unsigned long, "cljonic::to_ulong: ", "cljonic::can_to_ulong: ")
CLJONIC_NAMED_CONVERSION(to_ulong_long, unsigned long long, "cljonic::to_ulong_long: ", "cljonic::can_to_ulong_long: ")
CLJONIC_NAMED_CONVERSION(to_ushort, unsigned short, "cljonic::to_ushort: ", "cljonic::can_to_ushort: ")

#undef CLJONIC_NAMED_CONVERSION

} // namespace cljonic::core
