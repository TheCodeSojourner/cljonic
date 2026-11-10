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
    const auto before = cljonic::core::seq(source);
    const auto result = cljonic::core::take(requested, source);
    REQUIRE(result.count() == std::min<std::size_t>(requested, source.count()));
    auto iterator = source.begin();
    for (std::size_t index = 0; index < result.count(); ++index) {
        REQUIRE(result(index) == *iterator);
        ++iterator;
    }
    REQUIRE(cljonic::core::equal(cljonic::core::seq(source), before));
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

    constexpr auto values = cljonic::core::vector(1, 2, 3);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::take(2, values)), cljonic::Vector<int, 3>>);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::take(0, values)), decltype(cljonic::core::take(100, values))>);
    STATIC_REQUIRE(noexcept(cljonic::core::take(2, values)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take(2, values), cljonic::core::vector(1, 2)));
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::take(2, cljonic::core::queue(1, 2, 3)), cljonic::core::vector(1, 2)));
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::take(2, cljonic::core::string<4>("abc")), cljonic::core::vector('a', 'b')));
    constexpr auto take_while_source =
        cljonic::core::take_while([](int value) noexcept { return value < 3; }, cljonic::core::vector(1, 2, 4));
    constexpr auto drop_while_source =
        cljonic::core::drop_while([](int value) noexcept { return value < 3; }, cljonic::core::vector(1, 2, 4));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take(2, take_while_source), cljonic::core::vector(1, 2)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take(2, drop_while_source), cljonic::core::vector(4)));
    constexpr auto map_adapter_source = cljonic::core::hash_map(1, 10, 2, 20);
    constexpr auto map_adapter_prefix =
        cljonic::core::take_while([](const auto&) noexcept { return true; }, map_adapter_source);
    constexpr auto map_adapter_suffix =
        cljonic::core::drop_while([](const auto&) noexcept { return false; }, map_adapter_source);
    STATIC_REQUIRE((cljonic::core::equal(cljonic::core::take(2, map_adapter_prefix),
                                         cljonic::core::vector(cljonic::core::map_entry(1, 10),
                                                               cljonic::core::map_entry(2, 20))) ||
                    cljonic::core::equal(cljonic::core::take(2, map_adapter_prefix),
                                         cljonic::core::vector(cljonic::core::map_entry(2, 20),
                                                               cljonic::core::map_entry(1, 10)))));
    STATIC_REQUIRE((cljonic::core::equal(cljonic::core::take(2, map_adapter_suffix),
                                         cljonic::core::vector(cljonic::core::map_entry(1, 10),
                                                               cljonic::core::map_entry(2, 20))) ||
                    cljonic::core::equal(cljonic::core::take(2, map_adapter_suffix),
                                         cljonic::core::vector(cljonic::core::map_entry(2, 20),
                                                               cljonic::core::map_entry(1, 10)))));
    constexpr auto set_adapter_source = cljonic::core::hash_set(1, 2);
    constexpr auto set_adapter_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, set_adapter_source);
    constexpr auto set_adapter_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, set_adapter_source);
    STATIC_REQUIRE((cljonic::core::equal(cljonic::core::take(2, set_adapter_prefix),
                                         cljonic::core::vector(1, 2)) ||
                    cljonic::core::equal(cljonic::core::take(2, set_adapter_prefix),
                                         cljonic::core::vector(2, 1))));
    STATIC_REQUIRE((cljonic::core::equal(cljonic::core::take(2, set_adapter_suffix),
                                         cljonic::core::vector(1, 2)) ||
                    cljonic::core::equal(cljonic::core::take(2, set_adapter_suffix),
                                         cljonic::core::vector(2, 1))));
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::take(1, cljonic::core::hash_map(1, 2))),
                                cljonic::Vector<cljonic::MapEntry<int, int>, 1>>);
    STATIC_REQUIRE(cljonic::core::take(1, cljonic::core::vector(TakeElement{1}, TakeElement{2}))(0).value == 1);

    volatile int base_raw = 1;
    const auto base = base_raw;
    check_take_prefix(cljonic::core::vector(base, base + 1, base + 2), 2);
    check_take_prefix(cljonic::core::queue(base, base + 1, base + 2), 2);
    check_take_prefix(cljonic::core::hash_map(base, base + 10, base + 1, base + 11, base + 2, base + 12), 2);
    check_take_prefix(cljonic::core::hash_set(base, base + 1, base + 2), 2);
    check_take_prefix(cljonic::core::string<4>("abc"), 2);
    auto source = cljonic::core::vector(base, base + 1, base + 2);
    const auto result = cljonic::core::take(2, source);
    source = cljonic::core::vector(base + 10, base + 11, base + 12);
    REQUIRE(cljonic::core::equal(result, cljonic::core::vector(base, base + 1)));
}

