#include <cstddef>
#include <limits>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

template <typename Left, typename Right>
concept ArithmeticPairAvailable = cljonic::core::arithmetic_detail::ArithmeticPair<Left, Right>;

template <typename Left, typename Right>
concept IntegerArithmeticPairAvailable = cljonic::core::arithmetic_detail::IntegerArithmeticPair<Left, Right>;

TEST_CASE("Checked arithmetic admission and result types are lossless", "[arithmetic]") {
    TRACE_ID("entity-fields.CheckedArithmetic");
    TRACE_ID("invariant.CheckedArithmetic.OperandDomainIsClosedAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.ResultTypeIsStaticAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.UnsupportedTypesAreRejected");

    STATIC_REQUIRE(ArithmeticPairAvailable<int, int>);
    STATIC_REQUIRE(ArithmeticPairAvailable<short, unsigned short>);
    STATIC_REQUIRE(ArithmeticPairAvailable<double, double>);
    STATIC_REQUIRE_FALSE(ArithmeticPairAvailable<int, double>);
    STATIC_REQUIRE_FALSE(ArithmeticPairAvailable<float, double>);
    STATIC_REQUIRE_FALSE(ArithmeticPairAvailable<bool, int>);
    STATIC_REQUIRE_FALSE(ArithmeticPairAvailable<char, int>);
    STATIC_REQUIRE_FALSE(ArithmeticPairAvailable<std::byte, int>);
    STATIC_REQUIRE_FALSE(ArithmeticPairAvailable<unsigned long long, long long>);
    STATIC_REQUIRE_FALSE(IntegerArithmeticPairAvailable<float, float>);

    using cljonic::core::add;
    STATIC_REQUIRE(std::same_as<decltype(add(short{1}, int{2})), int>);
    STATIC_REQUIRE(std::same_as<decltype(add(static_cast<unsigned short>(1), short{2})), int>);
    STATIC_REQUIRE(std::same_as<decltype(add(1.0, 2.0)), double>);
}

TEST_CASE("Add provides Clojure-compatible arities and matching preflights", "[arithmetic]") {
    TRACE_ID("invariant.CheckedArithmetic.ClojureCompatibleAddArities");
    TRACE_ID("invariant.CheckedArithmetic.PreflightMatchesOperation");
    TRACE_ID("invariant.CheckedArithmetic.OperandDomainIsClosedAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.ResultTypeIsStaticAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.OverflowAndUnderflowAreChecked");

    using cljonic::core::add;
    using cljonic::core::can_add;

    STATIC_REQUIRE(std::same_as<decltype(add<int>()), int>);
    STATIC_REQUIRE(std::same_as<decltype(add<double>()), double>);
    STATIC_REQUIRE(add<int>() == 0);
    STATIC_REQUIRE(add<double>() == 0.0);
    STATIC_REQUIRE(add(7) == 7);
    STATIC_REQUIRE(add(1, 2, 3) == 6);
    STATIC_REQUIRE(add(short{1}, int{2}, short{3}) == 6);
    STATIC_REQUIRE(std::same_as<decltype(add(short{1}, int{2}, short{3})), int>);
    STATIC_REQUIRE(add(1.0, 2.0, 3.0) == 6.0);
    STATIC_REQUIRE(std::same_as<decltype(add(1.0, 2.0, 3.0)), double>);

    STATIC_REQUIRE(can_add<int>());
    STATIC_REQUIRE(can_add(7));
    STATIC_REQUIRE(can_add(1, 2, 3));
    STATIC_REQUIRE(can_add(short{1}, int{2}, short{3}));
    STATIC_REQUIRE(can_add(1.0, 2.0, 3.0));
    STATIC_REQUIRE_FALSE(can_add(std::numeric_limits<int>::max(), 1, 1));

    REQUIRE(add(std::numeric_limits<int>::max(), 1, 1) == 0);
    REQUIRE_FALSE(can_add(std::numeric_limits<double>::infinity(), 1.0, 1.0));
    REQUIRE(add(std::numeric_limits<double>::infinity(), 1.0, 1.0) == 0.0);
}

