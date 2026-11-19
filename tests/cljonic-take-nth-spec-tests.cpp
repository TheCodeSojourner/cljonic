#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <limits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename Source>
void check_take_nth(const Source& source, int stride) {
    const auto before = cljonic::core::seq(source);
    const auto result = cljonic::core::take_nth(stride, source);
    if (stride <= 0) {
        REQUIRE(result.count() == 0);
        return;
    }
    const auto stride_size = static_cast<std::size_t>(stride);
    std::size_t expected = 0;
    for (std::size_t index = 0; index < before.count(); ++index) {
        if (index % stride_size == 0) {
            ++expected;
        }
    }
    REQUIRE(result.count() == expected);
    std::size_t result_index = 0;
    for (std::size_t index = 0; index < before.count(); ++index) {
        if (index % stride_size == 0) {
            REQUIRE(result(result_index++) == before(index));
        }
    }
    REQUIRE(cljonic::core::equal(cljonic::core::seq(source), before));
}
} // namespace

TEST_CASE("TakeNth preserves collection striding and source-bound result types", "[take-nth]") {
    TRACE_ID("entity-fields.TakeNth");
    TRACE_ID("invariant.TakeNth.MinimumArityIsTwo");
    TRACE_ID("invariant.TakeNth.MaximumArityIsTwo");
    TRACE_ID("invariant.TakeNth.StrideIsFirstAndSourceIsSecond");
    TRACE_ID("invariant.TakeNth.AllCljonicCollectionsAndProducersAdmitted");
    TRACE_ID("invariant.TakeNth.ResultIsIndependentlyOwningVectorOfSourceElements");
    TRACE_ID("invariant.TakeNth.CollectionResultCapacityIsDeclaredCapacity");
    TRACE_ID("invariant.TakeNth.RuntimeStrideDoesNotChangeResultTypeOrCapacity");
    TRACE_ID("invariant.TakeNth.ZeroBasedOffsetsBeginAtFirstObservedElement");
    TRACE_ID("invariant.TakeNth.ResultUsesEveryStrideOffset");
    TRACE_ID("invariant.TakeNth.MapSetOrderSemanticallyUnordered");
    TRACE_ID("invariant.TakeNth.QueueFifoStringCharactersAndMapEntriesPreserved");
    TRACE_ID("invariant.TakeNth.NothrowStorageWithoutAdditionalComparisonGate");
    TRACE_ID("invariant.TakeNth.NoNestedResultAbsenceSentinelStatusOrPreflight");
    TRACE_ID("invariant.TakeNth.ConstexprNoexceptNoHeapNoRttiNonmutating");

    constexpr auto values = cljonic::core::vector(1, 2, 3, 4, 5);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::take_nth(2, values)), cljonic::Vector<int, 5>>);
    STATIC_REQUIRE(noexcept(cljonic::core::take_nth(2, values)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take_nth(2, values), cljonic::core::vector(1, 3, 5)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take_nth(1, values), values));
    STATIC_REQUIRE(cljonic::core::take_nth(0, values).count() == 0);
    STATIC_REQUIRE(cljonic::core::take_nth(-1, values).count() == 0);
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take_nth(2, cljonic::core::queue(10, 20, 30, 40)),
                                        cljonic::core::vector(10, 30)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take_nth(2, cljonic::core::string<6>("abcde")),
                                        cljonic::core::vector('a', 'c', 'e')));

    volatile int base_raw = 1;
    const auto base = base_raw;
    check_take_nth(cljonic::core::vector(base, base + 1, base + 2, base + 3), 2);
    check_take_nth(cljonic::core::queue(base, base + 1, base + 2, base + 3), 2);
    check_take_nth(cljonic::core::hash_map(base, base + 10, base + 1, base + 11, base + 2, base + 12), 2);
    check_take_nth(cljonic::core::hash_set(base, base + 1, base + 2, base + 3), 2);
    check_take_nth(cljonic::core::string<5>("abc"), 2);
}

