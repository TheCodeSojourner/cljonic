#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <limits>
#include <type_traits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
struct TakeElement {
    int value{};
    constexpr auto operator==(const TakeElement&) const noexcept -> bool = default;
};

template <typename Source>
void check_take_prefix(const Source& source, int requested) {
    const auto before = cljonic::seq(source);
    const auto result = cljonic::take(requested, source);
    REQUIRE(result.count() == std::min<std::size_t>(requested, source.count()));
    auto iterator = source.begin();
    for (std::size_t index = 0; index < result.count(); ++index) {
        REQUIRE(result(index) == *iterator);
        ++iterator;
    }
    REQUIRE(cljonic::equal(cljonic::seq(source), before));
}
} // namespace

TEST_CASE("Take preserves collection prefixes and source-bound result types", "[take]") {
    TRACE_ID("entity-fields.Take");
    TRACE_ID("invariant.Take.MinimumArityIsTwo");
    TRACE_ID("invariant.Take.MaximumArityIsTwo");
    TRACE_ID("invariant.Take.CountIsFirstAndSourceIsSecond");
    TRACE_ID("invariant.Take.AllCljonicCollectionsAndProducersAdmitted");
    TRACE_ID("invariant.Take.ResultIsIndependentlyOwningVectorOfSourceElements");
    TRACE_ID("invariant.Take.CollectionResultCapacityIsDeclaredCapacity");
    TRACE_ID("invariant.Take.RuntimeCountDoesNotChangeResultTypeOrCapacity");
    TRACE_ID("invariant.Take.ResultContainsPrefixInTraversalOrder");
    TRACE_ID("invariant.Take.MapSetOrderSemanticallyUnordered");
    TRACE_ID("invariant.Take.QueueFifoStringCharactersAndMapEntriesPreserved");
    TRACE_ID("invariant.Take.NothrowStorageWithoutAdditionalComparisonGate");
    TRACE_ID("invariant.Take.NoNestedResultAbsenceSentinelStatusOrPreflight");
    TRACE_ID("invariant.Take.ConstexprNoexceptNoHeapNoRttiNonmutating");

    constexpr auto values = cljonic::vector(1, 2, 3);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::take(2, values)), cljonic::Vector<int, 3>>);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::take(0, values)), decltype(cljonic::take(100, values))>);
    STATIC_REQUIRE(noexcept(cljonic::take(2, values)));
    STATIC_REQUIRE(cljonic::equal(cljonic::take(2, values), cljonic::vector(1, 2)));
    STATIC_REQUIRE(cljonic::equal(cljonic::take(2, cljonic::queue(1, 2, 3)), cljonic::vector(1, 2)));
    STATIC_REQUIRE(cljonic::equal(cljonic::take(2, cljonic::string<4>("abc")), cljonic::vector('a', 'b')));
    STATIC_REQUIRE(std::same_as<decltype(cljonic::take(1, cljonic::hash_map(1, 2))),
                                cljonic::Vector<cljonic::MapEntry<int, int>, 1>>);
    STATIC_REQUIRE(cljonic::take(1, cljonic::vector(TakeElement{1}, TakeElement{2}))(0).value == 1);

    volatile int base_raw = 1;
    const auto base = base_raw;
    check_take_prefix(cljonic::vector(base, base + 1, base + 2), 2);
    check_take_prefix(cljonic::queue(base, base + 1, base + 2), 2);
    check_take_prefix(cljonic::hash_map(base, base + 10, base + 1, base + 11, base + 2, base + 12), 2);
    check_take_prefix(cljonic::hash_set(base, base + 1, base + 2), 2);
    check_take_prefix(cljonic::string<4>("abc"), 2);
    auto source = cljonic::vector(base, base + 1, base + 2);
    const auto result = cljonic::take(2, source);
    source = cljonic::vector(base + 10, base + 11, base + 12);
    REQUIRE(cljonic::equal(result, cljonic::vector(base, base + 1)));
}

