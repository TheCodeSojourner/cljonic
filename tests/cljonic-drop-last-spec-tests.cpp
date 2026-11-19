#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <limits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename Source>
void check_drop_last_prefix(const Source& source, int requested) {
    const auto before = cljonic::core::seq(source);
    const auto result = cljonic::core::drop_last(requested, source);
    const auto retained =
        requested <= 0 ? before.count() : before.count() - std::min<std::size_t>(requested, before.count());
    REQUIRE(result.count() == retained);
    for (std::size_t index = 0; index < retained; ++index) {
        REQUIRE(result(index) == before(index));
    }
    REQUIRE(cljonic::core::equal(cljonic::core::seq(source), before));
}
} // namespace

TEST_CASE("DropLast preserves collection prefixes and source-bound result types", "[drop-last]") {
    TRACE_ID("entity-fields.DropLast");
    TRACE_ID("invariant.DropLast.MinimumArityIsTwo");
    TRACE_ID("invariant.DropLast.MaximumArityIsTwo");
    TRACE_ID("invariant.DropLast.CountIsFirstAndSourceIsSecond");
    TRACE_ID("invariant.DropLast.AllCljonicCollectionsAndProducersAdmitted");
    TRACE_ID("invariant.DropLast.ResultIsIndependentlyOwningVectorOfSourceElements");
    TRACE_ID("invariant.DropLast.CollectionResultCapacityIsDeclaredCapacity");
    TRACE_ID("invariant.DropLast.RuntimeCountDoesNotChangeResultTypeOrCapacity");
    TRACE_ID("invariant.DropLast.ResultIsOriginalWindowPrefixBeforeRequestedSuffix");
    TRACE_ID("invariant.DropLast.MapSetOrderSemanticallyUnordered");
    TRACE_ID("invariant.DropLast.QueueFifoStringCharactersAndMapEntriesPreserved");
    TRACE_ID("invariant.DropLast.NothrowStorageWithoutAdditionalComparisonGate");
    TRACE_ID("invariant.DropLast.NoNestedResultAbsenceSentinelStatusOrPreflight");
    TRACE_ID("invariant.DropLast.ConstexprNoexceptNoHeapNoRttiNonmutating");

    constexpr auto values = cljonic::core::vector(1, 2, 3, 4);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::drop_last(2, values)), cljonic::Vector<int, 4>>);
    STATIC_REQUIRE(noexcept(cljonic::core::drop_last(2, values)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::drop_last(2, values), cljonic::core::vector(1, 2)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::drop_last(-1, values), values));
    STATIC_REQUIRE(cljonic::core::drop_last(100, values).count() == 0);
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::drop_last(2, cljonic::core::queue(1, 2, 3, 4)),
                                        cljonic::core::vector(1, 2)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::drop_last(2, cljonic::core::string<5>("abcd")),
                                        cljonic::core::vector('a', 'b')));

    volatile int base_raw = 1;
    const auto base = base_raw;
    check_drop_last_prefix(cljonic::core::vector(base, base + 1, base + 2, base + 3), 2);
    check_drop_last_prefix(cljonic::core::queue(base, base + 1, base + 2, base + 3), 2);
    check_drop_last_prefix(cljonic::core::hash_map(base, base + 10, base + 1, base + 11, base + 2, base + 12), 2);
    check_drop_last_prefix(cljonic::core::hash_set(base, base + 1, base + 2, base + 3), 2);
    check_drop_last_prefix(cljonic::core::string<5>("abcd"), 2);
}

