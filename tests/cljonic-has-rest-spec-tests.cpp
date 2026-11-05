#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <type_traits>

#ifndef CLJONIC_USE_SINGLE_HEADER
#include <cljonic-has-rest.hpp>
#endif

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename T>
concept HasAdmittedHasRest =
    requires(const T& value) { requires(!std::same_as<decltype(cljonic::has_rest(value)), void>); };
template <typename... T>
concept AllHaveAdmittedHasRest = (HasAdmittedHasRest<T> && ...);
template <typename... T>
concept NoneHaveAdmittedHasRest = (!HasAdmittedHasRest<T> && ...);
} // namespace

TEST_CASE("has_rest exactly forwards has_next across its domain", "[has_rest]") {
    using cljonic::has_next;
    using cljonic::has_rest;

    TRACE_ID("entity-fields.HasRest");
    TRACE_ID("invariant.HasRest.HasRestFreeFunctionSupported");
    TRACE_ID("invariant.HasRest.SupportsExactHasNextDomain");
    TRACE_ID("invariant.HasRest.ReturnsExactlyHasNextResult");
    TRACE_ID("invariant.HasRest.ForwardsWithoutAdditionalPreflight");
    TRACE_ID("invariant.HasRest.PreservesInputValueState");
    TRACE_ID("invariant.HasRest.NoHeapAllocation");
    TRACE_ID("invariant.HasRest.NoRtti");
    TRACE_ID("invariant.HasRest.NoExceptions");
    TRACE_ID("invariant.HasRest.ConstexprCapable");
    TRACE_ID("invariant.HasRest.RejectsUnsupportedDomainWithTargetedDiagnostic");
    TRACE_ID("invariant.HasRest.DiagnosticNamesSupportedDomains");

    constexpr auto values = cljonic::Vector<int, 4>{10, 20, 30};
    constexpr auto map = cljonic::Map<int, int, 4>{cljonic::MapEntry{1, 100}};
    constexpr auto set = cljonic::Set<int, 4>{10, 20};
    constexpr auto queue = cljonic::Queue<int, 4>{10, 20};
    constexpr auto text = cljonic::String<4>{"AB"};
    constexpr auto range = cljonic::Range{0, 4};
    constexpr auto repeated = cljonic::repeat(3, 7);
    constexpr auto iterated = cljonic::iterate([](int value) noexcept { return value + 1; }, 0);
    constexpr auto generated = cljonic::repeatedly(3, [] noexcept { return 7; });
    constexpr auto taken = cljonic::take_while([](int value) noexcept { return value < 3; }, values);
    constexpr auto dropped =
        cljonic::drop_while([](int value) noexcept { return value < 2; }, cljonic::Vector<int, 4>{1, 2, 3});
    constexpr auto cycled_adapter = cljonic::cycle(taken);

    STATIC_REQUIRE(
        AllHaveAdmittedHasRest<decltype(values), decltype(map), decltype(set), decltype(queue), decltype(text),
                               decltype(range), decltype(repeated), decltype(iterated), decltype(generated),
                               decltype(taken), decltype(dropped), decltype(cycled_adapter)>);
    STATIC_REQUIRE(NoneHaveAdmittedHasRest<int, cljonic::MapEntry<int, int>, int[2]>);
    STATIC_REQUIRE((has_rest(values) == has_next(values) && has_rest(map) == has_next(map) &&
                    has_rest(set) == has_next(set) && has_rest(queue) == has_next(queue) &&
                    has_rest(text) == has_next(text) && has_rest(range) == has_next(range) &&
                    has_rest(repeated) == has_next(repeated) && has_rest(iterated) == has_next(iterated) &&
                    has_rest(generated) == has_next(generated) && has_rest(taken) == has_next(taken) &&
                    has_rest(dropped) == has_next(dropped) && has_rest(cycled_adapter) == has_next(cycled_adapter)));
    STATIC_REQUIRE(noexcept(has_rest(values)) && noexcept(has_rest(iterated)));

    const auto original_values = values;
    const auto original_range = range;
    REQUIRE(has_rest(values) == has_next(values));
    REQUIRE(has_rest(range) == has_next(range));
    REQUIRE(cljonic::equal(values, original_values));
    REQUIRE(range == original_range);
}

TEST_CASE("has_rest preserves callback timing from has_next", "[has_rest]") {
    TRACE_ID("invariant.HasRest.CallbackBehaviorMatchesHasNext");
    auto rest_calls = 0;
    const auto rest_producer = cljonic::repeatedly([&rest_calls] noexcept { return ++rest_calls; });
    auto next_calls = 0;
    const auto next_producer = cljonic::repeatedly([&next_calls] noexcept { return ++next_calls; });

    if constexpr (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U) {
        REQUIRE_FALSE(cljonic::has_rest(rest_producer));
        REQUIRE_FALSE(cljonic::has_next(next_producer));
        REQUIRE(rest_calls == 0);
        REQUIRE(next_calls == 0);
    } else {
        REQUIRE(cljonic::has_rest(rest_producer));
        REQUIRE(cljonic::has_next(next_producer));
        REQUIRE(rest_calls == 2);
        REQUIRE(next_calls == 2);
    }
}