TEST_CASE("TakeNth normalizes strides and bounds producer observation windows", "[take-nth]") {
    TRACE_ID("invariant.TakeNth.StrideUsesRepeatIntegerAdmission");
    TRACE_ID("invariant.TakeNth.NonpositiveStrideNormalizesBeforeUnsignedConversion");
    TRACE_ID("invariant.TakeNth.ProducerResultCapacityIsObservationCap");
    TRACE_ID("invariant.TakeNth.CapacityIsSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.TakeNth.OriginalWindowIsBoundedBySourceCountCapacityAndEndpoint");
    TRACE_ID("invariant.TakeNth.UnitStrideReturnsCompleteObservationWindow");
    TRACE_ID("invariant.TakeNth.NonpositiveZeroCapacityAndEmptySourceReturnEmpty");
    TRACE_ID("invariant.TakeNth.InvalidStridesSourcesAndAritiesHaveTargetedDiagnostics");

    constexpr auto values = cljonic::core::vector(1, 2, 3, 4);
    STATIC_REQUIRE(cljonic::core::take_nth(short{2}, values).count() == 2);
    STATIC_REQUIRE(cljonic::core::take_nth(std::size_t{2}, values).count() == 2);
    STATIC_REQUIRE(cljonic::core::take_nth(std::numeric_limits<int>::min(), values).count() == 0);
    STATIC_REQUIRE(cljonic::core::take_nth(std::numeric_limits<std::size_t>::max(), values).count() == 1);
    STATIC_REQUIRE(cljonic::core::take_nth(2, cljonic::Vector<int, 0>{}).count() == 0);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::take_nth(2, cljonic::core::repeat(3, 7))),
                                cljonic::Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);

    volatile int stride_raw = 2;
    const auto stride = stride_raw;
    const auto source = cljonic::core::vector(stride, stride + 1, stride + 2, stride + 3);
    REQUIRE(cljonic::core::equal(cljonic::core::take_nth(stride, source), cljonic::core::vector(stride, stride + 2)));
    REQUIRE(cljonic::core::take_nth(0, source).count() == 0);
    REQUIRE(cljonic::core::take_nth(-stride, source).count() == 0);
    REQUIRE(cljonic::core::equal(cljonic::core::take_nth(1, source), source));
    REQUIRE(cljonic::core::take_nth(stride, cljonic::Vector<int, 0>{}).count() == 0);
    REQUIRE(cljonic::core::take_nth(stride, cljonic::core::range(stride, stride + 4)).count() == 2);
}

TEST_CASE("TakeNth traverses one original window and preserves callback state", "[take-nth]") {
    TRACE_ID("invariant.TakeNth.OriginalWindowIsTraversedOnce");
    TRACE_ID("invariant.TakeNth.NoAdvancePastWindowBoundOrActualEndpoint");
    TRACE_ID("invariant.TakeNth.RepeatedlyCallbackOncePerVisitedValue");
    TRACE_ID("invariant.TakeNth.IterateStepsOnlyBetweenVisitedValues");
    TRACE_ID("invariant.TakeNth.SourceAndCallbackStatePreserved");
    TRACE_ID("invariant.TakeNth.ReferencedOrStaticCallbackStateRemainsShared");

    volatile int base_raw = 1;
    volatile int stride_raw = 2;
    const auto base = base_raw;
    const auto stride = stride_raw;
    REQUIRE(cljonic::core::equal(cljonic::core::take_nth(stride, cljonic::core::range(base, base + 5)),
                                 cljonic::core::vector(base, base + 2, base + 4)));
    REQUIRE(cljonic::core::equal(cljonic::core::take_nth(stride, cljonic::core::repeat(stride, base)),
                                 cljonic::core::vector(base)));
    REQUIRE(cljonic::core::equal(
        cljonic::core::take_nth(stride,
                                cljonic::core::take_while([base](int value) noexcept { return value < base + 4; },
                                                          cljonic::core::vector(base, base + 1, base + 2, base + 3))),
        cljonic::core::vector(base, base + 2)));
    REQUIRE(cljonic::core::equal(
        cljonic::core::take_nth(stride,
                                cljonic::core::drop_while([base](int value) noexcept { return value < base + 1; },
                                                          cljonic::core::vector(base, base + 1, base + 2, base + 3))),
        cljonic::core::vector(base + 1, base + 3)));

    auto source = cljonic::core::vector(base, base + 1, base + 2);
    const auto result = cljonic::core::take_nth(stride, source);
    source = cljonic::core::vector(base + 10, base + 11, base + 12);
    REQUIRE(cljonic::core::equal(result, cljonic::core::vector(base, base + 2)));
}