TEST_CASE("Sub provides Clojure-compatible unary binary and variadic arities", "[arithmetic]") {
    TRACE_ID("invariant.CheckedArithmetic.ClojureCompatibleSubArities");
    TRACE_ID("invariant.CheckedArithmetic.PreflightMatchesOperation");
    TRACE_ID("invariant.CheckedArithmetic.OperandDomainIsClosedAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.ResultTypeIsStaticAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.OverflowAndUnderflowAreChecked");

    using cljonic::core::can_subtract;
    using cljonic::core::sub;

    STATIC_REQUIRE(sub(7) == -7);
    STATIC_REQUIRE(sub(-7) == 7);
    STATIC_REQUIRE_FALSE(can_subtract(std::numeric_limits<int>::lowest()));
    REQUIRE(sub(std::numeric_limits<int>::lowest()) == 0);

    STATIC_REQUIRE(sub(10, 3) == 7);
    STATIC_REQUIRE(sub(10, 3, 2) == 5);
    STATIC_REQUIRE(sub(short{10}, int{3}, short{2}) == 5);
    STATIC_REQUIRE(std::same_as<decltype(sub(short{10}, int{3}, short{2})), int>);
    STATIC_REQUIRE(sub(10.0, 3.0, 2.0) == 5.0);
    STATIC_REQUIRE(std::same_as<decltype(sub(10.0, 3.0, 2.0)), double>);

    STATIC_REQUIRE(can_subtract(7));
    STATIC_REQUIRE(can_subtract(10, 3));
    STATIC_REQUIRE(can_subtract(10, 3, 2));
    STATIC_REQUIRE(can_subtract(short{10}, int{3}, short{2}));
    STATIC_REQUIRE(can_subtract(10.0, 3.0, 2.0));
    STATIC_REQUIRE_FALSE(can_subtract(std::numeric_limits<int>::max(), -1, -1));
    REQUIRE(sub(std::numeric_limits<int>::lowest(), 1) == 0);
    REQUIRE_FALSE(can_subtract(std::numeric_limits<double>::infinity(), 1.0, 1.0));
    REQUIRE(sub(std::numeric_limits<double>::infinity(), 1.0, 1.0) == 0.0);
}

TEST_CASE("Mult provides Clojure-compatible unary binary and variadic arities", "[arithmetic]") {
    TRACE_ID("invariant.CheckedArithmetic.ClojureCompatibleMultArities");
    TRACE_ID("invariant.CheckedArithmetic.PreflightMatchesOperation");
    TRACE_ID("invariant.CheckedArithmetic.OperandDomainIsClosedAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.ResultTypeIsStaticAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.OverflowAndUnderflowAreChecked");

    using cljonic::core::can_multiply;
    using cljonic::core::mult;

    STATIC_REQUIRE(std::same_as<decltype(mult<int>()), int>);
    STATIC_REQUIRE(std::same_as<decltype(mult<double>()), double>);
    STATIC_REQUIRE(mult<int>() == 1);
    STATIC_REQUIRE(mult<double>() == 1.0);
    STATIC_REQUIRE(mult(7) == 7);
    STATIC_REQUIRE(mult(2, 3) == 6);
    STATIC_REQUIRE(mult(2, 3, 4) == 24);
    STATIC_REQUIRE(mult(short{2}, int{3}, short{4}) == 24);
    STATIC_REQUIRE(std::same_as<decltype(mult(short{2}, int{3}, short{4})), int>);
    STATIC_REQUIRE(mult(2.0, 3.0, 4.0) == 24.0);
    STATIC_REQUIRE(std::same_as<decltype(mult(2.0, 3.0, 4.0)), double>);

    STATIC_REQUIRE(can_multiply<int>());
    STATIC_REQUIRE(can_multiply(7));
    STATIC_REQUIRE(can_multiply(2, 3));
    STATIC_REQUIRE(can_multiply(2, 3, 4));
    STATIC_REQUIRE(can_multiply(short{2}, int{3}, short{4}));
    STATIC_REQUIRE(can_multiply(2.0, 3.0, 4.0));
    STATIC_REQUIRE_FALSE(can_multiply(std::numeric_limits<int>::max(), 2));
    STATIC_REQUIRE_FALSE(can_multiply(std::numeric_limits<int>::max(), 2, 2));
    REQUIRE(mult(std::numeric_limits<int>::max(), 2) == 0);
    REQUIRE_FALSE(can_multiply(std::numeric_limits<double>::infinity(), 1.0));
    REQUIRE(mult(std::numeric_limits<double>::infinity(), 1.0) == 0.0);
}

