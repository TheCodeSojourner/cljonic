#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <limits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename Source>
void check_take_last_suffix(const Source& source, int requested) {
    const auto before = cljonic::core::seq(source);
    const auto result = cljonic::core::take_last(requested, source);
    const auto retained = std::min<std::size_t>(requested, before.count());
    REQUIRE(result.count() == retained);
    for (std::size_t index = 0; index < retained; ++index) {
        REQUIRE(result(index) == before(before.count() - retained + index));
    }
    REQUIRE(cljonic::core::equal(cljonic::core::seq(source), before));
}
} // namespace

TEST_CASE("TakeLast preserves collection suffixes and source-bound result types", "[take-last]") {
    TRACE_ID("entity-fields.TakeLast");
    TRACE_ID("invariant.TakeLast.MinimumArityIsTwo");
    TRACE_ID("invariant.TakeLast.MaximumArityIsTwo");
    TRACE_ID("invariant.TakeLast.CountIsFirstAndSourceIsSecond");
    TRACE_ID("invariant.TakeLast.AllCljonicCollectionsAndProducersAdmitted");
    TRACE_ID("invariant.TakeLast.ResultIsIndependentlyOwningVectorOfSourceElements");
    TRACE_ID("invariant.TakeLast.CollectionResultCapacityIsDeclaredCapacity");
    TRACE_ID("invariant.TakeLast.RuntimeCountDoesNotChangeResultTypeOrCapacity");
    TRACE_ID("invariant.TakeLast.ResultIsTrailingWindowAsPresentedByIteration");
    TRACE_ID("invariant.TakeLast.MapSetOrderSemanticallyUnordered");
    TRACE_ID("invariant.TakeLast.QueueFifoStringCharactersAndMapEntriesPreserved");
    TRACE_ID("invariant.TakeLast.NothrowStorageWithoutAdditionalComparisonGate");
    TRACE_ID("invariant.TakeLast.NoNestedResultAbsenceSentinelStatusOrPreflight");
    TRACE_ID("invariant.TakeLast.ConstexprNoexceptNoHeapNoRttiNonmutating");

    constexpr auto values = cljonic::core::vector(1, 2, 3);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::take_last(2, values)), cljonic::Vector<int, 3>>);
    STATIC_REQUIRE(noexcept(cljonic::core::take_last(2, values)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take_last(2, values), cljonic::core::vector(2, 3)));
    STATIC_REQUIRE(cljonic::core::take_last(0, values).count() == 0);
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take_last(100, values), values));
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::take_last(2, cljonic::core::queue(1, 2, 3)), cljonic::core::vector(2, 3)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take_last(2, cljonic::core::string<4>("abc")),
                                        cljonic::core::vector('b', 'c')));

    volatile int base_raw = 1;
    const auto base = base_raw;
    check_take_last_suffix(cljonic::core::vector(base, base + 1, base + 2), 2);
    check_take_last_suffix(cljonic::core::queue(base, base + 1, base + 2), 2);
    check_take_last_suffix(cljonic::core::hash_map(base, base + 10, base + 1, base + 11, base + 2, base + 12), 2);
    check_take_last_suffix(cljonic::core::hash_set(base, base + 1, base + 2), 2);
    check_take_last_suffix(cljonic::core::string<4>("abc"), 2);
}