TEST_CASE("Take normalizes counts and bounds requests by observable capacity", "[take]") {
    TRACE_ID("invariant.Take.CountUsesRepeatIntegerAdmission");
    TRACE_ID("invariant.Take.NonpositiveCountNormalizesBeforeUnsignedConversion");
    TRACE_ID("invariant.Take.ProducerResultCapacityIsObservationCap");
    TRACE_ID("invariant.Take.CapacityIsSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.Take.ResultCountIsMinimumOfRequestTraversalLengthAndCapacity");
    TRACE_ID("invariant.Take.OversizedRequestReturnsObservableSourceWithoutFailure");
    TRACE_ID("invariant.Take.InvalidCountsSourcesAndAritiesHaveTargetedDiagnostics");

    constexpr auto values = cljonic::vector(1, 2, 3);
    STATIC_REQUIRE(cljonic::count(cljonic::take(short{2}, values)) == 2);
    STATIC_REQUIRE(cljonic::count(cljonic::take(std::size_t{2}, values)) == 2);
    STATIC_REQUIRE(cljonic::count(cljonic::take(std::numeric_limits<int>::min(), values)) == 0);
    STATIC_REQUIRE(cljonic::equal(cljonic::take(std::numeric_limits<std::size_t>::max(), values), values));
    STATIC_REQUIRE(cljonic::count(cljonic::take(1, cljonic::Vector<int, 0>{})) == 0);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::take(2, cljonic::repeat(3, 7))),
                                cljonic::Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);

    volatile int count_raw = 2;
    const auto requested = count_raw;
    const auto source = cljonic::vector(requested, requested + 1, requested + 2);
    REQUIRE(cljonic::equal(cljonic::take(requested, source), cljonic::vector(requested, requested + 1)));
    REQUIRE(cljonic::take(0, source).count() == 0);
    REQUIRE(cljonic::take(-requested, source).count() == 0);
    REQUIRE(cljonic::take(requested, cljonic::Vector<int, 0>{}).count() == 0);
    REQUIRE(cljonic::take(requested, cljonic::Vector<int, 4>{}).count() == 0);
    REQUIRE(cljonic::equal(cljonic::take(requested + 10, source), source));
    constexpr auto cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    const auto bounded = cljonic::take(cap + 1, cljonic::repeat(cap + 10, requested));
    REQUIRE(bounded.capacity() == cap);
    REQUIRE(bounded.count() == cap);
    REQUIRE(bounded(0) == requested);
    REQUIRE(bounded(cap - 1) == requested);
    const auto unbounded = cljonic::take(std::numeric_limits<std::size_t>::max(), cljonic::repeat(requested));
    REQUIRE(unbounded.count() == cap);
    REQUIRE(unbounded(cap - 1) == requested);
}

TEST_CASE("Take limits producer evaluation and preserves value-held callback state", "[take]") {
    TRACE_ID("invariant.Take.NonpositiveRequestOrZeroCapacityDoesNotBeginTraversal");
    TRACE_ID("invariant.Take.EmptySourceIncludingCycleReturnsTypedEmptyWithoutCallbacks");
    TRACE_ID("invariant.Take.NoAdvancePastLastReturnedElement");
    TRACE_ID("invariant.Take.IterateStepsOnlyBetweenReturnedElements");
    TRACE_ID("invariant.Take.RepeatedlyCallbackOncePerReturnedElement");
    TRACE_ID("invariant.Take.SourceAndValueHeldCallbackStatePreserved");
    TRACE_ID("invariant.Take.ReferencedCallbackStateRemainsShared");

    volatile int base_raw = 1;
    volatile int count_raw = 3;
    const auto base = base_raw;
    const auto requested = count_raw;
    REQUIRE(cljonic::equal(cljonic::take(requested, cljonic::range(base, base + 5)),
                           cljonic::vector(base, base + 1, base + 2)));
    REQUIRE(cljonic::equal(cljonic::take(requested, cljonic::repeat(2, base)), cljonic::vector(base, base)));
    REQUIRE(cljonic::equal(cljonic::take(requested, cljonic::repeat(base)), cljonic::vector(base, base, base)));
    REQUIRE(cljonic::equal(cljonic::take(requested, cljonic::cycle(cljonic::vector(base, base + 1))),
                           cljonic::vector(base, base + 1, base)));
    REQUIRE(cljonic::take(requested, cljonic::cycle(cljonic::Vector<int, 0>{})).count() == 0);

    int steps = 0;
    const auto iterated = cljonic::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        base);
    REQUIRE(cljonic::take(0, iterated).count() == 0);
    REQUIRE(cljonic::take(-requested, iterated).count() == 0);
    REQUIRE(steps == 0);
    REQUIRE(cljonic::take(1, iterated)(0) == base);
    REQUIRE(steps == 0);
    REQUIRE(cljonic::equal(cljonic::take(requested, iterated), cljonic::vector(base, base + 1, base + 2)));
    REQUIRE(steps == requested - 1);

    int calls = 0;
    const auto produced = cljonic::repeatedly([&calls, base]() noexcept {
        ++calls;
        return base + calls;
    });
    REQUIRE(cljonic::take(0, produced).count() == 0);
    REQUIRE(cljonic::take(-requested, produced).count() == 0);
    REQUIRE(cljonic::take(requested, cljonic::repeatedly(0,
                                                         [&calls]() noexcept {
                                                             ++calls;
                                                             return 0;
                                                         }))
                .count() == 0);
    REQUIRE(calls == 0);
    REQUIRE(cljonic::equal(cljonic::take(requested, produced), cljonic::vector(base + 1, base + 2, base + 3)));
    REQUIRE(calls == requested);
    REQUIRE(cljonic::take(1, produced)(0) == base + requested + 1);
    REQUIRE(calls == requested + 1);
    const auto stateful = cljonic::repeatedly(5, [value = base]() mutable noexcept { return value++; });
    const auto prefix = cljonic::take(requested, stateful);
    REQUIRE(cljonic::equal(prefix, cljonic::vector(base, base + 1, base + 2)));
    REQUIRE(cljonic::equal(prefix, cljonic::take(requested, stateful)));
    REQUIRE(cljonic::count(stateful) == 5);
    REQUIRE(cljonic::equal(cljonic::take(20, stateful), cljonic::vector(base, base + 1, base + 2, base + 3, base + 4)));
}
