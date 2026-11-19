#include <catch2/catch_test_macros.hpp>

#include <concepts>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename Source>
void check_butlast(const Source& source) {
    const auto before = cljonic::core::seq(source);
    const auto result = cljonic::core::butlast(source);
    const auto retained = before.count() < 2 ? 0 : before.count() - 1;
    REQUIRE(result.count() == retained);
    for (std::size_t index = 0; index < retained; ++index) {
        REQUIRE(result(index) == before(index));
    }
    REQUIRE(cljonic::core::equal(cljonic::core::seq(source), before));
}
} // namespace

TEST_CASE("ButLast preserves collection prefixes and source-bound result types", "[butlast]") {
    TRACE_ID("entity-fields.ButLast");
    TRACE_ID("invariant.ButLast.MinimumArityIsOne");
    TRACE_ID("invariant.ButLast.MaximumArityIsOne");
    TRACE_ID("invariant.ButLast.SourceIsTheOnlyArgument");
    TRACE_ID("invariant.ButLast.AllCljonicCollectionsAndProducersAdmitted");
    TRACE_ID("invariant.ButLast.ResultIsIndependentlyOwningVectorOfSourceElements");
    TRACE_ID("invariant.ButLast.CollectionResultCapacityIsDeclaredCapacity");
    TRACE_ID("invariant.ButLast.FinalElementIsRemovedFromOriginalWindow");
    TRACE_ID("invariant.ButLast.MapSetOrderSemanticallyUnordered");
    TRACE_ID("invariant.ButLast.QueueFifoStringCharactersAndMapEntriesPreserved");
    TRACE_ID("invariant.ButLast.NothrowStorageWithoutAdditionalComparisonGate");
    TRACE_ID("invariant.ButLast.NoNestedResultAbsenceSentinelStatusOrPreflight");
    TRACE_ID("invariant.ButLast.ConstexprNoexceptNoHeapNoRttiNonmutating");

    constexpr auto values = cljonic::core::vector(1, 2, 3, 4);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::butlast(values)), cljonic::Vector<int, 4>>);
    STATIC_REQUIRE(noexcept(cljonic::core::butlast(values)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::butlast(values), cljonic::core::vector(1, 2, 3)));
    STATIC_REQUIRE(cljonic::core::butlast(cljonic::Vector<int, 1>{1}).count() == 0);
    STATIC_REQUIRE(cljonic::core::butlast(cljonic::Vector<int, 0>{}).count() == 0);
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::butlast(cljonic::core::queue(1, 2, 3)), cljonic::core::vector(1, 2)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::butlast(cljonic::core::string<5>("abcd")),
                                        cljonic::core::vector('a', 'b', 'c')));

    volatile int base_raw = 1;
    const auto base = base_raw;
    check_butlast(cljonic::core::vector(base, base + 1, base + 2, base + 3));
    check_butlast(cljonic::core::queue(base, base + 1, base + 2, base + 3));
    check_butlast(cljonic::core::hash_map(base, base + 10, base + 1, base + 11, base + 2, base + 12));
    check_butlast(cljonic::core::hash_set(base, base + 1, base + 2, base + 3));
    check_butlast(cljonic::core::string<5>("abcd"));

    const auto one = cljonic::core::vector(base);
    const auto zero = cljonic::Vector<int, 0>{};
    REQUIRE(cljonic::core::butlast(one).count() == 0);
    REQUIRE(cljonic::core::butlast(zero).count() == 0);
}

TEST_CASE("ButLast bounds producer observation windows and preserves callbacks", "[butlast]") {
    TRACE_ID("invariant.ButLast.ProducerResultCapacityIsObservationCap");
    TRACE_ID("invariant.ButLast.CapacityIsSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.ButLast.OriginalWindowIsBoundedBySourceCountCapacityAndEndpoint");
    TRACE_ID("invariant.ButLast.OneAndZeroElementWindowsReturnEmpty");
    TRACE_ID("invariant.ButLast.ActualEmptySourceIncludingCycleReturnsEmptyWithoutCallbacks");
    TRACE_ID("invariant.ButLast.OriginalWindowIsTraversedOnce");
    TRACE_ID("invariant.ButLast.NoSecondObservationOrTraversalCache");
    TRACE_ID("invariant.ButLast.NoAdvancePastWindowBoundOrActualEndpoint");
    TRACE_ID("invariant.ButLast.RepeatedlyCallbackOncePerVisitedValue");
    TRACE_ID("invariant.ButLast.IterateStepsOnlyBetweenVisitedValues");
    TRACE_ID("invariant.ButLast.SourceAndCallbackStatePreserved");
    TRACE_ID("invariant.ButLast.ReferencedOrStaticCallbackStateRemainsShared");
    TRACE_ID("invariant.ButLast.InvalidSourcesAndAritiesHaveTargetedDiagnostics");

    constexpr auto values = cljonic::core::vector(1, 2, 3);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::butlast(cljonic::core::range(1, 5))),
                                cljonic::Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::butlast(cljonic::core::range(1, 5)), cljonic::core::vector(1, 2, 3)));
    STATIC_REQUIRE(cljonic::core::butlast(cljonic::core::repeat(1, 7)).count() == 0);
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::butlast(cljonic::core::repeat(3, 7)), cljonic::core::vector(7, 7)));
    STATIC_REQUIRE(cljonic::core::butlast(cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0);

    int calls = 0;
    const auto repeatedly = cljonic::core::repeatedly(4, [&calls] noexcept { return ++calls; });
    const auto repeatedly_result = cljonic::core::butlast(repeatedly);
    REQUIRE(repeatedly_result.count() == 3);
    REQUIRE(calls == 4);

    const auto stateful = cljonic::core::repeatedly(4, [value = 1] mutable noexcept { return value++; });
    const auto first = cljonic::core::butlast(stateful);
    const auto second = cljonic::core::butlast(stateful);
    REQUIRE(cljonic::core::equal(first, cljonic::core::vector(1, 2, 3)));
    REQUIRE(cljonic::core::equal(first, second));

    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    const auto iterated_result = cljonic::core::butlast(iterated);
    REQUIRE(iterated_result.count() == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - 1);
    REQUIRE(steps >= 1);
    REQUIRE(cljonic::core::equal(cljonic::core::butlast(values), cljonic::core::vector(1, 2)));
}