TEST_CASE("Div provides floating reciprocal binary and variadic arities", "[arithmetic]") {
    TRACE_ID("invariant.CheckedArithmetic.ClojureCompatibleDivArities");
    TRACE_ID("invariant.CheckedArithmetic.PreflightMatchesOperation");
    TRACE_ID("invariant.CheckedArithmetic.OperandDomainIsClosedAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.ResultTypeIsStaticAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.OverflowAndUnderflowAreChecked");

    using cljonic::core::can_div;
    using cljonic::core::div;

    STATIC_REQUIRE(div(2.0) == 0.5);
    STATIC_REQUIRE(can_div(2.0));
    STATIC_REQUIRE_FALSE(can_div(0.0));
    REQUIRE(div(0.0) == 0.0);

    STATIC_REQUIRE(div(12.0, 3.0) == 4.0);
    STATIC_REQUIRE(div(24.0, 3.0, 2.0) == 4.0);
    STATIC_REQUIRE(std::same_as<decltype(div(24.0, 3.0, 2.0)), double>);
    STATIC_REQUIRE(can_div(24.0, 3.0, 2.0));
    STATIC_REQUIRE_FALSE(can_div(24.0, 0.0, 2.0));
    REQUIRE(div(24.0, 0.0, 2.0) == 0.0);

    STATIC_REQUIRE(div(12, 3) == 4);
    STATIC_REQUIRE(div(24, 3, 2) == 4);
    STATIC_REQUIRE(std::same_as<decltype(div(short{24}, int{3}, short{2})), int>);
    STATIC_REQUIRE(can_div(24, 3, 2));
    STATIC_REQUIRE_FALSE(can_div(24, 0, 2));
}

TEST_CASE("Checked integer arithmetic predicts overflow and preserves its fallback contract", "[arithmetic]") {
    TRACE_ID("invariant.CheckedArithmetic.OverflowAndUnderflowAreChecked");
    TRACE_ID("invariant.CheckedArithmetic.PreflightMatchesOperation");
    TRACE_ID("invariant.CheckedArithmetic.NoThrowNoHeapAndConstexpr");

    using cljonic::core::add;
    using cljonic::core::can_add;
    using cljonic::core::can_dec;
    using cljonic::core::can_inc;
    using cljonic::core::can_multiply;
    using cljonic::core::can_negate;
    using cljonic::core::can_subtract;
    using cljonic::core::dec;
    using cljonic::core::inc;
    using cljonic::core::mult;
    using cljonic::core::negate;
    using cljonic::core::sub;

    STATIC_REQUIRE(can_add(2, 3));
    STATIC_REQUIRE(add(2, 3) == 5);
    STATIC_REQUIRE_FALSE(can_add(std::numeric_limits<int>::max(), 1));
    STATIC_REQUIRE_FALSE(can_subtract(std::numeric_limits<int>::lowest(), 1));
    STATIC_REQUIRE_FALSE(can_multiply(std::numeric_limits<int>::max(), 2));
    REQUIRE(add(std::numeric_limits<int>::max(), 1) == 0);
    REQUIRE(sub(std::numeric_limits<int>::lowest(), 1) == 0);
    REQUIRE(mult(std::numeric_limits<int>::max(), 2) == 0);

    STATIC_REQUIRE(can_inc(7));
    STATIC_REQUIRE(inc(7) == 8);
    STATIC_REQUIRE_FALSE(can_inc(std::numeric_limits<int>::max()));
    STATIC_REQUIRE_FALSE(can_dec(std::numeric_limits<int>::lowest()));
    STATIC_REQUIRE_FALSE(can_negate(std::numeric_limits<int>::lowest()));
    STATIC_REQUIRE_FALSE(can_negate(1U));
    STATIC_REQUIRE(can_negate(0U));
    REQUIRE(dec(std::numeric_limits<int>::lowest()) == 0);
    REQUIRE(negate(std::numeric_limits<int>::lowest()) == 0);

    STATIC_REQUIRE(noexcept(can_add(1, 2)));
    STATIC_REQUIRE(noexcept(add(1, 2)));
}

TEST_CASE("Integer division follows checked Clojure-compatible semantics", "[arithmetic]") {
    TRACE_ID("invariant.CheckedArithmetic.DivisionAndFloatingFailuresAreChecked");
    TRACE_ID("invariant.CheckedArithmetic.QuotRemModSemantics");
    TRACE_ID("invariant.CheckedArithmetic.PreflightMatchesOperation");

    using cljonic::core::can_div;
    using cljonic::core::can_mod;
    using cljonic::core::can_quot;
    using cljonic::core::can_rem;
    using cljonic::core::div;
    using cljonic::core::mod;
    using cljonic::core::quot;
    using cljonic::core::rem;

    STATIC_REQUIRE(quot(-7, 3) == -2);
    STATIC_REQUIRE(rem(-7, 3) == -1);
    STATIC_REQUIRE(mod(-7, 3) == 2);
    STATIC_REQUIRE(rem(7, -3) == 1);
    STATIC_REQUIRE(mod(7, -3) == -2);
    STATIC_REQUIRE(div(-7, 3) == -2);

    STATIC_REQUIRE_FALSE(can_div(1, 0));
    STATIC_REQUIRE_FALSE(can_quot(1, 0));
    STATIC_REQUIRE_FALSE(can_rem(1, 0));
    STATIC_REQUIRE_FALSE(can_mod(1, 0));
    STATIC_REQUIRE_FALSE(can_div(std::numeric_limits<int>::lowest(), -1));
    REQUIRE(div(1, 0) == 0);
    REQUIRE(quot(1, 0) == 0);
    REQUIRE(rem(1, 0) == 0);
    REQUIRE(mod(1, 0) == 0);
    REQUIRE(div(std::numeric_limits<int>::lowest(), -1) == 0);
}

