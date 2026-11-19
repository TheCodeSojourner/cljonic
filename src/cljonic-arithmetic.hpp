#pragma once

#include <bit>
#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>
#include <utility>

/** \file cljonic-arithmetic.hpp
 * \anchor CheckedArithmetic
 * Checked fixed-width numeric free functions.
 *
 * Arithmetic operands are accepted only when converting or combining them cannot silently lose data: at compile time,
 * integer operands determine a shared return type that can represent every value from both operand types; floating
 * operands must have exactly the same supported floating type, and that type is used as the return type. Boolean,
 * character, byte, enum, pointer, user-defined, and mixed integer/floating operand pairs are rejected. Integer and
 * floating arithmetic reject overflow, underflow, division by zero, and non-finite results.
 *
 * Every operation with a runtime failure condition has a matching \c can_* preflight. A false preflight or a runtime
 * failure returns the value-initialized result type, leaving the operands unchanged. For a numeric result type, this
 * value is \c 0. A failure in required constant evaluation rejects the expression.
 *
 * Integer division follows Clojure-compatible semantics:
 * - \c div and \c quot truncate toward zero.
 * - \c rem has the numerator's sign.
 * - \c mod divides and rounds the quotient toward negative infinity, producing a remainder with the denominator's
 *   sign.
 *
 * For example, \c quot(-7, 3) returns -2, \c rem(-7, 3) returns -1, and \c mod(-7, 3) returns 2. The bitwise
 * operations preserve fixed-width representation; shifts outside the value width and overflowing left shifts fail
 * preflight.
 *
 * \c add provides Clojure-compatible arities: \c add<T>() returns \c T{}, unary \c add(x) returns x, binary \c add(x,
 * y) adds two operands, and variadic \c add(x, y, &more) folds from left to right. \c can_add provides the
 * corresponding preflight for the same arities.
 *
 * \c sub provides unary checked negation \c sub(x), which returns the negated value of \c x; binary subtraction \c
 * sub(x,y), which returns \c x minus \c y; and left-to-right variadic subtraction \c sub(x,y,&more), which folds the
 * operands from left to right. \c sub() is rejected. \c can_sub provides the matching preflight for the unary, binary,
 * and variadic forms.
 *
 * \c mult provides a typed one identity \c mult<T>(), which returns the value \c T{1}; unary identity \c mult(x),
 * which returns \c x; binary multiplication \c mult(x,y), which returns the product; and left-to-right variadic
 * multiplication \c mult(x,y,&more), which folds the operands from left to right.
 *
 * \c div provides a unary floating reciprocal \c div(x), which returns \c 1/x; binary division \c div(x,y), which
 * returns \c x divided by \c y; and left-to-right variadic division \c div(x,y,&more), which folds the operands from
 * left to right. Unary integer division is rejected. \c can_div provides the matching side-effect-free preflight.
 */