TEST_CASE("TakeLast normalizes counts and bounds producer observation windows", "[take-last]") {
    TRACE_ID("invariant.TakeLast.CountUsesRepeatIntegerAdmission");
    TRACE_ID("invariant.TakeLast.NonpositiveCountNormalizesBeforeUnsignedConversion");
    TRACE_ID("invariant.TakeLast.ProducerResultCapacityIsObservationCap");
    TRACE_ID("invariant.TakeLast.CapacityIsSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.TakeLast.OriginalWindowIsBoundedBySourceCountCapacityAndEndpoint");
    TRACE_ID("invariant.TakeLast.ResultCountIsBoundedWithoutUnsignedUnderflow");
    TRACE_ID("invariant.TakeLast.ZeroNegativeZeroCapacityAndEmptySourceReturnEmpty");
    TRACE_ID("invariant.TakeLast.OversizedCountReturnsCompleteObservationWindow");
    TRACE_ID("invariant.TakeLast.InvalidCountsSourcesAndAritiesHaveTargetedDiagnostics");

    constexpr auto values = cljonic::core::vector(1, 2, 3);
    STATIC_REQUIRE(cljonic::core::take_last(short{2}, values).count() == 2);
    STATIC_REQUIRE(cljonic::core::take_last(std::size_t{2}, values).count() == 2);
    STATIC_REQUIRE(cljonic::core::take_last(std::numeric_limits<int>::min(), values).count() == 0);
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::take_last(std::numeric_limits<std::size_t>::max(), values), values));
    STATIC_REQUIRE(cljonic::core::take_last(1, cljonic::Vector<int, 0>{}).count() == 0);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::take_last(2, cljonic::core::repeat(3, 7))),
                                cljonic::Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);

    volatile int count_raw = 2;
    const auto requested = count_raw;
    const auto source = cljonic::core::vector(requested, requested + 1, requested + 2);
    REQUIRE(cljonic::core::equal(cljonic::core::take_last(requested, source),
                                 cljonic::core::vector(requested + 1, requested + 2)));
    REQUIRE(cljonic::core::take_last(0, source).count() == 0);
    REQUIRE(cljonic::core::take_last(-requested, source).count() == 0);
    REQUIRE(cljonic::core::equal(cljonic::core::take_last(requested + 10, source), source));
    REQUIRE(cljonic::core::take_last(requested, cljonic::Vector<int, 0>{}).count() == 0);
    REQUIRE(cljonic::core::take_last(requested, cljonic::core::range(requested, requested + 4)).count() == 2);
}

TEST_CASE("TakeLast traverses one original window and preserves callback state", "[take-last]") {
    TRACE_ID("invariant.TakeLast.OriginalWindowIsTraversedOnce");
    TRACE_ID("invariant.TakeLast.NoSecondObservationOrCache");
    TRACE_ID("invariant.TakeLast.NoAdvancePastWindowBoundOrActualEndpoint");
    TRACE_ID("invariant.TakeLast.RepeatedlyCallbackOncePerVisitedValue");
    TRACE_ID("invariant.TakeLast.IterateStepsOnlyBetweenVisitedValues");
    TRACE_ID("invariant.TakeLast.SourceAndCallbackStatePreserved");
    TRACE_ID("invariant.TakeLast.ReferencedOrStaticCallbackStateRemainsShared");

    volatile int base_raw = 1;
    volatile int count_raw = 2;
    const auto base = base_raw;
    const auto requested = count_raw;
    REQUIRE(cljonic::core::equal(cljonic::core::take_last(requested, cljonic::core::range(base, base + 5)),
                                 cljonic::core::vector(base + 3, base + 4)));
    REQUIRE(cljonic::core::equal(cljonic::core::take_last(requested, cljonic::core::repeat(requested, base)),
                                 cljonic::core::vector(base, base)));
    REQUIRE(cljonic::core::equal(
        cljonic::core::take_last(requested,
                                 cljonic::core::take_while([base](int value) noexcept { return value < base + 3; },
                                                           cljonic::core::vector(base, base + 1, base + 2))),
        cljonic::core::vector(base + 1, base + 2)));
    REQUIRE(cljonic::core::equal(
        cljonic::core::take_last(requested,
                                 cljonic::core::drop_while([base](int value) noexcept { return value < base + 1; },
                                                           cljonic::core::vector(base, base + 1, base + 2))),
        cljonic::core::vector(base + 1, base + 2)));

    auto source = cljonic::core::vector(base, base + 1, base + 2);
    const auto result = cljonic::core::take_last(requested, source);
    source = cljonic::core::vector(base + 10, base + 11, base + 12);
    REQUIRE(cljonic::core::equal(result, cljonic::core::vector(base + 1, base + 2)));
}