TEST_CASE("Take normalizes counts and bounds requests by observable capacity", "[take]") {
    TRACE_ID("invariant.Take.CountUsesRepeatIntegerAdmission");
    TRACE_ID("invariant.Take.NonpositiveCountNormalizesBeforeUnsignedConversion");
    TRACE_ID("invariant.Take.ProducerResultCapacityIsObservationCap");
    TRACE_ID("invariant.Take.CapacityIsSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.Take.ResultCountIsMinimumOfRequestTraversalLengthAndCapacity");
    TRACE_ID("invariant.Take.OversizedRequestReturnsObservableSourceWithoutFailure");
    TRACE_ID("invariant.Take.InvalidCountsSourcesAndAritiesHaveTargetedDiagnostics");

    constexpr auto values = cljonic::core::vector(1, 2, 3);
    STATIC_REQUIRE(cljonic::core::count(cljonic::core::take(short{2}, values)) == 2);
    STATIC_REQUIRE(cljonic::core::count(cljonic::core::take(std::size_t{2}, values)) == 2);
    STATIC_REQUIRE(cljonic::core::count(cljonic::core::take(std::numeric_limits<int>::min(), values)) == 0);
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take(std::numeric_limits<std::size_t>::max(), values), values));
    STATIC_REQUIRE(cljonic::core::count(cljonic::core::take(1, cljonic::Vector<int, 0>{})) == 0);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::take(2, cljonic::core::repeat(3, 7))),
                                cljonic::Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);

    volatile int count_raw = 2;
    const auto requested = count_raw;
    const auto source = cljonic::core::vector(requested, requested + 1, requested + 2);
    REQUIRE(
        cljonic::core::equal(cljonic::core::take(requested, source), cljonic::core::vector(requested, requested + 1)));
    REQUIRE(cljonic::core::take(0, source).count() == 0);
    REQUIRE(cljonic::core::take(-requested, source).count() == 0);
    REQUIRE(cljonic::core::take(requested, cljonic::Vector<int, 0>{}).count() == 0);
    REQUIRE(cljonic::core::take(requested, cljonic::Vector<int, 4>{}).count() == 0);
    REQUIRE(cljonic::core::equal(cljonic::core::take(requested + 10, source), source));
    constexpr auto cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    const auto bounded = cljonic::core::take(cap + 1, cljonic::core::repeat(cap + 10, requested));
    REQUIRE(bounded.capacity() == cap);
    REQUIRE(bounded.count() == cap);
    REQUIRE(bounded(0) == requested);
    REQUIRE(bounded(cap - 1) == requested);
    const auto unbounded =
        cljonic::core::take(std::numeric_limits<std::size_t>::max(), cljonic::core::repeat(requested));
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
    REQUIRE(cljonic::core::equal(cljonic::core::take(requested, cljonic::core::range(base, base + 5)),
                                 cljonic::core::vector(base, base + 1, base + 2)));
    REQUIRE(cljonic::core::equal(cljonic::core::take(requested, cljonic::core::repeat(2, base)),
                                 cljonic::core::vector(base, base)));
    REQUIRE(cljonic::core::equal(cljonic::core::take(requested, cljonic::core::repeat(base)),
                                 cljonic::core::vector(base, base, base)));
    REQUIRE(cljonic::core::equal(
        cljonic::core::take(requested, cljonic::core::take_while([](int value) noexcept { return value < 4; },
                                                                  cljonic::core::vector(base, base + 1, base + 2))),
        cljonic::core::vector(base, base + 1, base + 2)));
    REQUIRE(cljonic::core::equal(
        cljonic::core::take(requested, cljonic::core::drop_while([](int value) noexcept { return value < 2; },
                                                                  cljonic::core::vector(base, base + 1, base + 2))),
        cljonic::core::vector(base + 1, base + 2)));
    const auto runtime_map_adapter_source =
        cljonic::core::hash_map(base, base + 10, base + 1, base + 11);
    const auto runtime_map_adapter_prefix =
        cljonic::core::take_while([](const auto&) noexcept { return true; }, runtime_map_adapter_source);
    const auto runtime_map_adapter_suffix =
        cljonic::core::drop_while([](const auto&) noexcept { return false; }, runtime_map_adapter_source);
    REQUIRE((cljonic::core::equal(cljonic::core::take(2, runtime_map_adapter_prefix),
                                  cljonic::core::vector(cljonic::core::map_entry(base, base + 10),
                                                        cljonic::core::map_entry(base + 1, base + 11))) ||
             cljonic::core::equal(cljonic::core::take(2, runtime_map_adapter_prefix),
                                  cljonic::core::vector(cljonic::core::map_entry(base + 1, base + 11),
                                                        cljonic::core::map_entry(base, base + 10)))));
    REQUIRE((cljonic::core::equal(cljonic::core::take(2, runtime_map_adapter_suffix),
                                  cljonic::core::vector(cljonic::core::map_entry(base, base + 10),
                                                        cljonic::core::map_entry(base + 1, base + 11))) ||
             cljonic::core::equal(cljonic::core::take(2, runtime_map_adapter_suffix),
                                  cljonic::core::vector(cljonic::core::map_entry(base + 1, base + 11),
                                                        cljonic::core::map_entry(base, base + 10)))));
    const auto runtime_set_adapter_source = cljonic::core::hash_set(base, base + 1);
    const auto runtime_set_adapter_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, runtime_set_adapter_source);
    const auto runtime_set_adapter_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, runtime_set_adapter_source);
    REQUIRE((cljonic::core::equal(cljonic::core::take(2, runtime_set_adapter_prefix),
                                  cljonic::core::vector(base, base + 1)) ||
             cljonic::core::equal(cljonic::core::take(2, runtime_set_adapter_prefix),
                                  cljonic::core::vector(base + 1, base))));
    REQUIRE((cljonic::core::equal(cljonic::core::take(2, runtime_set_adapter_suffix),
                                  cljonic::core::vector(base, base + 1)) ||
             cljonic::core::equal(cljonic::core::take(2, runtime_set_adapter_suffix),
                                  cljonic::core::vector(base + 1, base))));
    REQUIRE(cljonic::core::equal(
        cljonic::core::take(requested, cljonic::core::cycle(cljonic::core::vector(base, base + 1))),
        cljonic::core::vector(base, base + 1, base)));
    REQUIRE(cljonic::core::take(requested, cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0);

    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        base);
    REQUIRE(cljonic::core::take(0, iterated).count() == 0);
    REQUIRE(cljonic::core::take(-requested, iterated).count() == 0);
    REQUIRE(steps == 0);
    REQUIRE(cljonic::core::take(1, iterated)(0) == base);
    REQUIRE(steps == 0);
    REQUIRE(cljonic::core::equal(cljonic::core::take(requested, iterated),
                                 cljonic::core::vector(base, base + 1, base + 2)));
    REQUIRE(steps == requested - 1);

    int calls = 0;
    const auto produced = cljonic::core::repeatedly([&calls, base]() noexcept {
        ++calls;
        return base + calls;
    });
    REQUIRE(cljonic::core::take(0, produced).count() == 0);
    REQUIRE(cljonic::core::take(-requested, produced).count() == 0);
    REQUIRE(cljonic::core::take(requested, cljonic::core::repeatedly(0,
                                                                     [&calls]() noexcept {
                                                                         ++calls;
                                                                         return 0;
                                                                     }))
                .count() == 0);
    REQUIRE(calls == 0);
    REQUIRE(cljonic::core::equal(cljonic::core::take(requested, produced),
                                 cljonic::core::vector(base + 1, base + 2, base + 3)));
    REQUIRE(calls == requested);
    REQUIRE(cljonic::core::take(1, produced)(0) == base + requested + 1);
    REQUIRE(calls == requested + 1);
    const auto stateful = cljonic::core::repeatedly(5, [value = base]() mutable noexcept { return value++; });
    const auto prefix = cljonic::core::take(requested, stateful);
    REQUIRE(cljonic::core::equal(prefix, cljonic::core::vector(base, base + 1, base + 2)));
    REQUIRE(cljonic::core::equal(prefix, cljonic::core::take(requested, stateful)));
    REQUIRE(cljonic::core::count(stateful) == 5);
    REQUIRE(cljonic::core::equal(cljonic::core::take(20, stateful),
                                 cljonic::core::vector(base, base + 1, base + 2, base + 3, base + 4)));
}