TEST_CASE("Floating arithmetic rejects nonfinite overflow and underflow results", "[arithmetic]") {
    TRACE_ID("invariant.CheckedArithmetic.DivisionAndFloatingFailuresAreChecked");
    TRACE_ID("invariant.CheckedArithmetic.OverflowAndUnderflowAreChecked");
    TRACE_ID("invariant.CheckedArithmetic.PreflightMatchesOperation");

    using cljonic::core::add;
    using cljonic::core::can_add;
    using cljonic::core::can_div;
    using cljonic::core::can_multiply;
    using cljonic::core::div;
    using cljonic::core::mult;

    STATIC_REQUIRE(add(1.5, 2.5) == 4.0);
    STATIC_REQUIRE(can_add(1.5, 2.5));
    STATIC_REQUIRE_FALSE(can_add(std::numeric_limits<double>::infinity(), 1.0));
    volatile double maximum = std::numeric_limits<double>::max();
    REQUIRE_FALSE(can_multiply(maximum, 2.0));
    STATIC_REQUIRE_FALSE(can_div(1.0, 0.0));
    REQUIRE(add(std::numeric_limits<double>::infinity(), 1.0) == 0.0);
    REQUIRE(mult(std::numeric_limits<double>::max(), 2.0) == 0.0);
    REQUIRE(div(1.0, 0.0) == 0.0);

    if constexpr (std::numeric_limits<double>::has_denorm != std::denorm_absent) {
        constexpr double tiny = std::numeric_limits<double>::denorm_min();
        STATIC_REQUIRE_FALSE(can_multiply(tiny, 0.5));
        REQUIRE(mult(tiny, 0.5) == 0.0);
        STATIC_REQUIRE(can_div(tiny, tiny));
        STATIC_REQUIRE(div(tiny, tiny) == 1.0);
    }
}

TEST_CASE("Checked bitwise operations reject invalid shift counts", "[arithmetic]") {
    TRACE_ID("invariant.CheckedArithmetic.OverflowAndUnderflowAreChecked");
    TRACE_ID("invariant.CheckedArithmetic.PreflightMatchesOperation");
    TRACE_ID("invariant.CheckedArithmetic.UnsupportedTypesAreRejected");

    using cljonic::core::bit_and;
    using cljonic::core::bit_and_not;
    using cljonic::core::bit_clear;
    using cljonic::core::bit_flip;
    using cljonic::core::bit_not;
    using cljonic::core::bit_or;
    using cljonic::core::bit_set;
    using cljonic::core::bit_shift_left;
    using cljonic::core::bit_shift_right;
    using cljonic::core::bit_test;
    using cljonic::core::bit_xor;
    using cljonic::core::can_bit_shift_left;
    using cljonic::core::can_bit_shift_right;
    using cljonic::core::can_unsigned_bit_shift_right;
    using cljonic::core::unsigned_bit_shift_right;

    STATIC_REQUIRE(bit_and(0b1100, 0b1010) == 0b1000);
    STATIC_REQUIRE(bit_or(0b1100, 0b1010) == 0b1110);
    STATIC_REQUIRE(bit_xor(0b1100, 0b1010) == 0b0110);
    STATIC_REQUIRE(bit_and_not(0b1100, 0b1000) == 0b0100);
    STATIC_REQUIRE(bit_not(0) == -1);
    STATIC_REQUIRE(bit_set(0, 3) == 8);
    STATIC_REQUIRE(bit_clear(0b1111, 2) == 0b1011);
    STATIC_REQUIRE(bit_flip(0b1010, 0) == 0b1011);
    STATIC_REQUIRE(bit_test(0b1010, 1));
    STATIC_REQUIRE_FALSE(bit_test(0b1010, 0));

    STATIC_REQUIRE(can_bit_shift_left(1, 3));
    STATIC_REQUIRE(bit_shift_left(1, 3) == 8);
    STATIC_REQUIRE_FALSE(can_bit_shift_left(1, std::numeric_limits<int>::digits));
    STATIC_REQUIRE_FALSE(can_bit_shift_left(1, -1));
    STATIC_REQUIRE_FALSE(can_bit_shift_left(std::numeric_limits<int>::max(), 1));
    REQUIRE(bit_shift_left(std::numeric_limits<int>::max(), 1) == 0);

    STATIC_REQUIRE(can_bit_shift_right(-8, 2));
    STATIC_REQUIRE(bit_shift_right(-8, 2) == -2);
    STATIC_REQUIRE(can_unsigned_bit_shift_right(-8, 1));
    STATIC_REQUIRE(unsigned_bit_shift_right(-8, 1) == std::numeric_limits<int>::max() - 3);
    STATIC_REQUIRE_FALSE(can_bit_shift_right(1, std::numeric_limits<int>::digits));
}

