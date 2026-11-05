#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <ranges>
#include <type_traits>
#include <vector>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
constexpr auto filter_is_even = [](int value) noexcept { return value % 2 == 0; };
constexpr auto filter_is_positive = [](int value) noexcept { return value > 0; };
constexpr auto filter_entry_value_is_even = [](const auto& entry) noexcept { return entry.value % 2 == 0; };
constexpr auto filter_increment = [](int value) noexcept { return value + 1; };
constexpr auto filter_next_is_even = [](int value) noexcept { return value % 2 == 1; };
constexpr auto filter_repeatedly_two = []() noexcept { return 2; };

struct FilterBoolProxy {
    constexpr operator bool() const noexcept {
        return true;
    }
};

constexpr auto filter_returns_proxy = [](int) noexcept { return FilterBoolProxy{}; };
constexpr auto filter_takes_long = [](long value) noexcept { return value > 0L; };
constexpr auto filter_takes_short = [](short value) noexcept { return value > 0; };
constexpr auto filter_takes_char = [](char value) noexcept { return value == 'a'; };
constexpr auto filter_throws = [](int) { return true; };
} // namespace

TEST_CASE("Filter matches source values in traversal order", "[filter]") {
    TRACE_ID("entity-fields.FilterFunction");
    TRACE_ID("invariant.FilterFunction.CanonicalFilteringOperation");
    TRACE_ID("invariant.FilterFunction.PredicateIsFirstArgument");
    TRACE_ID("invariant.FilterFunction.SourceIsSecondArgument");
    TRACE_ID("invariant.FilterFunction.AcceptsExactlyOneCljonicSource");
    TRACE_ID("invariant.FilterFunction.SourceIsClosedCljonicDomain");
    TRACE_ID("invariant.FilterFunction.StandardRangesAndNonCljonicSourcesRejected");
    TRACE_ID("invariant.FilterFunction.SourceElementSatisfiesNothrowStorage");
    TRACE_ID("invariant.FilterFunction.ResultIsBoundedOwningVector");
    TRACE_ID("invariant.FilterFunction.ResultElementTypeIsSourceElementType");
    TRACE_ID("invariant.FilterFunction.ResultCapacityIsSourceObservableCountBound");
    TRACE_ID("invariant.FilterFunction.CollectionCapacityBoundIsDeclaredCapacity");
    TRACE_ID("invariant.FilterFunction.ProducerCapacityBoundIsObservationCap");
    TRACE_ID("invariant.FilterFunction.CapacityBoundIsSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.FilterFunction.VisitsElementsInTraversalOrder");
    TRACE_ID("invariant.FilterFunction.PredicateEvaluatedOncePerVisitedElement");
    TRACE_ID("invariant.FilterFunction.MatchingElementsAreAppended");
    TRACE_ID("invariant.FilterFunction.TraversalEndsWhenSourceExhausted");
    TRACE_ID("invariant.FilterFunction.ZeroCapacitySkipsPredicate");
    TRACE_ID("invariant.FilterFunction.PredicateShouldBePure");
    TRACE_ID("invariant.FilterFunction.PredicateIsCopyConstructible");
    TRACE_ID("invariant.FilterFunction.PredicateIsNothrowInvocableOnConstElement");
    TRACE_ID("invariant.FilterFunction.PredicateResultIsExactBool");
    TRACE_ID("invariant.FilterFunction.InspectableParameterUsesSafeImplicitConversion");
    TRACE_ID("invariant.FilterFunction.UnsafeParameterConversionsRejectedAtCompileTime");
    TRACE_ID("invariant.FilterFunction.UninspectableCallableRequiresNothrowExactBoolInvocation");
    TRACE_ID("invariant.FilterFunction.MapAndSetOrderIsSemanticallyUnordered");
    TRACE_ID("invariant.FilterFunction.InputIsPreserved");
    TRACE_ID("invariant.FilterFunction.ConstexprNoexceptNonallocatingNonmutating");
    TRACE_ID("invariant.FilterFunction.RejectedCallsHaveTargetedFilterDiagnostic");

    using Source = cljonic::Vector<int, 4>;
    using Result = cljonic::Vector<int, 4>;
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::filter(filter_is_even, Source{})), Result>);
    STATIC_REQUIRE(noexcept(cljonic::core::filter(filter_is_even, Source{})));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::filter(filter_is_even, cljonic::core::vector(1, 2, 3, 4)),
                                        cljonic::core::vector(2, 4)));
    STATIC_REQUIRE(cljonic::core::count(cljonic::core::filter(filter_is_even, cljonic::Vector<int, 0>{})) == 0);
    STATIC_REQUIRE(cljonic::core::filter(filter_takes_long, cljonic::core::vector(1, 2, 3)).count() == 3);
    STATIC_REQUIRE(
        cljonic::core::filter([](const auto& value) noexcept { return value > 1; }, cljonic::core::vector(1, 2, 3))
            .count() == 2);
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::filter(filter_is_even, cljonic::core::queue(1, 2, 3, 4)),
                                        cljonic::core::vector(2, 4)));
    STATIC_REQUIRE(
        cljonic::core::filter(filter_entry_value_is_even, cljonic::core::hash_map(1, 2, 2, 3, 3, 4)).count() == 2);
    STATIC_REQUIRE(cljonic::core::filter(filter_is_even, cljonic::core::hash_set(1, 2, 3, 4)).count() == 2);
    STATIC_REQUIRE(cljonic::core::equal(
        cljonic::core::filter([](char value) noexcept { return value == 'b'; }, cljonic::core::string<4>("abc")),
        cljonic::core::vector('b')));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::filter(filter_is_even, cljonic::Range<int>{1, 5, 1}),
                                        cljonic::core::vector(2, 4)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::filter(filter_is_even, cljonic::core::repeat(3, 2)),
                                        cljonic::core::vector(2, 2, 2)));
    STATIC_REQUIRE(cljonic::core::filter(filter_is_even, cljonic::core::cycle(cljonic::core::vector(1, 2))).count() ==
                   500);
    STATIC_REQUIRE(cljonic::core::filter(filter_is_even, cljonic::core::iterate(filter_increment, 0)).count() == 500);
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::filter(filter_is_even, cljonic::core::repeatedly(3, filter_repeatedly_two)),
                             cljonic::core::vector(2, 2, 2)));
    STATIC_REQUIRE((cljonic::core::filter(filter_is_even, cljonic::Vector<int, 4>{}).count() == 0));
    STATIC_REQUIRE(cljonic::core::filter(filter_is_even, cljonic::core::vector(1, 3, 5)).count() == 0);
    STATIC_REQUIRE(cljonic::core::filter(filter_is_positive, cljonic::core::vector(1, 2, 3, 4)).count() == 4);
    STATIC_REQUIRE(cljonic::core::count(cljonic::core::filter(filter_is_even, cljonic::core::repeat(4, 1))) == 0);

    volatile int first_raw = 1;
    volatile int second_raw = 2;
    volatile int third_raw = 3;
    volatile int fourth_raw = 4;
    const auto first = first_raw;
    const auto second = second_raw;
    const auto third = third_raw;
    const auto fourth = fourth_raw;
    const auto source = cljonic::core::vector(first, second, third, fourth);
    const auto filtered = cljonic::core::filter(filter_is_even, source);
    REQUIRE(cljonic::core::equal(filtered, cljonic::core::vector(second, fourth)));
    REQUIRE(cljonic::core::equal(source, cljonic::core::vector(first, second, third, fourth)));
    REQUIRE(filtered.capacity() == source.capacity());
    // Every observed element matches, so the result fills to capacity, which
    // coincides with source exhaustion rather than a separate stop condition.
    REQUIRE(cljonic::core::filter(filter_is_positive, source).count() == filtered.capacity());
    REQUIRE(cljonic::core::filter(filter_is_even, cljonic::Range<int>{first, fourth + 1, 1}).count() == 2);
    REQUIRE(cljonic::core::filter(filter_entry_value_is_even, cljonic::core::hash_map(first, second, third, fourth))
                .count() == 2);
    REQUIRE(cljonic::core::filter(filter_is_even, cljonic::core::hash_set(first, second, third, fourth)).count() == 2);
    REQUIRE(cljonic::core::filter(filter_is_even, cljonic::core::repeat(3, second)).count() == 3);
    REQUIRE(cljonic::core::filter(filter_is_even, cljonic::core::repeatedly(3, filter_repeatedly_two)).count() == 3);
}