namespace cljonic::core {

namespace arithmetic_detail {

template <typename T>
inline constexpr bool character_type_v =
    std::same_as<std::remove_cvref_t<T>, char> || std::same_as<std::remove_cvref_t<T>, signed char> ||
    std::same_as<std::remove_cvref_t<T>, unsigned char> || std::same_as<std::remove_cvref_t<T>, wchar_t> ||
    std::same_as<std::remove_cvref_t<T>, char8_t> || std::same_as<std::remove_cvref_t<T>, char16_t> ||
    std::same_as<std::remove_cvref_t<T>, char32_t>;

template <typename T>
concept SupportedInteger =
    std::integral<std::remove_cvref_t<T>> && !std::same_as<std::remove_cvref_t<T>, bool> && !character_type_v<T>;

template <typename T>
concept SupportedFloating = std::floating_point<std::remove_cvref_t<T>>;

template <typename Candidate, typename Source>
inline constexpr bool contains_integer_domain_v = [] {
    if constexpr (std::signed_integral<Source>) {
        return std::signed_integral<Candidate> &&
               std::numeric_limits<Candidate>::digits >= std::numeric_limits<Source>::digits;
    } else if constexpr (std::unsigned_integral<Candidate>) {
        return std::numeric_limits<Candidate>::digits >= std::numeric_limits<Source>::digits;
    } else {
        return std::signed_integral<Candidate> &&
               std::numeric_limits<Candidate>::digits >= std::numeric_limits<Source>::digits;
    }
}();

struct no_common_integer_type {};

template <typename Left, typename Right, typename... Candidates>
struct common_integer_selector;

template <typename Left, typename Right>
struct common_integer_selector<Left, Right> {
    using type = no_common_integer_type;
};

template <typename Left, typename Right, typename Candidate, typename... Candidates>
struct common_integer_selector<Left, Right, Candidate, Candidates...> {
    using type =
        std::conditional_t<contains_integer_domain_v<Candidate, Left> && contains_integer_domain_v<Candidate, Right>,
                           Candidate, typename common_integer_selector<Left, Right, Candidates...>::type>;
};

template <typename Left, typename Right>
using integer_result_t = std::conditional_t<
    std::same_as<std::remove_cvref_t<Left>, std::remove_cvref_t<Right>>, std::remove_cvref_t<Left>,
    typename common_integer_selector<std::remove_cvref_t<Left>, std::remove_cvref_t<Right>, short, unsigned short, int,
                                     unsigned int, long, unsigned long, long long, unsigned long long>::type>;

template <typename Left, typename Right>
concept IntegerArithmeticPair = SupportedInteger<Left> && SupportedInteger<Right> &&
                                !std::same_as<integer_result_t<Left, Right>, no_common_integer_type>;

template <typename Left, typename Right>
concept FloatingArithmeticPair = SupportedFloating<Left> && SupportedFloating<Right> &&
                                 std::same_as<std::remove_cvref_t<Left>, std::remove_cvref_t<Right>>;

template <typename Left, typename Right>
concept ArithmeticPair = IntegerArithmeticPair<Left, Right> || FloatingArithmeticPair<Left, Right>;

template <typename Left, typename Right>
using result_t =
    std::conditional_t<IntegerArithmeticPair<Left, Right>, integer_result_t<Left, Right>, std::remove_cvref_t<Left>>;

template <typename... Types>
struct common_arithmetic_pack_result;

template <typename T, typename... Rest>
using common_arithmetic_pack_tail_t = common_arithmetic_pack_result<T, Rest...>::type;

template <typename T>
struct common_arithmetic_pack_result<T> {
    using type =
        std::conditional_t<SupportedInteger<T> || SupportedFloating<T>, std::remove_cvref_t<T>, no_common_integer_type>;
};

template <typename First, typename Second, typename... Rest>
struct common_arithmetic_pack_result<First, Second, Rest...> {
    using pair_type = std::conditional_t<
        IntegerArithmeticPair<First, Second>, integer_result_t<First, Second>,
        std::conditional_t<FloatingArithmeticPair<First, Second>, std::remove_cvref_t<First>, no_common_integer_type>>;
    using type = std::conditional_t<std::same_as<pair_type, no_common_integer_type>, no_common_integer_type,
                                    common_arithmetic_pack_tail_t<pair_type, Rest...>>;
};

template <typename... Types>
using arithmetic_pack_result_t = common_arithmetic_pack_result<Types...>::type;

template <typename... Types>
concept IntegerArithmeticPack = sizeof...(Types) > 0 && (SupportedInteger<Types> && ...) &&
                                !std::same_as<arithmetic_pack_result_t<Types...>, no_common_integer_type>;

template <typename... Types>
concept FloatingArithmeticPack = sizeof...(Types) > 0 && (SupportedFloating<Types> && ...) &&
                                 !std::same_as<arithmetic_pack_result_t<Types...>, no_common_integer_type>;

template <typename... Types>
concept ArithmeticPack = IntegerArithmeticPack<Types...> || FloatingArithmeticPack<Types...>;

template <typename T>
struct arithmetic_result {
    bool succeeds;
    T value;
};

template <typename T>
[[nodiscard]] constexpr auto failure() noexcept -> arithmetic_result<T> {
    return {false, T{}};
}

template <typename T>
[[nodiscard]] constexpr auto success(T value) noexcept -> arithmetic_result<T> {
    return {true, value};
}

[[noreturn]] void reject_constant_arithmetic_failure() noexcept;

template <typename T>
[[nodiscard]] constexpr auto success_or_floating_failure(T value) noexcept -> arithmetic_result<T>;

template <std::signed_integral T>
[[nodiscard]] constexpr auto add_signed_nonnegative(T left, T right) noexcept -> arithmetic_result<T> {
    return left > std::numeric_limits<T>::max() - right ? failure<T>() : success<T>(left + right);
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto add_signed_negative(T left, T right) noexcept -> arithmetic_result<T> {
    return left < std::numeric_limits<T>::lowest() - right ? failure<T>() : success<T>(left + right);
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto add_values(T left, T right) noexcept -> arithmetic_result<T> {
    if (right >= 0) {
        return add_signed_nonnegative(left, right);
    }
    return add_signed_negative(left, right);
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto add_values(T left, T right) noexcept -> arithmetic_result<T> {
    return left > std::numeric_limits<T>::max() - right ? failure<T>() : success<T>(left + right);
}

template <std::floating_point T>
[[nodiscard]] constexpr auto add_values(T left, T right) noexcept -> arithmetic_result<T> {
    if (!std::isfinite(left) || !std::isfinite(right)) {
        return failure<T>();
    }
    return success_or_floating_failure(left + right);
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto subtract_signed_nonnegative(T left, T right) noexcept -> arithmetic_result<T> {
    return left < std::numeric_limits<T>::lowest() + right ? failure<T>() : success<T>(left - right);
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto subtract_signed_negative(T left, T right) noexcept -> arithmetic_result<T> {
    return left > std::numeric_limits<T>::max() + right ? failure<T>() : success<T>(left - right);
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto subtract_values(T left, T right) noexcept -> arithmetic_result<T> {
    if (right >= 0) {
        return subtract_signed_nonnegative(left, right);
    }
    return subtract_signed_negative(left, right);
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto subtract_values(T left, T right) noexcept -> arithmetic_result<T> {
    return left < right ? failure<T>() : success<T>(left - right);
}

template <std::floating_point T>
[[nodiscard]] constexpr auto subtract_values(T left, T right) noexcept -> arithmetic_result<T> {
    if (!std::isfinite(left) || !std::isfinite(right)) {
        return failure<T>();
    }
    return success_or_floating_failure(left - right);
}

template <typename T>
[[nodiscard]] constexpr auto floating_inputs_are_finite(T left, T right) noexcept -> bool {
    return std::isfinite(left) && std::isfinite(right);
}

template <typename T>
[[nodiscard]] constexpr auto success_or_floating_failure(T value) noexcept -> arithmetic_result<T> {
    return std::isfinite(value) ? success(value) : failure<T>();
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto positive_multiplication_overflows(T left, T right) noexcept -> bool {
    if (right > 0) {
        return left > std::numeric_limits<T>::max() / right;
    }
    return right < std::numeric_limits<T>::lowest() / left;
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto negative_multiplication_overflows(T left, T right) noexcept -> bool {
    if (right > 0) {
        return left < std::numeric_limits<T>::lowest() / right;
    }
    return right < std::numeric_limits<T>::max() / left;
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto signed_multiplication_overflows(T left, T right) noexcept -> bool {
    if (left > 0) {
        return positive_multiplication_overflows(left, right);
    }
    return negative_multiplication_overflows(left, right);
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto multiply_values(T left, T right) noexcept -> arithmetic_result<T> {
    if (left == 0 || right == 0) {
        return success<T>(T{});
    }
    if (signed_multiplication_overflows(left, right)) {
        return failure<T>();
    }
    return success<T>(left * right);
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto multiply_values(T left, T right) noexcept -> arithmetic_result<T> {
    if (right == 0) {
        return success<T>(T{});
    }
    return left > std::numeric_limits<T>::max() / right ? failure<T>() : success<T>(left * right);
}

template <std::floating_point T>
[[nodiscard]] constexpr auto multiplication_has_zero_factor(T left, T right) noexcept -> bool {
    return left == T{} || right == T{};
}

template <std::floating_point T>
[[nodiscard]] constexpr auto finite_multiplication_result(T left, T right, T value) noexcept -> arithmetic_result<T> {
    if (!std::isfinite(value)) {
        return failure<T>();
    }
    if (value != T{}) {
        return success(value);
    }
    return multiplication_has_zero_factor(left, right) ? success(value) : failure<T>();
}

template <std::floating_point T>
[[nodiscard]] constexpr auto multiply_values(T left, T right) noexcept -> arithmetic_result<T> {
    if (!floating_inputs_are_finite(left, right)) {
        return failure<T>();
    }
    return finite_multiplication_result(left, right, left * right);
}

template <std::floating_point T>
struct division_result_input {
    T numerator;
    T value;
};

template <std::floating_point T>
[[nodiscard]] constexpr auto division_result(division_result_input<T> input) noexcept -> arithmetic_result<T> {
    if (!std::isfinite(input.value)) {
        return failure<T>();
    }
    if (input.value == T{}) {
        if (input.numerator != T{}) {
            return failure<T>();
        }
    }
    return success(input.value);
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto divide_values(T left, T right) noexcept -> arithmetic_result<T> {
    if (right == 0) {
        return failure<T>();
    }
    if (left == std::numeric_limits<T>::lowest() && right == T{-1}) {
        return failure<T>();
    }
    return success<T>(left / right);
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto divide_values(T left, T right) noexcept -> arithmetic_result<T> {
    return right == 0 ? failure<T>() : success<T>(left / right);
}

template <std::floating_point T>
[[nodiscard]] constexpr auto divide_values(T left, T right) noexcept -> arithmetic_result<T> {
    if (!floating_inputs_are_finite(left, right) || right == T{}) {
        return failure<T>();
    }
    const T value = left / right;
    return division_result(division_result_input<T>{left, value});
}

template <typename T>
    requires std::integral<T>
[[nodiscard]] constexpr auto remainder_values(T left, T right) noexcept -> arithmetic_result<T> {
    const auto quotient = divide_values(left, right);
    if (!quotient.succeeds) {
        return failure<T>();
    }
    return success<T>(left % right);
}

template <typename T>
    requires std::integral<T>
[[nodiscard]] constexpr auto modulus_values(T left, T right) noexcept -> arithmetic_result<T> {
    const auto quotient = divide_values(left, right);
    if (!quotient.succeeds) {
        return failure<T>();
    }
    T value = left % right;
    if (value != T{} && ((value < 0) != (right < 0))) {
        value += right;
    }
    return success(value);
}

template <typename T>
[[nodiscard]] constexpr auto result_or_fallback(arithmetic_result<T> result) noexcept -> T {
    if (result.succeeds) {
        return result.value;
    }
    if consteval {
        reject_constant_arithmetic_failure();
    } else {
        return T{};
    }
}

template <typename Left, typename Right, typename Operation>
[[nodiscard]] constexpr auto evaluate(Left left, Right right, Operation operation) noexcept
    -> arithmetic_result<result_t<Left, Right>> {
    using R = result_t<Left, Right>;
    return operation(static_cast<R>(left), static_cast<R>(right));
}

template <typename R, typename... Args>
[[nodiscard]] constexpr auto add_pack_values(Args... args) noexcept -> arithmetic_result<R> {
    arithmetic_result<R> result = success<R>(R{});
    bool first = true;
    auto consume = [&]<typename T>(T value) {
        if (!result.succeeds) {
            return;
        }
        if (first) {
            first = false;
            result = success<R>(static_cast<R>(value));
        } else {
            result = add_values(result.value, static_cast<R>(value));
        }
    };
    (consume(args), ...);
    return result;
}

template <typename R, typename... Args>
[[nodiscard]] constexpr auto can_add_pack_values(Args... args) noexcept -> bool {
    return add_pack_values<R>(args...).succeeds;
}

template <typename R, typename... Args>
[[nodiscard]] constexpr auto sub_pack_values(Args... args) noexcept -> arithmetic_result<R> {
    arithmetic_result<R> result = success<R>(R{});
    bool first = true;
    auto consume = [&]<typename T>(T value) {
        if (!result.succeeds) {
            return;
        }
        if (first) {
            first = false;
            result = success<R>(static_cast<R>(value));
        } else {
            result = subtract_values(result.value, static_cast<R>(value));
        }
    };
    (consume(args), ...);
    return result;
}

template <typename R, typename... Args>
[[nodiscard]] constexpr auto can_sub_pack_values(Args... args) noexcept -> bool {
    return sub_pack_values<R>(args...).succeeds;
}

template <typename R, typename... Args>
[[nodiscard]] constexpr auto mult_pack_values(Args... args) noexcept -> arithmetic_result<R> {
    arithmetic_result<R> result = success<R>(R{1});
    bool first = true;
    auto consume = [&]<typename T>(T value) {
        if (!result.succeeds) {
            return;
        }
        if (first) {
            first = false;
            result = success<R>(static_cast<R>(value));
        } else {
            result = multiply_values(result.value, static_cast<R>(value));
        }
    };
    (consume(args), ...);
    return result;
}

template <typename R, typename... Args>
[[nodiscard]] constexpr auto can_mult_pack_values(Args... args) noexcept -> bool {
    return mult_pack_values<R>(args...).succeeds;
}

template <typename R, typename... Args>
[[nodiscard]] constexpr auto div_pack_values(Args... args) noexcept -> arithmetic_result<R> {
    arithmetic_result<R> result = success<R>(R{});
    bool first = true;
    auto consume = [&]<typename T>(T value) {
        if (!result.succeeds) {
            return;
        }
        if (first) {
            first = false;
            result = success<R>(static_cast<R>(value));
        } else {
            result = divide_values(result.value, static_cast<R>(value));
        }
    };
    (consume(args), ...);
    return result;
}

template <typename R, typename... Args>
[[nodiscard]] constexpr auto can_div_pack_values(Args... args) noexcept -> bool {
    return div_pack_values<R>(args...).succeeds;
}

template <typename T>
concept ShiftCount = SupportedInteger<T>;

template <typename T, std::signed_integral Count>
[[nodiscard]] constexpr auto valid_shift_count(Count count) noexcept -> bool {
    return count >= 0 && std::cmp_less(count, std::numeric_limits<T>::digits);
}

template <typename T, std::unsigned_integral Count>
[[nodiscard]] constexpr auto valid_shift_count(Count count) noexcept -> bool {
    return std::cmp_less(count, std::numeric_limits<T>::digits);
}

template <typename T>
using unsigned_t = std::make_unsigned_t<T>;

template <std::signed_integral T>
struct shift_result_input {
    T original;
    T reconstructed;
};

template <std::signed_integral T>
[[nodiscard]] constexpr auto checked_shift_result_preserves_sign(shift_result_input<T> input) noexcept
    -> arithmetic_result<T>;

template <std::unsigned_integral T, typename Count>
[[nodiscard]] constexpr auto bit_shift_left_value(T value, Count count) noexcept -> arithmetic_result<T> {
    if (!valid_shift_count<T>(count)) {
        return failure<T>();
    }
    const auto highest = std::numeric_limits<T>::max() >> static_cast<std::size_t>(count);
    return value <= highest ? success<T>(value << static_cast<std::size_t>(count)) : failure<T>();
}

template <std::signed_integral T, typename Count>
[[nodiscard]] constexpr auto bit_shift_left_value(T value, Count count) noexcept -> arithmetic_result<T> {
    if (!valid_shift_count<T>(count)) {
        return failure<T>();
    }
    using U = unsigned_t<T>;
    const auto shifted = std::bit_cast<U>(value) << static_cast<std::size_t>(count);
    const T reconstructed = std::bit_cast<T>(shifted);
    return checked_shift_result_preserves_sign(shift_result_input<T>{value, reconstructed});
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto checked_shift_result_preserves_sign(shift_result_input<T> input) noexcept
    -> arithmetic_result<T> {
    if (input.original < 0) {
        return input.reconstructed <= 0 ? success(input.reconstructed) : failure<T>();
    }
    return input.reconstructed >= 0 ? success(input.reconstructed) : failure<T>();
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto arithmetic_shift_fill(T value, unsigned_t<T> shifted, std::size_t count) noexcept
    -> unsigned_t<T> {
    if (value >= 0 || count == 0) {
        return shifted;
    }
    return shifted |
           (std::numeric_limits<unsigned_t<T>>::max() << (std::numeric_limits<unsigned_t<T>>::digits - count));
}

template <typename T, typename Count>
[[nodiscard]] constexpr auto bit_shift_right_value(T value, Count count) noexcept -> arithmetic_result<T> {
    if (!valid_shift_count<T>(count)) {
        return failure<T>();
    }
    const auto amount = static_cast<std::size_t>(count);
    const auto shifted = std::bit_cast<unsigned_t<T>>(value) >> amount;
    if constexpr (std::signed_integral<T>) {
        return success(std::bit_cast<T>(arithmetic_shift_fill(value, shifted, amount)));
    }
    return success(std::bit_cast<T>(shifted));
}

template <typename T, typename Count>
[[nodiscard]] constexpr auto unsigned_bit_shift_right_value(T value, Count count) noexcept -> arithmetic_result<T> {
    if (!valid_shift_count<T>(count)) {
        return failure<T>();
    }
    using U = unsigned_t<T>;
    const U shifted = std::bit_cast<U>(value) >> static_cast<std::size_t>(count);
    return success(std::bit_cast<T>(shifted));
}

template <typename T>
[[nodiscard]] constexpr auto increment_value(T value) noexcept -> arithmetic_result<T> {
    return value == std::numeric_limits<T>::max() ? failure<T>() : success<T>(value + T{1});
}

template <typename T>
[[nodiscard]] constexpr auto decrement_value(T value) noexcept -> arithmetic_result<T> {
    return value == std::numeric_limits<T>::lowest() ? failure<T>() : success<T>(value - T{1});
}

template <std::signed_integral T>
[[nodiscard]] constexpr auto negate_value(T value) noexcept -> arithmetic_result<T> {
    return value == std::numeric_limits<T>::lowest() ? failure<T>() : success<T>(-value);
}

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto negate_value(T value) noexcept -> arithmetic_result<T> {
    return value == T{} ? success<T>(T{}) : failure<T>();
}

template <std::floating_point T>
[[nodiscard]] constexpr auto negate_value(T value) noexcept -> arithmetic_result<T> {
    return std::isfinite(value) ? success<T>(-value) : failure<T>();
}

template <typename...>
inline constexpr bool dependent_false = false;

} // namespace arithmetic_detail

/** \anchor Add
 * The \b add free function provides Clojure-compatible checked addition.
 *
 * \c add<T>() returns \c T{} as the typed zero identity. Unary \c add(x) returns \c x. Binary \c add(x,y) returns the
 * sum of \c x and \c y. Variadic \c add(x,y,&more) returns the left-to-right sum of all operands. Integer overflow and
 * floating overflow, underflow, or nonfinite failures follow \ref CheckedArithmetic. In those runtime failure cases,
 * \c add returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(add<int>() == 0);
  static_assert(add(1, 2, 3) == 6);
  return add(1, 2) == 3 ? 0 : 1;
}
~~~~~
 */
template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto add() noexcept -> std::remove_cvref_t<T> {
    return std::remove_cvref_t<T>{};
}

/** \anchor CanAdd
 * The \b can_add preflight reports whether the matching \ref Add "add" call can succeed without checked arithmetic
 * failure. It supports \c can_add<T>(), \c can_add(x), \c can_add(x,y), and \c can_add(x,y,&more). It is
 * side-effect-free and returns \c true when the matching \c add call can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_add(1, 2, 3));
  return can_add(1, 2) ? 0 : 1;
}
~~~~~
 */
template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto can_add() noexcept -> bool {
    return true;
}

template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto add(T value) noexcept -> std::remove_cvref_t<T> {
    return static_cast<std::remove_cvref_t<T>>(value);
}

template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto can_add(T value) noexcept -> bool {
    (void)value;
    return true;
}

template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto add(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    return arithmetic_detail::result_or_fallback(
        arithmetic_detail::evaluate(left, right, [](auto a, auto b) { return arithmetic_detail::add_values(a, b); }));
}

template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto can_add(Left left, Right right) noexcept -> bool {
    return arithmetic_detail::evaluate(left, right, [](auto a, auto b) { return arithmetic_detail::add_values(a, b); })
        .succeeds;
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>
[[nodiscard]] constexpr auto add(First first, Second second, Third third, Rest... rest) noexcept
    -> arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...> {
    using R = arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...>;
    return arithmetic_detail::result_or_fallback(arithmetic_detail::add_pack_values<R>(first, second, third, rest...));
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>
[[nodiscard]] constexpr auto can_add(First first, Second second, Third third, Rest... rest) noexcept -> bool {
    using R = arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...>;
    return arithmetic_detail::can_add_pack_values<R>(first, second, third, rest...);
}

/** \anchor Sub
 * The \b sub free function provides Clojure-compatible checked subtraction.
 *
 * Unary \c sub(x) returns the checked negation of \c x. Binary \c sub(x,y) returns \c x minus \c y. Variadic \c
 * sub(x,y,&more) returns the left-to-right subtraction of all operands. There is no zero-argument form. Overflow,
 * underflow, and other checked failures follow \ref CheckedArithmetic. In those runtime failure cases, \c sub returns
 * \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(sub(7) == -7);
  static_assert(sub(10, 3, 2) == 5);
  return sub(10, 3) == 7 ? 0 : 1;
}
~~~~~
 */
template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto sub(T value) noexcept -> std::remove_cvref_t<T> {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::result_or_fallback(arithmetic_detail::negate_value(static_cast<U>(value)));
}

/** \anchor CanSub
 * The \b can_sub preflight reports whether the matching \ref Sub "sub" call can succeed without checked arithmetic
 * failure. It supports \c can_sub(x), \c can_sub(x,y), and \c can_sub(x,y,&more). It is side-effect-free and returns
 * \c true when the matching \c sub call can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_sub(10, 3));
  return can_sub(10, 3) ? 0 : 1;
}
~~~~~
 */
template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto can_sub(T value) noexcept -> bool {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::negate_value(static_cast<U>(value)).succeeds;
}

template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto sub(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    return arithmetic_detail::result_or_fallback(arithmetic_detail::evaluate(
        left, right, [](auto a, auto b) { return arithmetic_detail::subtract_values(a, b); }));
}

template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto can_sub(Left left, Right right) noexcept -> bool {
    return arithmetic_detail::evaluate(left, right,
                                       [](auto a, auto b) { return arithmetic_detail::subtract_values(a, b); })
        .succeeds;
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>
[[nodiscard]] constexpr auto sub(First first, Second second, Third third, Rest... rest) noexcept
    -> arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...> {
    using R = arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...>;
    return arithmetic_detail::result_or_fallback(arithmetic_detail::sub_pack_values<R>(first, second, third, rest...));
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>
[[nodiscard]] constexpr auto can_sub(First first, Second second, Third third, Rest... rest) noexcept -> bool {
    using R = arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...>;
    return arithmetic_detail::can_sub_pack_values<R>(first, second, third, rest...);
}

/** \anchor Mult
 * The \b mult free function provides Clojure-compatible checked multiplication.
 *
 * \c mult<T>() returns \c T{1} as the typed one identity. Unary \c mult(x) returns \c x. Binary \c mult(x,y) returns
 * the product of \c x and \c y. Variadic \c mult(x,y,&more) returns the left-to-right product of all operands.
 * Overflow and floating failure follow \ref CheckedArithmetic. In those runtime failure cases, \c mult returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(mult<int>() == 1);
  static_assert(mult(2, 3, 4) == 24);
  return mult(2, 3) == 6 ? 0 : 1;
}
~~~~~
 */
template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto mult() noexcept -> std::remove_cvref_t<T> {
    return std::remove_cvref_t<T>{1};
}

/** \anchor CanMult
 * The \b can_mult preflight reports whether the matching \ref Mult "mult" call can succeed without checked arithmetic
 * failure. It supports \c can_mult<T>(), \c can_mult(x), \c can_mult(x,y), and \c can_mult(x,y,&more). It is
 * side-effect-free and returns \c true when the matching \c mult call can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_mult(2, 3, 4));
  return can_mult(2, 3) ? 0 : 1;
}
~~~~~
 */
template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto can_mult() noexcept -> bool {
    return true;
}

template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto mult(T value) noexcept -> std::remove_cvref_t<T> {
    return static_cast<std::remove_cvref_t<T>>(value);
}

template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto can_mult(T value) noexcept -> bool {
    (void)value;
    return true;
}

template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto mult(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    return arithmetic_detail::result_or_fallback(arithmetic_detail::evaluate(
        left, right, [](auto a, auto b) { return arithmetic_detail::multiply_values(a, b); }));
}

template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto can_mult(Left left, Right right) noexcept -> bool {
    return arithmetic_detail::evaluate(left, right,
                                       [](auto a, auto b) { return arithmetic_detail::multiply_values(a, b); })
        .succeeds;
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>
[[nodiscard]] constexpr auto mult(First first, Second second, Third third, Rest... rest) noexcept
    -> arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...> {
    using R = arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...>;
    return arithmetic_detail::result_or_fallback(arithmetic_detail::mult_pack_values<R>(first, second, third, rest...));
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>
[[nodiscard]] constexpr auto can_mult(First first, Second second, Third third, Rest... rest) noexcept -> bool {
    using R = arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...>;
    return arithmetic_detail::can_mult_pack_values<R>(first, second, third, rest...);
}

/** \anchor Div
 * The \b div free function provides checked division.
 *
 * Unary floating \c div(x) returns \c 1/x; unary integer division is rejected. Binary \c div(x,y) returns \c x divided
 * by \c y. Variadic \c div(x,y,&more) returns the left-to-right quotient of all operands. Integer division follows
 * fixed-width truncate-toward-zero semantics; floating division follows \ref CheckedArithmetic. Division by zero, the
 * smallest-integer divided by negative one, and other runtime failures return \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(cljonic::core::div(2.0) == 0.5);
  static_assert(cljonic::core::div(24.0, 3.0, 2.0) == 4.0);
  return cljonic::core::div(12, 3) == 4 ? 0 : 1;
}
~~~~~
 */
template <arithmetic_detail::SupportedFloating T>
[[nodiscard]] constexpr auto div(T value) noexcept -> std::remove_cvref_t<T> {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::result_or_fallback(
        arithmetic_detail::divide_values(static_cast<U>(T{1}), static_cast<U>(value)));
}

/** \anchor CanDiv
 * The \b can_div preflight reports whether the matching \ref Div "div" call can succeed without checked arithmetic
 * failure. It supports \c can_div(x) for floating values, \c can_div(x,y), and \c can_div(x,y,&more). Unary integer
 * division is rejected. It is side-effect-free and returns \c true when the matching \c div call can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_div(24.0, 3.0, 2.0));
  return can_div(12, 3) ? 0 : 1;
}
~~~~~
 */
template <arithmetic_detail::SupportedFloating T>
[[nodiscard]] constexpr auto can_div(T value) noexcept -> bool {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::divide_values(static_cast<U>(T{1}), static_cast<U>(value)).succeeds;
}

template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto div(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    return arithmetic_detail::result_or_fallback(arithmetic_detail::evaluate(
        left, right, [](auto a, auto b) { return arithmetic_detail::divide_values(a, b); }));
}

template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto can_div(Left left, Right right) noexcept -> bool {
    return arithmetic_detail::evaluate(left, right,
                                       [](auto a, auto b) { return arithmetic_detail::divide_values(a, b); })
        .succeeds;
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>
[[nodiscard]] constexpr auto div(First first, Second second, Third third, Rest... rest) noexcept
    -> arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...> {
    using R = arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...>;
    return arithmetic_detail::result_or_fallback(arithmetic_detail::div_pack_values<R>(first, second, third, rest...));
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>
[[nodiscard]] constexpr auto can_div(First first, Second second, Third third, Rest... rest) noexcept -> bool {
    using R = arithmetic_detail::arithmetic_pack_result_t<First, Second, Third, Rest...>;
    return arithmetic_detail::can_div_pack_values<R>(first, second, third, rest...);
}

/** \anchor Quot
 * The \b quot free function returns the integer quotient truncated toward zero. It is checked for division by zero and
 * the case where the smallest representable integer is divided by negative one, because that quotient would exceed the
 * integer type's range. If either condition occurs, \c quot returns \c 0 at runtime.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(quot(-7, 3) == -2);
  return quot(7, 3) == 2 ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto quot(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    return div(left, right);
}

/** \anchor CanQuot
 * The \b can_quot preflight reports whether \ref Quot "quot" can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_quot(-7, 3));
  return can_quot(7, 3) ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::ArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto can_quot(Left left, Right right) noexcept -> bool {
    return can_div(left, right);
}

/** \anchor Rem
 * The \b rem free function returns the integer remainder with the numerator's sign. Division by zero or the
 * smallest-integer divided by negative one is a runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(rem(-7, 3) == -1);
  return rem(7, 3) == 1 ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::IntegerArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto rem(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    return arithmetic_detail::result_or_fallback(arithmetic_detail::evaluate(
        left, right, [](auto a, auto b) { return arithmetic_detail::remainder_values(a, b); }));
}

/** \anchor CanRem
 * The \b can_rem preflight reports whether \ref Rem "rem" can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_rem(-7, 3));
  return can_rem(7, 3) ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::IntegerArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto can_rem(Left left, Right right) noexcept -> bool {
    return arithmetic_detail::evaluate(left, right,
                                       [](auto a, auto b) { return arithmetic_detail::remainder_values(a, b); })
        .succeeds;
}

/** \anchor Mod
 * The \b mod free function returns the integer modulus. Its quotient is rounded toward negative infinity and the
 * result has the denominator's sign. Division by zero or the smallest-integer divided by negative one is a runtime
 * failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(mod(-7, 3) == 2);
  return mod(7, -3) == -2 ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::IntegerArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto mod(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    return arithmetic_detail::result_or_fallback(arithmetic_detail::evaluate(
        left, right, [](auto a, auto b) { return arithmetic_detail::modulus_values(a, b); }));
}

/** \anchor CanMod
 * The \b can_mod preflight reports whether \ref Mod "mod" can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_mod(-7, 3));
  return can_mod(7, -3) ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::IntegerArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto can_mod(Left left, Right right) noexcept -> bool {
    return arithmetic_detail::evaluate(left, right,
                                       [](auto a, auto b) { return arithmetic_detail::modulus_values(a, b); })
        .succeeds;
}

/** \anchor Inc
 * The \b inc free function returns the checked increment of one supported integer. Incrementing the largest
 * representable integer is a runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(inc(7) == 8);
  return inc(7) == 8 ? 0 : 1;
}
~~~~~
 */
template <arithmetic_detail::SupportedInteger T>
    requires(!std::is_reference_v<T>)
[[nodiscard]] constexpr auto inc(T value) noexcept -> T {
    return arithmetic_detail::result_or_fallback(arithmetic_detail::increment_value(value));
}

/** \anchor CanInc
  * The \b can_inc preflight reports whether incrementing the value succeeds.
  *
  * \b Examples
  *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_inc(7));
  return can_inc(7) ? 0 : 1;
}
~~~~~
  */
template <arithmetic_detail::SupportedInteger T>
    requires(!std::is_reference_v<T>)
[[nodiscard]] constexpr auto can_inc(T value) noexcept -> bool {
    return arithmetic_detail::increment_value(value).succeeds;
}

/** \anchor Dec
 * The \b dec free function returns the checked decrement of one supported integer. Decrementing the smallest
 * representable integer is a runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(dec(7) == 6);
  return dec(7) == 6 ? 0 : 1;
}
~~~~~
 */
template <arithmetic_detail::SupportedInteger T>
    requires(!std::is_reference_v<T>)
[[nodiscard]] constexpr auto dec(T value) noexcept -> T {
    return arithmetic_detail::result_or_fallback(arithmetic_detail::decrement_value(value));
}

/** \anchor CanDec
  * The \b can_dec preflight reports whether decrementing the value succeeds.
  *
  * \b Examples
  *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_dec(7));
  return can_dec(7) ? 0 : 1;
}
~~~~~
  */
template <arithmetic_detail::SupportedInteger T>
    requires(!std::is_reference_v<T>)
[[nodiscard]] constexpr auto can_dec(T value) noexcept -> bool {
    return arithmetic_detail::decrement_value(value).succeeds;
}

/** \anchor Negate
 * The \b negate free function returns checked negation for an integer or floating-point value. Negating the smallest
 * representable integer or a nonfinite floating-point value is a runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(negate(7) == -7);
  static_assert(negate(-7) == 7);
  return negate(2.5) == -2.5 ? 0 : 1;
}
~~~~~
 */
template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto negate(T value) noexcept -> std::remove_cvref_t<T> {
    return arithmetic_detail::result_or_fallback(arithmetic_detail::negate_value(std::remove_cvref_t<T>(value)));
}

/** \anchor CanNegate
 * The \b can_negate preflight reports whether \ref Negate "negate" can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_negate(7));
  return can_negate(-7) ? 0 : 1;
}
~~~~~
 */
template <typename T>
    requires arithmetic_detail::SupportedInteger<T> || arithmetic_detail::SupportedFloating<T>
[[nodiscard]] constexpr auto can_negate(T value) noexcept -> bool {
    return arithmetic_detail::negate_value(std::remove_cvref_t<T>(value)).succeeds;
}

/** \anchor BitShiftLeft
 * The \b bit_shift_left free function performs a checked fixed-width left shift. A negative or too-large count, or a
 * result that exceeds the fixed-width integer range, is a runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_shift_left(1, 3) == 8);
  return can_bit_shift_left(1, 3) ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto bit_shift_left(T value, Count count) noexcept -> std::remove_cvref_t<T> {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::result_or_fallback(arithmetic_detail::bit_shift_left_value(static_cast<U>(value), count));
}

/** \anchor CanBitShiftLeft
 * The \b can_bit_shift_left preflight reports whether \ref BitShiftLeft "bit_shift_left" can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_bit_shift_left(1, 3));
  return can_bit_shift_left(1, 3) ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto can_bit_shift_left(T value, Count count) noexcept -> bool {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::bit_shift_left_value(static_cast<U>(value), count).succeeds;
}

/** \anchor BitShiftRight
 * The \b bit_shift_right free function performs a checked arithmetic right shift. A negative or too-large count is a
 * runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_shift_right(-8, 2) == -2);
  return can_bit_shift_right(-8, 2) ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto bit_shift_right(T value, Count count) noexcept -> std::remove_cvref_t<T> {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::result_or_fallback(
        arithmetic_detail::bit_shift_right_value(static_cast<U>(value), count));
}

/** \anchor CanBitShiftRight
 * The \b can_bit_shift_right preflight reports whether \ref BitShiftRight "bit_shift_right" can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_bit_shift_right(-8, 2));
  return can_bit_shift_right(-8, 2) ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto can_bit_shift_right(T value, Count count) noexcept -> bool {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::bit_shift_right_value(static_cast<U>(value), count).succeeds;
}

/** \anchor UnsignedBitShiftRight
 * The \b unsigned_bit_shift_right free function performs a logical fixed-width right shift. A negative or too-large
 * count is a runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(unsigned_bit_shift_right(-8, 1) ==
                std::numeric_limits<int>::max() - 3);
  return can_unsigned_bit_shift_right(-8, 1) ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto unsigned_bit_shift_right(T value, Count count) noexcept -> std::remove_cvref_t<T> {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::result_or_fallback(
        arithmetic_detail::unsigned_bit_shift_right_value(static_cast<U>(value), count));
}

/** \anchor CanUnsignedBitShiftRight
 * The \b can_unsigned_bit_shift_right preflight reports whether \ref UnsignedBitShiftRight "unsigned_bit_shift_right"
 * can succeed.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(can_unsigned_bit_shift_right(-8, 1));
  return can_unsigned_bit_shift_right(-8, 1) ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto can_unsigned_bit_shift_right(T value, Count count) noexcept -> bool {
    using U = std::remove_cvref_t<T>;
    return arithmetic_detail::unsigned_bit_shift_right_value(static_cast<U>(value), count).succeeds;
}

/** \anchor BitNot
 * The \b bit_not free function returns the fixed-width bitwise complement.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_not(0) == -1);
  return bit_not(0) == -1 ? 0 : 1;
}
~~~~~
 */
template <arithmetic_detail::SupportedInteger T>
[[nodiscard]] constexpr auto bit_not(T value) noexcept -> std::remove_cvref_t<T> {
    using U = std::remove_cvref_t<T>;
    return std::bit_cast<U>(
        static_cast<arithmetic_detail::unsigned_t<U>>(~std::bit_cast<arithmetic_detail::unsigned_t<U>>(value)));
}

#define CLJONIC_BINARY_BITWISE(name, op)                                                                               \
    template <typename Left, typename Right>                                                                           \
        requires arithmetic_detail::IntegerArithmeticPair<Left, Right>                                                 \
    [[nodiscard]] constexpr auto name(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {   \
        using R = arithmetic_detail::result_t<Left, Right>;                                                            \
        return std::bit_cast<R>(std::bit_cast<arithmetic_detail::unsigned_t<R>>(static_cast<R>(left))                  \
                                    op std::bit_cast<arithmetic_detail::unsigned_t<R>>(static_cast<R>(right)));        \
    }

/** \anchor BitAnd
 * The \b bit_and free function returns the fixed-width bitwise AND of two integers.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_and(0b1100, 0b1010) == 0b1000);
  return bit_and(0b1100, 0b1010) == 0b1000 ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::IntegerArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto bit_and(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    using R = arithmetic_detail::result_t<Left, Right>;
    return std::bit_cast<R>(std::bit_cast<arithmetic_detail::unsigned_t<R>>(static_cast<R>(left)) &
                            std::bit_cast<arithmetic_detail::unsigned_t<R>>(static_cast<R>(right)));
}

/** \anchor BitOr
 * The \b bit_or free function returns the fixed-width bitwise OR of two integers.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_or(0b1100, 0b1010) == 0b1110);
  return bit_or(0b1100, 0b1010) == 0b1110 ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::IntegerArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto bit_or(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    using R = arithmetic_detail::result_t<Left, Right>;
    return std::bit_cast<R>(std::bit_cast<arithmetic_detail::unsigned_t<R>>(static_cast<R>(left)) |
                            std::bit_cast<arithmetic_detail::unsigned_t<R>>(static_cast<R>(right)));
}

/** \anchor BitXor
 * The \b bit_xor free function returns the fixed-width bitwise XOR of two integers.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_xor(0b1100, 0b1010) == 0b0110);
  return bit_xor(0b1100, 0b1010) == 0b0110 ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::IntegerArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto bit_xor(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    using R = arithmetic_detail::result_t<Left, Right>;
    return std::bit_cast<R>(std::bit_cast<arithmetic_detail::unsigned_t<R>>(static_cast<R>(left)) ^
                            std::bit_cast<arithmetic_detail::unsigned_t<R>>(static_cast<R>(right)));
}

#undef CLJONIC_BINARY_BITWISE

/** \anchor BitAndNot
 * The \b bit_and_not free function clears each bit in the right operand from the left operand.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_and_not(0b1100, 0b1000) == 0b0100);
  return bit_and_not(0b1100, 0b1000) == 0b0100 ? 0 : 1;
}
~~~~~
 */
template <typename Left, typename Right>
    requires arithmetic_detail::IntegerArithmeticPair<Left, Right>
[[nodiscard]] constexpr auto bit_and_not(Left left, Right right) noexcept -> arithmetic_detail::result_t<Left, Right> {
    using R = arithmetic_detail::result_t<Left, Right>;
    using U = arithmetic_detail::unsigned_t<R>;
    return std::bit_cast<R>(std::bit_cast<U>(static_cast<R>(left)) & ~std::bit_cast<U>(static_cast<R>(right)));
}

/** \anchor BitClear
 * The \b bit_clear free function clears one bit index in a supported integer. An invalid or too-large bit index is a
 * runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_clear(0b1111, 2) == 0b1011);
  return bit_clear(0b1111, 2) == 0b1011 ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto bit_clear(T value, Count index) noexcept -> std::remove_cvref_t<T> {
    using U = std::remove_cvref_t<T>;
    return !can_bit_shift_left(U{1}, index)
               ? U{}
               : std::bit_cast<U>(std::bit_cast<arithmetic_detail::unsigned_t<U>>(value) &
                                  ~std::bit_cast<arithmetic_detail::unsigned_t<U>>(bit_shift_left(U{1}, index)));
}

/** \anchor BitSet
 * The \b bit_set free function sets one bit index in a supported integer. An invalid or too-large bit index is a
 * runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_set(0, 3) == 8);
  return bit_set(0, 3) == 8 ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto bit_set(T value, Count index) noexcept -> std::remove_cvref_t<T> {
    using U = std::remove_cvref_t<T>;
    return !can_bit_shift_left(U{1}, index)
               ? U{}
               : std::bit_cast<U>(std::bit_cast<arithmetic_detail::unsigned_t<U>>(value) |
                                  std::bit_cast<arithmetic_detail::unsigned_t<U>>(bit_shift_left(U{1}, index)));
}

/** \anchor BitFlip
 * The \b bit_flip free function flips one bit index in a supported integer. An invalid or too-large bit index is a
 * runtime failure that returns \c 0.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_flip(0b1010, 0) == 0b1011);
  return bit_flip(0b1010, 0) == 0b1011 ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto bit_flip(T value, Count index) noexcept -> std::remove_cvref_t<T> {
    using U = std::remove_cvref_t<T>;
    return !can_bit_shift_left(U{1}, index)
               ? U{}
               : std::bit_cast<U>(std::bit_cast<arithmetic_detail::unsigned_t<U>>(value) ^
                                  std::bit_cast<arithmetic_detail::unsigned_t<U>>(bit_shift_left(U{1}, index)));
}

/** \anchor BitTest
 * The \b bit_test free function reports whether one bit index is set in a supported integer. An invalid or too-large
 * bit index returns \c false.
 *
 * \b Examples
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

int main() {
  static_assert(bit_test(0b1010, 1));
  return bit_test(0b1010, 1) ? 0 : 1;
}
~~~~~
 */
template <typename T, typename Count>
    requires arithmetic_detail::SupportedInteger<T> && arithmetic_detail::ShiftCount<Count>
[[nodiscard]] constexpr auto bit_test(T value, Count index) noexcept -> bool {
    using U = std::remove_cvref_t<T>;
    return can_bit_shift_left(U{1}, index) &&
           (std::bit_cast<arithmetic_detail::unsigned_t<U>>(value) &
            std::bit_cast<arithmetic_detail::unsigned_t<U>>(bit_shift_left(U{1}, index))) != 0;
}

/// @cond CLJONIC_INTERNAL

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto add() noexcept -> void {
    static_assert(
        arithmetic_detail::dependent_false<T>,
        "cljonic::add: the zero-argument form requires an explicit supported integer or floating-point type.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto can_add() noexcept -> void {
    static_assert(
        arithmetic_detail::dependent_false<T>,
        "cljonic::can_add: the zero-argument form requires an explicit supported integer or floating-point type.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto add([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::add: expected one supported integer or floating-point value.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto can_add([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::can_add: expected one supported integer or floating-point value.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto sub([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::sub: unary negation requires one supported integer or floating-point value.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto can_sub([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::can_sub: unary negation requires one supported integer or floating-point value.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto mult() noexcept -> void {
    static_assert(
        arithmetic_detail::dependent_false<T>,
        "cljonic::mult: the zero-argument form requires an explicit supported integer or floating-point type.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto can_mult() noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::can_mult: the zero-argument form requires an explicit supported integer or "
                  "floating-point type.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto mult([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::mult: unary multiplication requires one supported integer or floating-point value.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto can_mult([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::can_mult: unary multiplication requires one supported integer or floating-point value.");
}

template <typename T>
    requires arithmetic_detail::SupportedInteger<T>
constexpr auto div([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::div: unary division is supported only for one supported binary floating-point type.");
}

template <typename T>
    requires arithmetic_detail::SupportedInteger<T>
constexpr auto can_div([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::can_div: unary division is supported only for one supported binary floating-point type.");
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires(!arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>)
constexpr auto add([[maybe_unused]] First first, [[maybe_unused]] Second second, [[maybe_unused]] Third third,
                   [[maybe_unused]] Rest... rest) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<First, Second, Third, Rest...>,
                  "cljonic::add: variadic operands require supported integers with a lossless common type or identical "
                  "supported floating-point types.");
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires(!arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>)
constexpr auto can_add([[maybe_unused]] First first, [[maybe_unused]] Second second, [[maybe_unused]] Third third,
                       [[maybe_unused]] Rest... rest) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<First, Second, Third, Rest...>,
                  "cljonic::can_add: variadic operands require the same operand pack as add.");
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires(!arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>)
constexpr auto sub([[maybe_unused]] First first, [[maybe_unused]] Second second, [[maybe_unused]] Third third,
                   [[maybe_unused]] Rest... rest) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<First, Second, Third, Rest...>,
                  "cljonic::sub: variadic operands require supported integers with a lossless common type or identical "
                  "supported floating-point types.");
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires(!arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>)
constexpr auto can_sub([[maybe_unused]] First first, [[maybe_unused]] Second second, [[maybe_unused]] Third third,
                       [[maybe_unused]] Rest... rest) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<First, Second, Third, Rest...>,
                  "cljonic::can_sub: variadic operands require the same operand pack as sub.");
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires(!arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>)
constexpr auto mult([[maybe_unused]] First first, [[maybe_unused]] Second second, [[maybe_unused]] Third third,
                    [[maybe_unused]] Rest... rest) noexcept -> void {
    static_assert(
        arithmetic_detail::dependent_false<First, Second, Third, Rest...>,
        "cljonic::mult: variadic operands require supported integers with a lossless common type or identical "
        "supported floating-point types.");
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires(!arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>)
constexpr auto can_mult([[maybe_unused]] First first, [[maybe_unused]] Second second, [[maybe_unused]] Third third,
                        [[maybe_unused]] Rest... rest) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<First, Second, Third, Rest...>,
                  "cljonic::can_mult: variadic operands require the same operand pack as mult.");
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires(!arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>)
constexpr auto div([[maybe_unused]] First first, [[maybe_unused]] Second second, [[maybe_unused]] Third third,
                   [[maybe_unused]] Rest... rest) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<First, Second, Third, Rest...>,
                  "cljonic::div: variadic operands require supported integers with a lossless common type or identical "
                  "supported floating-point types.");
}

template <typename First, typename Second, typename Third, typename... Rest>
    requires(!arithmetic_detail::ArithmeticPack<First, Second, Third, Rest...>)
constexpr auto can_div([[maybe_unused]] First first, [[maybe_unused]] Second second, [[maybe_unused]] Third third,
                       [[maybe_unused]] Rest... rest) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<First, Second, Third, Rest...>,
                  "cljonic::can_div: variadic operands require the same operand pack as div.");
}

#define CLJONIC_BINARY_ARITHMETIC_FALLBACK(name, admitted_concept, message)                                            \
    template <typename Left, typename Right>                                                                           \
        requires(!arithmetic_detail::admitted_concept<Left, Right>)                                                    \
    constexpr auto name([[maybe_unused]] Left left, [[maybe_unused]] Right right) noexcept -> void {                   \
        static_assert(arithmetic_detail::dependent_false<Left, Right>, message);                                       \
    }

CLJONIC_BINARY_ARITHMETIC_FALLBACK(
    add, ArithmeticPair,
    "cljonic::add: expected two supported integer types with a lossless common type, or two "
    "values of the same supported floating type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(can_add, ArithmeticPair, "cljonic::can_add: expected the same operand pair as add.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(
    sub, ArithmeticPair,
    "cljonic::sub: expected two supported integer types with a lossless common type, or two "
    "values of the same supported floating type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(can_sub, ArithmeticPair, "cljonic::can_sub: expected the same operand pair as sub.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(
    mult, ArithmeticPair,
    "cljonic::mult: expected two supported integer types with a lossless common type, or two "
    "values of the same supported floating type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(can_mult, ArithmeticPair,
                                   "cljonic::can_mult: expected the same operand pair as mult.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(
    div, ArithmeticPair,
    "cljonic::div: expected two supported integer types with a lossless common type, or two "
    "values of the same supported floating type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(can_div, ArithmeticPair, "cljonic::can_div: expected the same operand pair as div.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(
    quot, ArithmeticPair,
    "cljonic::quot: expected two supported integer types with a lossless common type, or two "
    "values of the same supported floating type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(can_quot, ArithmeticPair,
                                   "cljonic::can_quot: expected the same operand pair as quot.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(rem, IntegerArithmeticPair,
                                   "cljonic::rem: expected two supported integer types with a lossless common type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(can_rem, IntegerArithmeticPair,
                                   "cljonic::can_rem: expected the same operand pair as rem.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(mod, IntegerArithmeticPair,
                                   "cljonic::mod: expected two supported integer types with a lossless common type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(can_mod, IntegerArithmeticPair,
                                   "cljonic::can_mod: expected the same operand pair as mod.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(
    bit_and, IntegerArithmeticPair,
    "cljonic::bit_and: expected two supported integer types with a lossless common type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(bit_or, IntegerArithmeticPair,
                                   "cljonic::bit_or: expected two supported integer types with a lossless common type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(
    bit_xor, IntegerArithmeticPair,
    "cljonic::bit_xor: expected two supported integer types with a lossless common type.")
CLJONIC_BINARY_ARITHMETIC_FALLBACK(
    bit_and_not, IntegerArithmeticPair,
    "cljonic::bit_and_not: expected two supported integer types with a lossless common type.")

#undef CLJONIC_BINARY_ARITHMETIC_FALLBACK

#define CLJONIC_INTEGER_UNARY_FALLBACK(name, message)                                                                  \
    template <typename T>                                                                                              \
        requires(!arithmetic_detail::SupportedInteger<T>)                                                              \
    constexpr auto name([[maybe_unused]] T value) noexcept -> void {                                                   \
        static_assert(arithmetic_detail::dependent_false<T>, message);                                                 \
    }

CLJONIC_INTEGER_UNARY_FALLBACK(inc, "cljonic::inc: expected a supported integer.")
CLJONIC_INTEGER_UNARY_FALLBACK(can_inc, "cljonic::can_inc: expected a supported integer.")
CLJONIC_INTEGER_UNARY_FALLBACK(dec, "cljonic::dec: expected a supported integer.")
CLJONIC_INTEGER_UNARY_FALLBACK(can_dec, "cljonic::can_dec: expected a supported integer.")
CLJONIC_INTEGER_UNARY_FALLBACK(bit_not, "cljonic::bit_not: expected a supported integer.")

#undef CLJONIC_INTEGER_UNARY_FALLBACK

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto negate([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::negate: expected a supported integer or floating type.");
}

template <typename T>
    requires(!arithmetic_detail::SupportedInteger<T> && !arithmetic_detail::SupportedFloating<T>)
constexpr auto can_negate([[maybe_unused]] T value) noexcept -> void {
    static_assert(arithmetic_detail::dependent_false<T>,
                  "cljonic::can_negate: expected a supported integer or floating type.");
}

#define CLJONIC_SHIFT_FALLBACK(name)                                                                                   \
    template <typename T, typename Count>                                                                              \
        requires(!arithmetic_detail::SupportedInteger<T> || !arithmetic_detail::ShiftCount<Count>)                     \
    constexpr auto name([[maybe_unused]] T value, [[maybe_unused]] Count count) noexcept -> void {                     \
        static_assert(arithmetic_detail::dependent_false<T, Count>,                                                    \
                      "cljonic::" #name ": expected a supported integer value and integer shift count.");              \
    }

CLJONIC_SHIFT_FALLBACK(bit_shift_left)
CLJONIC_SHIFT_FALLBACK(can_bit_shift_left)
CLJONIC_SHIFT_FALLBACK(bit_shift_right)
CLJONIC_SHIFT_FALLBACK(can_bit_shift_right)
CLJONIC_SHIFT_FALLBACK(unsigned_bit_shift_right)
CLJONIC_SHIFT_FALLBACK(can_unsigned_bit_shift_right)
CLJONIC_SHIFT_FALLBACK(bit_clear)
CLJONIC_SHIFT_FALLBACK(bit_set)
CLJONIC_SHIFT_FALLBACK(bit_flip)
CLJONIC_SHIFT_FALLBACK(bit_test)

#undef CLJONIC_SHIFT_FALLBACK

/// @endcond

} // namespace cljonic::core
