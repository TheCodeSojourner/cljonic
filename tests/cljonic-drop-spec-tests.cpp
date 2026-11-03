#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <limits>
#include <utility>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
struct DropElement {
    int value{};
};

template <typename Source>
void check_drop_suffix(const Source& source, int requested) {
    const auto before = cljonic::seq(source);
    const auto result = cljonic::drop(requested, source);
    REQUIRE(result.capacity() == before.capacity());
    REQUIRE(result.count() == before.count() - static_cast<std::size_t>(requested));
    for (std::size_t index = 0; index < result.count(); ++index) {
        REQUIRE(result(index) == before(index + static_cast<std::size_t>(requested)));
    }
    REQUIRE(cljonic::equal(cljonic::seq(source), before));
}
} // namespace

TEST_CASE("Drop returns independent source-bound collection suffixes", "[drop]") {
    TRACE_ID("entity-fields.Drop");
    TRACE_ID("invariant.Drop.MinimumArityIsTwo");
    TRACE_ID("invariant.Drop.MaximumArityIsTwo");
    TRACE_ID("invariant.Drop.CountIsFirstAndSourceIsSecond");
    TRACE_ID("invariant.Drop.AllCljonicCollectionsAndProducersAdmitted");
    TRACE_ID("invariant.Drop.ResultIsIndependentlyOwningVectorOfSourceElements");
    TRACE_ID("invariant.Drop.CollectionResultCapacityIsDeclaredCapacity");
    TRACE_ID("invariant.Drop.RuntimeCountDoesNotChangeResultTypeOrCapacity");
    TRACE_ID("invariant.Drop.ResultIsOriginalWindowSuffixAfterRequestedPrefix");
    TRACE_ID("invariant.Drop.MapSetOrderSemanticallyUnordered");
    TRACE_ID("invariant.Drop.QueueFifoStringCharactersAndMapEntriesPreserved");
    TRACE_ID("invariant.Drop.NothrowStorageWithoutAdditionalComparisonGate");
    TRACE_ID("invariant.Drop.SuffixIsCompleteWithinOriginalBoundedWindow");
    TRACE_ID("invariant.Drop.NoNestedResultAbsenceSentinelStatusOrPreflight");
    TRACE_ID("invariant.Drop.ConstexprNoexceptNoHeapNoRttiNonmutating");

    constexpr auto values = cljonic::vector<int, 5>(1, 2, 3);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::drop(2, values)), cljonic::Vector<int, 5>>);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::drop(0, values)), decltype(cljonic::drop(100, values))>);
    STATIC_REQUIRE(noexcept(cljonic::drop(2, values)));
    STATIC_REQUIRE(cljonic::equal(cljonic::drop(2, values), cljonic::vector(3)));
    STATIC_REQUIRE(cljonic::equal(cljonic::drop(1, cljonic::queue(1, 2, 3)), cljonic::vector(2, 3)));
    STATIC_REQUIRE(cljonic::equal(cljonic::drop(1, cljonic::string<4>("abc")), cljonic::vector('b', 'c')));
    STATIC_REQUIRE(std::same_as<decltype(cljonic::drop(1, cljonic::hash_map(1, 2))),
                                cljonic::Vector<cljonic::MapEntry<int, int>, 1>>);
    STATIC_REQUIRE(cljonic::drop(1, cljonic::vector(DropElement{1}, DropElement{2}))(0).value == 2);

    volatile int base_raw = 1;
    const auto base = base_raw;
    check_drop_suffix(cljonic::vector<int, 5>(base, base + 1, base + 2), 1);
    check_drop_suffix(cljonic::queue(base, base + 1, base + 2), 1);
    check_drop_suffix(cljonic::hash_map(base, base + 10, base + 1, base + 11, base + 2, base + 12), 1);
    check_drop_suffix(cljonic::hash_set(base, base + 1, base + 2), 1);
    check_drop_suffix(cljonic::string<4>("abc"), 1);
    auto source = cljonic::vector(base, base + 1, base + 2);
    const auto result = cljonic::drop(1, source);
    source = cljonic::vector(base + 10, base + 11, base + 12);
    REQUIRE(cljonic::equal(result, cljonic::vector(base + 1, base + 2)));
}

