#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <type_traits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

TEST_CASE("Comp callable composition operations", "[comp]") {
    using namespace cljonic;
    using namespace cljonic::core;

    TRACE_ID("entity-fields.Comp");
    TRACE_ID("invariant.Comp.CompAcceptsZeroOrMoreFunctions");
    TRACE_ID("invariant.Comp.CompIdentityBehaviorForZeroFunctions");
    TRACE_ID("invariant.Comp.CompEquivalentCallableForOneFunction");
    TRACE_ID("invariant.Comp.CompInvokesMultipleFunctionsRightToLeft");
    TRACE_ID("invariant.Comp.CompChecksCompatibleResultAndParameterChain");
    TRACE_ID("invariant.Comp.CompRejectsIncompatibleChain");
    TRACE_ID("invariant.Comp.CompRejectsThrowingCopy");
    TRACE_ID("invariant.Comp.CompRejectsThrowingInvocation");
    TRACE_ID("invariant.Comp.CompSupportsCompileTimeEvaluation");
    TRACE_ID("invariant.Comp.CompIsNothrow");
    TRACE_ID("invariant.Comp.CompIsNonMutating");
    TRACE_ID("invariant.Comp.CompIsNonAllocating");
    TRACE_ID("invariant.Comp.CompHasNoRtti");
    TRACE_ID("invariant.Comp.CompHasNoExceptions");
    TRACE_ID("invariant.Comp.CompUsesSingleThreadedExecution");
    TRACE_ID("invariant.Comp.CompIsReferentiallyTransparent");

    constexpr auto zero_function_composition = comp();
    constexpr auto increment = [](int value) noexcept { return value + 1; };
    constexpr auto increment_composed = comp(increment);
    constexpr auto twice = comp(increment, increment);
    constexpr auto thrice = comp(increment, increment, increment);

    STATIC_REQUIRE(zero_function_composition(42) == 42);
    STATIC_REQUIRE(comp(increment)(5) == 6);
    STATIC_REQUIRE(increment_composed(5) == 6);
    STATIC_REQUIRE(twice(5) == 7);
    STATIC_REQUIRE(thrice(5) == 8);
    STATIC_REQUIRE(noexcept(zero_function_composition(42)));
    STATIC_REQUIRE(noexcept(increment_composed(42)));
    STATIC_REQUIRE(noexcept(twice(42)));

    const auto runtime_value = 5;
    const auto runtime_twice = twice(runtime_value);
    REQUIRE(zero_function_composition(42) == 42);
    REQUIRE(comp(increment)(runtime_value) == 6);
    REQUIRE(runtime_twice == 7);
    REQUIRE(thrice(runtime_value) == 8);
}