TEST_CASE("Checked arithmetic exercises every runtime policy branch", "[arithmetic][coverage]") {
    TRACE_ID("invariant.CheckedArithmetic.OperandDomainIsClosedAndLossless");
    TRACE_ID("invariant.CheckedArithmetic.OverflowAndUnderflowAreChecked");
    TRACE_ID("invariant.CheckedArithmetic.DivisionAndFloatingFailuresAreChecked");
    TRACE_ID("invariant.CheckedArithmetic.QuotRemModSemantics");
    TRACE_ID("invariant.CheckedArithmetic.PreflightMatchesOperation");

    using namespace cljonic::core;
    using cljonic::core::div;

    volatile int positive = 2;
    volatile int negative = -3;
    volatile int zero = 0;
    volatile int maximum = std::numeric_limits<int>::max();
    volatile int minimum = std::numeric_limits<int>::lowest();
    volatile double floating = 2.5;
    volatile double floating_negative = -1.5;
    volatile double floating_zero = 0.0;
    volatile double floating_maximum = std::numeric_limits<double>::max();
    volatile double floating_infinity = std::numeric_limits<double>::infinity();

    REQUIRE(add(positive, negative) == -1);
    REQUIRE(add(negative, negative) == -6);
    REQUIRE(add(minimum, -1) == 0);
    REQUIRE(add(floating, floating_negative) == 1.0);
    REQUIRE_FALSE(can_add(floating_infinity, floating));

    REQUIRE(sub(positive, negative) == 5);
    REQUIRE(sub(minimum, 1) == 0);
    REQUIRE(sub(maximum, -1) == 0);
    REQUIRE(sub(floating, floating_negative) == 4.0);
    REQUIRE(sub(floating_infinity, floating) == 0.0);

    REQUIRE(mult(positive, negative) == -6);
    REQUIRE(mult(negative, positive) == -6);
    REQUIRE(mult(negative, negative) == 9);
    REQUIRE(mult(positive, zero) == 0);
    REQUIRE(mult(positive, minimum) == 0);
    REQUIRE(mult(negative, maximum) == 0);
    REQUIRE(mult(minimum, -1) == 0);
    REQUIRE(mult(floating, floating) == 6.25);
    REQUIRE(mult(floating_maximum, 2.0) == 0.0);
    REQUIRE(mult(floating_infinity, 1.0) == 0.0);

    REQUIRE(div(negative, positive) == -1);
    REQUIRE(div(maximum, positive) == 1073741823);
    REQUIRE(quot(minimum, positive) == -1073741824);
    REQUIRE(rem(-7, positive) == -1);
    REQUIRE(rem(7, -2) == 1);
    REQUIRE(mod(-7, positive) == 1);
    REQUIRE(mod(7, -2) == -1);
    REQUIRE(div(floating, floating) == 1.0);
    REQUIRE(div(floating, floating_zero) == 0.0);
    REQUIRE(div(floating_maximum, 0.5) == 0.0);

    REQUIRE(bit_shift_left(negative, 1) == -6);
    REQUIRE(bit_shift_left(minimum, 1) == 0);
    REQUIRE(bit_shift_left(1, std::numeric_limits<int>::digits) == 0);

    if constexpr (std::numeric_limits<double>::has_denorm != std::denorm_absent) {
        volatile double tiny = std::numeric_limits<double>::denorm_min();
        REQUIRE(mult(tiny, 0.5) == 0.0);
        REQUIRE(div(tiny, 2.0) == 0.0);
    }
}