TEST_CASE("Drop normalizes counts within the original observation window", "[drop]") {
    TRACE_ID("invariant.Drop.CountUsesRepeatIntegerAdmission");
    TRACE_ID("invariant.Drop.NonpositiveCountNormalizesBeforeUnsignedConversion");
    TRACE_ID("invariant.Drop.ProducerResultCapacityIsObservationCap");
    TRACE_ID("invariant.Drop.CapacityIsSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.Drop.OriginalWindowIsBoundedBySourceCountCapacityAndEndpoint");
    TRACE_ID("invariant.Drop.DroppingDoesNotStartFreshProducerObservationWindow");
    TRACE_ID("invariant.Drop.ResultCountSubtractionDoesNotUnderflow");
    TRACE_ID("invariant.Drop.NonpositiveCountRetainsEntireOriginalWindow");
    TRACE_ID("invariant.Drop.InvalidCountsSourcesAndAritiesHaveTargetedDiagnostics");

    constexpr auto values = cljonic::vector(1, 2, 3);
    STATIC_REQUIRE(cljonic::drop(short{2}, values).count() == 1);
    STATIC_REQUIRE(cljonic::drop(std::size_t{2}, values).count() == 1);
    STATIC_REQUIRE(cljonic::equal(cljonic::drop(std::numeric_limits<int>::min(), values), values));
    STATIC_REQUIRE(cljonic::drop(std::numeric_limits<std::size_t>::max(), values).count() == 0);
    STATIC_REQUIRE(cljonic::drop(0, cljonic::Vector<int, 0>{}).count() == 0);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::drop(1, cljonic::repeat(3, 7))),
                                cljonic::Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);

    volatile int count_raw = 2;
    const auto requested = count_raw;
    const auto source = cljonic::vector(requested, requested + 1, requested + 2);
    REQUIRE(cljonic::equal(cljonic::drop(requested, source), cljonic::vector(requested + 2)));
    REQUIRE(cljonic::equal(cljonic::drop(0, source), source));
    REQUIRE(cljonic::equal(cljonic::drop(-requested, source), source));
    REQUIRE(cljonic::drop(3, source).count() == 0);
    REQUIRE(cljonic::drop(std::numeric_limits<std::size_t>::max(), source).count() == 0);
    REQUIRE(cljonic::drop(requested, cljonic::Vector<int, 0>{}).count() == 0);
    REQUIRE(cljonic::drop(requested, cljonic::Vector<int, 4>{}).count() == 0);
    constexpr auto cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    const auto finite = cljonic::drop(requested, cljonic::repeat(cap + 10, requested));
    const auto unbounded = cljonic::drop(requested, cljonic::repeat(requested));
    REQUIRE(finite.capacity() == cap);
    REQUIRE(finite.count() == cap - requested);
    REQUIRE(finite(0) == requested);
    REQUIRE(unbounded.count() == cap - requested);
    REQUIRE(unbounded(unbounded.count() - 1) == requested);
    REQUIRE(cljonic::drop(cap, cljonic::repeat(requested)).count() == 0);
    REQUIRE(cljonic::drop(cap + 1, cljonic::repeat(cap + 10, requested)).count() == 0);
    REQUIRE(cljonic::equal(cljonic::drop(1, cljonic::range(requested, requested + 4)),
                           cljonic::vector(requested + 1, requested + 2, requested + 3)));
    const auto cycled = cljonic::drop(1, cljonic::cycle(cljonic::vector(requested, requested + 1)));
    REQUIRE(cycled.count() == cap - 1);
    REQUIRE(cycled(0) == requested + 1);
    REQUIRE(cycled(cycled.count() - 1) == requested + static_cast<int>((cap - 1) % 2));
}