TEST_CASE("DropLast normalizes counts and bounds producer observation windows", "[drop-last]") {
    TRACE_ID("invariant.DropLast.CountUsesRepeatIntegerAdmission");
    TRACE_ID("invariant.DropLast.NonpositiveCountNormalizesBeforeUnsignedConversion");
    TRACE_ID("invariant.DropLast.ProducerResultCapacityIsObservationCap");
    TRACE_ID("invariant.DropLast.CapacityIsSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.DropLast.OriginalWindowIsBoundedBySourceCountCapacityAndEndpoint");
    TRACE_ID("invariant.DropLast.NonpositiveCountRetainsEntireOriginalWindow");
    TRACE_ID("invariant.DropLast.CountCoveringWindowBoundOrZeroCapacitySkipsTraversal");
    TRACE_ID("invariant.DropLast.ActualEmptySourceIncludingCycleReturnsEmptyWithoutCallbacks");
    TRACE_ID("invariant.DropLast.InvalidCountsSourcesAndAritiesHaveTargetedDiagnostics");

    constexpr auto values = cljonic::core::vector(1, 2, 3, 4);
    STATIC_REQUIRE(cljonic::core::drop_last(short{2}, values).count() == 2);
    STATIC_REQUIRE(cljonic::core::drop_last(std::size_t{2}, values).count() == 2);
    STATIC_REQUIRE(cljonic::core::drop_last(std::numeric_limits<int>::min(), values).count() == 4);
    STATIC_REQUIRE(cljonic::core::drop_last(std::numeric_limits<std::size_t>::max(), values).count() == 0);
    STATIC_REQUIRE(cljonic::core::drop_last(1, cljonic::Vector<int, 0>{}).count() == 0);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::drop_last(2, cljonic::core::repeat(3, 7))),
                                cljonic::Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);

    volatile int count_raw = 2;
    const auto requested = count_raw;
    const auto source = cljonic::core::vector(requested, requested + 1, requested + 2, requested + 3);
    REQUIRE(cljonic::core::equal(cljonic::core::drop_last(requested, source),
                                 cljonic::core::vector(requested, requested + 1)));
    REQUIRE(cljonic::core::equal(cljonic::core::drop_last(0, source), source));
    REQUIRE(cljonic::core::equal(cljonic::core::drop_last(-requested, source), source));
    REQUIRE(cljonic::core::drop_last(requested + 10, source).count() == 0);
    REQUIRE(cljonic::core::drop_last(requested, cljonic::Vector<int, 0>{}).count() == 0);
    REQUIRE(cljonic::core::drop_last(requested, cljonic::core::range(requested, requested + 5)).count() == 3);
}

TEST_CASE("DropLast traverses one original window and preserves callback state", "[drop-last]") {
    TRACE_ID("invariant.DropLast.RemovalDoesNotStartFreshProducerObservationWindow");
    TRACE_ID("invariant.DropLast.PrefixAndRemovedSuffixUseOneOriginalWindowTraversal");
    TRACE_ID("invariant.DropLast.NoAdvancePastWindowBoundOrActualEndpoint");
    TRACE_ID("invariant.DropLast.RepeatedlyCallbackOncePerVisitedValue");
    TRACE_ID("invariant.DropLast.IterateStepsOnlyBetweenVisitedValues");
    TRACE_ID("invariant.DropLast.SourceAndValueHeldCallbackStatePreserved");
    TRACE_ID("invariant.DropLast.ReferencedOrStaticCallbackStateRemainsShared");

    volatile int base_raw = 1;
    volatile int count_raw = 2;
    const auto base = base_raw;
    const auto requested = count_raw;
    REQUIRE(cljonic::core::equal(cljonic::core::drop_last(requested, cljonic::core::range(base, base + 5)),
                                 cljonic::core::vector(base, base + 1, base + 2)));
    const auto cycled = cljonic::core::drop_last(
        requested, cljonic::core::cycle(cljonic::core::vector(base, base + 1, base + 2, base + 3)));
    REQUIRE(cycled.count() == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - requested);
    REQUIRE(cycled(0) == base);
    REQUIRE(cycled(1) == base + 1);
    REQUIRE(cljonic::core::drop_last(requested, cljonic::core::repeat(requested, base)).count() == 0);
    REQUIRE(cljonic::core::equal(
        cljonic::core::drop_last(requested,
                                 cljonic::core::take_while([base](int value) noexcept { return value < base + 4; },
                                                           cljonic::core::vector(base, base + 1, base + 2, base + 3))),
        cljonic::core::vector(base, base + 1)));
    REQUIRE(cljonic::core::equal(
        cljonic::core::drop_last(requested,
                                 cljonic::core::drop_while([base](int value) noexcept { return value < base + 1; },
                                                           cljonic::core::vector(base, base + 1, base + 2, base + 3))),
        cljonic::core::vector(base + 1)));

    auto source = cljonic::core::vector(base, base + 1, base + 2, base + 3);
    const auto result = cljonic::core::drop_last(requested, source);
    source = cljonic::core::vector(base + 10, base + 11, base + 12, base + 13);
    REQUIRE(cljonic::core::equal(result, cljonic::core::vector(base, base + 1)));
}