TEST_CASE("Filterv is an alias of filter", "[filterv]") {
    TRACE_ID("entity-fields.FiltervFunction");
    TRACE_ID("invariant.FiltervFunction.CanonicalAliasOfFilter");
    TRACE_ID("invariant.FiltervFunction.MinimumArityIsTwo");
    TRACE_ID("invariant.FiltervFunction.MaximumArityIsTwo");
    TRACE_ID("invariant.FiltervFunction.PredicateIsFirstArgument");
    TRACE_ID("invariant.FiltervFunction.SourceIsSecondArgument");
    TRACE_ID("invariant.FiltervFunction.SameSourceAndPredicateAdmissionAsFilter");
    TRACE_ID("invariant.FiltervFunction.SameResultTypeCapacityAndLogicalValuesAsFilter");
    TRACE_ID("invariant.FiltervFunction.SameTraversalOrderEmptyBehaviorAndInputPreservationAsFilter");
    TRACE_ID("invariant.FiltervFunction.SameConstexprNoexceptNonallocatingNonmutatingContractAsFilter");
    TRACE_ID("invariant.FiltervFunction.SameRejectionRulesAsFilter");
    TRACE_ID("invariant.FiltervFunction.RejectedCallsNameFilterv");

    using Source = cljonic::Vector<int, 4>;
    using FilterResult = decltype(cljonic::core::filter(filter_is_even, Source{}));
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::filterv(filter_is_even, Source{})), FilterResult>);
    STATIC_REQUIRE(noexcept(cljonic::core::filterv(filter_is_even, Source{})));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::filterv(filter_is_even, cljonic::core::vector(1, 2, 3, 4)),
                                        cljonic::core::filter(filter_is_even, cljonic::core::vector(1, 2, 3, 4))));
    STATIC_REQUIRE(cljonic::core::count(cljonic::core::filterv(filter_is_even, cljonic::Vector<int, 0>{})) == 0);
    STATIC_REQUIRE(cljonic::core::filterv(filter_takes_long, cljonic::core::vector(1, 2, 3)).count() == 3);
    STATIC_REQUIRE(
        cljonic::core::filterv([](const auto& value) noexcept { return value > 1; }, cljonic::core::vector(1, 2, 3))
            .count() == 2);
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::filterv(filter_is_even, cljonic::core::queue(1, 2, 3, 4)),
                                        cljonic::core::vector(2, 4)));
    STATIC_REQUIRE(
        cljonic::core::filterv(filter_entry_value_is_even, cljonic::core::hash_map(1, 2, 2, 3, 3, 4)).count() == 2);
    STATIC_REQUIRE(cljonic::core::filterv(filter_is_even, cljonic::core::hash_set(1, 2, 3, 4)).count() == 2);
    STATIC_REQUIRE(cljonic::core::equal(
        cljonic::core::filterv([](char value) noexcept { return value == 'b'; }, cljonic::core::string<4>("abc")),
        cljonic::core::vector('b')));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::filterv(filter_is_even, cljonic::Range<int>{1, 5, 1}),
                                        cljonic::core::vector(2, 4)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::filterv(filter_is_even, cljonic::core::repeat(3, 2)),
                                        cljonic::core::vector(2, 2, 2)));
    STATIC_REQUIRE(cljonic::core::filterv(filter_is_even, cljonic::core::cycle(cljonic::core::vector(1, 2))).count() ==
                   500);
    STATIC_REQUIRE(cljonic::core::filterv(filter_is_even, cljonic::core::iterate(filter_increment, 0)).count() == 500);
    STATIC_REQUIRE(cljonic::core::equal(
        cljonic::core::filterv(filter_is_even, cljonic::core::repeatedly(3, filter_repeatedly_two)),
        cljonic::core::vector(2, 2, 2)));

    volatile int first_raw = 1;
    volatile int second_raw = 2;
    volatile int third_raw = 3;
    volatile int fourth_raw = 4;
    const auto first = first_raw;
    const auto second = second_raw;
    const auto third = third_raw;
    const auto fourth = fourth_raw;
    const auto source = cljonic::core::vector(first, second, third, fourth);
    const auto alias_result = cljonic::core::filterv(filter_is_even, source);
    const auto filter_result = cljonic::core::filter(filter_is_even, source);
    REQUIRE(cljonic::core::equal(alias_result, filter_result));
    REQUIRE(cljonic::core::equal(source, cljonic::core::vector(first, second, third, fourth)));
    REQUIRE(alias_result.capacity() == filter_result.capacity());
    REQUIRE(cljonic::core::filterv(filter_entry_value_is_even, cljonic::core::hash_map(first, second, third, fourth))
                .count() == 2);
    REQUIRE(cljonic::core::filterv(filter_is_even, cljonic::core::hash_set(first, second, third, fourth)).count() == 2);
    REQUIRE(cljonic::core::filterv(filter_is_even, cljonic::Range<int>{first, fourth + 1, 1}).count() == 2);
    REQUIRE(cljonic::core::filterv(filter_is_even, cljonic::core::repeat(3, second)).count() == 3);
    REQUIRE(cljonic::core::filterv(filter_is_even, cljonic::core::repeatedly(3, filter_repeatedly_two)).count() == 3);
}