TEST_CASE("Drop bounds callback evaluation and preserves producer values", "[drop]") {
    TRACE_ID("invariant.Drop.CountCoveringWindowBoundOrZeroCapacitySkipsTraversal");
    TRACE_ID("invariant.Drop.EmptySourceIncludingCycleReturnsTypedEmptyWithoutCallbacks");
    TRACE_ID("invariant.Drop.SkippedAndRetainedValuesUseOneOriginalWindowTraversal");
    TRACE_ID("invariant.Drop.NoAdvancePastFinalRequiredValueOrActualEndpoint");
    TRACE_ID("invariant.Drop.RepeatedlyCallbackOncePerVisitedValueIncludingSkipped");
    TRACE_ID("invariant.Drop.IterateStepsOnlyBetweenVisitedValues");
    TRACE_ID("invariant.Drop.SourceAndValueHeldCallbackStatePreserved");
    TRACE_ID("invariant.Drop.ReferencedOrStaticCallbackStateRemainsShared");

    volatile int requested_raw = 1;
    const auto requested = requested_raw;
    constexpr auto cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    REQUIRE(cljonic::drop(requested, cljonic::cycle(cljonic::Vector<int, 0>{})).count() == 0);
    REQUIRE(cljonic::drop(0, cljonic::cycle(cljonic::Vector<int, 2>{})).count() == 0);
    int calls = 0;
    const auto produced = cljonic::repeatedly(4, [&calls] noexcept { return ++calls; });
    REQUIRE(cljonic::drop(4, produced).count() == 0);
    REQUIRE(cljonic::drop(100, produced).count() == 0);
    REQUIRE(cljonic::drop(1, cljonic::repeatedly(0, [&calls] noexcept { return ++calls; })).count() == 0);
    const auto empty_cycle = cljonic::cycle(cljonic::repeatedly(0, [&calls] noexcept { return ++calls; }));
    REQUIRE(cljonic::drop(requested, empty_cycle).count() == 0);
    REQUIRE(calls == 0);
    REQUIRE(cljonic::equal(cljonic::drop(requested, produced), cljonic::vector(2, 3, 4)));
    REQUIRE(calls == 4);
    REQUIRE(cljonic::equal(cljonic::drop(-requested, produced), cljonic::vector(5, 6, 7, 8)));
    REQUIRE(calls == 8);
    const auto stateful = cljonic::repeatedly(4, [value = 1] mutable noexcept { return value++; });
    REQUIRE(cljonic::equal(cljonic::drop(requested, stateful), cljonic::vector(2, 3, 4)));
    REQUIRE(cljonic::equal(cljonic::drop(requested, stateful), cljonic::vector(2, 3, 4)));
    REQUIRE(stateful.count() == 4);

    int steps = 0;
    const auto iterated = cljonic::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    REQUIRE(cljonic::drop(cap, iterated).count() == 0);
    REQUIRE(steps == 0);
    const auto suffix = cljonic::drop(requested, iterated);
    REQUIRE(suffix.count() == cap - requested);
    REQUIRE(suffix(0) == 2);
    REQUIRE(suffix(suffix.count() - 1) == static_cast<int>(cap));
    REQUIRE(std::cmp_equal(steps, cap - 1));
    calls = 0;
    const auto unbounded = cljonic::repeatedly([&calls] noexcept { return ++calls; });
    REQUIRE(cljonic::drop(cap, unbounded).count() == 0);
    REQUIRE(calls == 0);
    const auto generated = cljonic::drop(requested, unbounded);
    REQUIRE(generated.count() == cap - requested);
    REQUIRE(generated(0) == 2);
    REQUIRE(generated(generated.count() - 1) == static_cast<int>(cap));
    REQUIRE(std::cmp_equal(calls, cap));
    calls = 0;
    const auto longer = cljonic::repeatedly(cap + 3, [&calls] noexcept { return ++calls; });
    const auto bounded = cljonic::drop(requested, longer);
    REQUIRE(bounded.count() == cap - requested);
    REQUIRE(bounded(0) == 2);
    REQUIRE(bounded(bounded.count() - 1) == static_cast<int>(cap));
    REQUIRE(std::cmp_equal(calls, cap));
    calls = 0;
    REQUIRE(cljonic::drop(cap, longer).count() == 0);
    REQUIRE(calls == 0);
    REQUIRE(cljonic::equal(cljonic::drop(0, cljonic::repeatedly(1, [&calls] noexcept { return ++calls; })),
                           cljonic::vector(1)));
    REQUIRE(calls == 1);
}
