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
    const auto before = cljonic::core::seq(source);
    const auto result = cljonic::core::drop(requested, source);
    REQUIRE(result.capacity() == before.capacity());
    REQUIRE(result.count() == before.count() - static_cast<std::size_t>(requested));
    for (std::size_t index = 0; index < result.count(); ++index) {
        REQUIRE(result(index) == before(index + static_cast<std::size_t>(requested)));
    }
    REQUIRE(cljonic::core::equal(cljonic::core::seq(source), before));
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

    constexpr auto values = cljonic::core::vector<int, 5>(1, 2, 3);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::drop(2, values)), cljonic::Vector<int, 5>>);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::drop(0, values)), decltype(cljonic::core::drop(100, values))>);
    STATIC_REQUIRE(noexcept(cljonic::core::drop(2, values)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::drop(2, values), cljonic::core::vector(3)));
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::drop(1, cljonic::core::queue(1, 2, 3)), cljonic::core::vector(2, 3)));
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::drop(1, cljonic::core::string<4>("abc")), cljonic::core::vector('b', 'c')));
    constexpr auto take_while_source =
        cljonic::core::take_while([](int value) noexcept { return value < 4; }, cljonic::core::vector(1, 2, 3, 4));
    constexpr auto drop_while_source =
        cljonic::core::drop_while([](int value) noexcept { return value < 3; }, cljonic::core::vector(1, 2, 3, 4));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::drop(1, take_while_source), cljonic::core::vector(2, 3)));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::drop(1, drop_while_source), cljonic::core::vector(4)));
    constexpr auto map_adapter_source = cljonic::core::hash_map(1, 10, 2, 20);
    constexpr auto map_adapter_prefix =
        cljonic::core::take_while([](const auto&) noexcept { return true; }, map_adapter_source);
    constexpr auto map_adapter_suffix =
        cljonic::core::drop_while([](const auto&) noexcept { return false; }, map_adapter_source);
    constexpr auto dropped_map_prefix = cljonic::core::drop(1, map_adapter_prefix);
    constexpr auto dropped_map_suffix = cljonic::core::drop(1, map_adapter_suffix);
    STATIC_REQUIRE(dropped_map_prefix.count() == 1U);
    STATIC_REQUIRE(dropped_map_suffix.count() == 1U);
    STATIC_REQUIRE((dropped_map_prefix(0U) == cljonic::core::map_entry(1, 10) ||
                    dropped_map_prefix(0U) == cljonic::core::map_entry(2, 20)));
    STATIC_REQUIRE((dropped_map_suffix(0U) == cljonic::core::map_entry(1, 10) ||
                    dropped_map_suffix(0U) == cljonic::core::map_entry(2, 20)));
    constexpr auto set_adapter_source = cljonic::core::hash_set(1, 2);
    constexpr auto set_adapter_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, set_adapter_source);
    constexpr auto set_adapter_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, set_adapter_source);
    constexpr auto dropped_set_prefix = cljonic::core::drop(1, set_adapter_prefix);
    constexpr auto dropped_set_suffix = cljonic::core::drop(1, set_adapter_suffix);
    STATIC_REQUIRE(dropped_set_prefix.count() == 1U);
    STATIC_REQUIRE(dropped_set_suffix.count() == 1U);
    STATIC_REQUIRE((dropped_set_prefix(0U) == 1 || dropped_set_prefix(0U) == 2));
    STATIC_REQUIRE((dropped_set_suffix(0U) == 1 || dropped_set_suffix(0U) == 2));
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::drop(1, cljonic::core::hash_map(1, 2))),
                                cljonic::Vector<cljonic::MapEntry<int, int>, 1>>);
    STATIC_REQUIRE(cljonic::core::drop(1, cljonic::core::vector(DropElement{1}, DropElement{2}))(0).value == 2);

    volatile int base_raw = 1;
    const auto base = base_raw;
    check_drop_suffix(cljonic::core::vector<int, 5>(base, base + 1, base + 2), 1);
    check_drop_suffix(cljonic::core::queue(base, base + 1, base + 2), 1);
    check_drop_suffix(cljonic::core::hash_map(base, base + 10, base + 1, base + 11, base + 2, base + 12), 1);
    check_drop_suffix(cljonic::core::hash_set(base, base + 1, base + 2), 1);
    check_drop_suffix(cljonic::core::string<4>("abc"), 1);
    REQUIRE(cljonic::core::equal(
        cljonic::core::drop(1, cljonic::core::take_while([base](int value) noexcept { return value < base + 3; },
                                                          cljonic::core::vector(base, base + 1, base + 2))),
        cljonic::core::vector(base + 1, base + 2)));
    REQUIRE(cljonic::core::equal(
        cljonic::core::drop(1, cljonic::core::drop_while([base](int value) noexcept { return value < base + 1; },
                                                          cljonic::core::vector(base, base + 1, base + 2))),
        cljonic::core::vector(base + 2)));
    const auto runtime_map_adapter_source =
        cljonic::core::hash_map(base, base + 10, base + 1, base + 11);
    const auto runtime_map_adapter_prefix =
        cljonic::core::take_while([](const auto&) noexcept { return true; }, runtime_map_adapter_source);
    const auto runtime_map_adapter_suffix =
        cljonic::core::drop_while([](const auto&) noexcept { return false; }, runtime_map_adapter_source);
    const auto runtime_dropped_map_prefix = cljonic::core::drop(1, runtime_map_adapter_prefix);
    const auto runtime_dropped_map_suffix = cljonic::core::drop(1, runtime_map_adapter_suffix);
    REQUIRE(runtime_dropped_map_prefix.count() == 1U);
    REQUIRE(runtime_dropped_map_suffix.count() == 1U);
    REQUIRE((runtime_dropped_map_prefix(0U) == cljonic::core::map_entry(base, base + 10) ||
             runtime_dropped_map_prefix(0U) == cljonic::core::map_entry(base + 1, base + 11)));
    REQUIRE((runtime_dropped_map_suffix(0U) == cljonic::core::map_entry(base, base + 10) ||
             runtime_dropped_map_suffix(0U) == cljonic::core::map_entry(base + 1, base + 11)));
    const auto runtime_set_adapter_source = cljonic::core::hash_set(base, base + 1);
    const auto runtime_set_adapter_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, runtime_set_adapter_source);
    const auto runtime_set_adapter_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, runtime_set_adapter_source);
    const auto runtime_dropped_set_prefix = cljonic::core::drop(1, runtime_set_adapter_prefix);
    const auto runtime_dropped_set_suffix = cljonic::core::drop(1, runtime_set_adapter_suffix);
    REQUIRE(runtime_dropped_set_prefix.count() == 1U);
    REQUIRE(runtime_dropped_set_suffix.count() == 1U);
    REQUIRE((runtime_dropped_set_prefix(0U) == base || runtime_dropped_set_prefix(0U) == base + 1));
    REQUIRE((runtime_dropped_set_suffix(0U) == base || runtime_dropped_set_suffix(0U) == base + 1));
    auto source = cljonic::core::vector(base, base + 1, base + 2);
    const auto result = cljonic::core::drop(1, source);
    source = cljonic::core::vector(base + 10, base + 11, base + 12);
    REQUIRE(cljonic::core::equal(result, cljonic::core::vector(base + 1, base + 2)));
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

    constexpr auto values = cljonic::core::vector(1, 2, 3);
    STATIC_REQUIRE(cljonic::core::drop(short{2}, values).count() == 1);
    STATIC_REQUIRE(cljonic::core::drop(std::size_t{2}, values).count() == 1);
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::drop(std::numeric_limits<int>::min(), values), values));
    STATIC_REQUIRE(cljonic::core::drop(std::numeric_limits<std::size_t>::max(), values).count() == 0);
    STATIC_REQUIRE(cljonic::core::drop(0, cljonic::Vector<int, 0>{}).count() == 0);
    STATIC_REQUIRE(std::same_as<decltype(cljonic::core::drop(1, cljonic::core::repeat(3, 7))),
                                cljonic::Vector<int, cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE>>);

    volatile int count_raw = 2;
    const auto requested = count_raw;
    const auto source = cljonic::core::vector(requested, requested + 1, requested + 2);
    REQUIRE(cljonic::core::equal(cljonic::core::drop(requested, source), cljonic::core::vector(requested + 2)));
    REQUIRE(cljonic::core::equal(cljonic::core::drop(0, source), source));
    REQUIRE(cljonic::core::equal(cljonic::core::drop(-requested, source), source));
    REQUIRE(cljonic::core::drop(3, source).count() == 0);
    REQUIRE(cljonic::core::drop(std::numeric_limits<std::size_t>::max(), source).count() == 0);
    REQUIRE(cljonic::core::drop(requested, cljonic::Vector<int, 0>{}).count() == 0);
    REQUIRE(cljonic::core::drop(requested, cljonic::Vector<int, 4>{}).count() == 0);
    constexpr auto cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    const auto finite = cljonic::core::drop(requested, cljonic::core::repeat(cap + 10, requested));
    const auto unbounded = cljonic::core::drop(requested, cljonic::core::repeat(requested));
    REQUIRE(finite.capacity() == cap);
    REQUIRE(finite.count() == cap - requested);
    REQUIRE(finite(0) == requested);
    REQUIRE(unbounded.count() == cap - requested);
    REQUIRE(unbounded(unbounded.count() - 1) == requested);
    REQUIRE(cljonic::core::drop(cap, cljonic::core::repeat(requested)).count() == 0);
    REQUIRE(cljonic::core::drop(cap + 1, cljonic::core::repeat(cap + 10, requested)).count() == 0);
    REQUIRE(cljonic::core::equal(cljonic::core::drop(1, cljonic::core::range(requested, requested + 4)),
                                 cljonic::core::vector(requested + 1, requested + 2, requested + 3)));
    const auto cycled = cljonic::core::drop(1, cljonic::core::cycle(cljonic::core::vector(requested, requested + 1)));
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
    REQUIRE(cljonic::core::drop(requested, cljonic::core::cycle(cljonic::Vector<int, 0>{})).count() == 0);
    REQUIRE(cljonic::core::drop(0, cljonic::core::cycle(cljonic::Vector<int, 2>{})).count() == 0);
    int calls = 0;
    const auto produced = cljonic::core::repeatedly(4, [&calls] noexcept { return ++calls; });
    REQUIRE(cljonic::core::drop(4, produced).count() == 0);
    REQUIRE(cljonic::core::drop(100, produced).count() == 0);
    REQUIRE(cljonic::core::drop(1, cljonic::core::repeatedly(0, [&calls] noexcept { return ++calls; })).count() == 0);
    const auto empty_cycle = cljonic::core::cycle(cljonic::core::repeatedly(0, [&calls] noexcept { return ++calls; }));
    REQUIRE(cljonic::core::drop(requested, empty_cycle).count() == 0);
    REQUIRE(calls == 0);
    REQUIRE(cljonic::core::equal(cljonic::core::drop(requested, produced), cljonic::core::vector(2, 3, 4)));
    REQUIRE(calls == 4);
    REQUIRE(cljonic::core::equal(cljonic::core::drop(-requested, produced), cljonic::core::vector(5, 6, 7, 8)));
    REQUIRE(calls == 8);
    const auto stateful = cljonic::core::repeatedly(4, [value = 1] mutable noexcept { return value++; });
    REQUIRE(cljonic::core::equal(cljonic::core::drop(requested, stateful), cljonic::core::vector(2, 3, 4)));
    REQUIRE(cljonic::core::equal(cljonic::core::drop(requested, stateful), cljonic::core::vector(2, 3, 4)));
    REQUIRE(stateful.count() == 4);

    int steps = 0;
    const auto iterated = cljonic::core::iterate(
        [&steps](int value) noexcept {
            ++steps;
            return value + 1;
        },
        1);
    REQUIRE(cljonic::core::drop(cap, iterated).count() == 0);
    REQUIRE(steps == 0);
    const auto suffix = cljonic::core::drop(requested, iterated);
    REQUIRE(suffix.count() == cap - requested);
    REQUIRE(suffix(0) == 2);
    REQUIRE(suffix(suffix.count() - 1) == static_cast<int>(cap));
    REQUIRE(std::cmp_equal(steps, cap - 1));
    calls = 0;
    const auto unbounded = cljonic::core::repeatedly([&calls] noexcept { return ++calls; });
    REQUIRE(cljonic::core::drop(cap, unbounded).count() == 0);
    REQUIRE(calls == 0);
    const auto generated = cljonic::core::drop(requested, unbounded);
    REQUIRE(generated.count() == cap - requested);
    REQUIRE(generated(0) == 2);
    REQUIRE(generated(generated.count() - 1) == static_cast<int>(cap));
    REQUIRE(std::cmp_equal(calls, cap));
    calls = 0;
    const auto longer = cljonic::core::repeatedly(cap + 3, [&calls] noexcept { return ++calls; });
    const auto bounded = cljonic::core::drop(requested, longer);
    REQUIRE(bounded.count() == cap - requested);
    REQUIRE(bounded(0) == 2);
    REQUIRE(bounded(bounded.count() - 1) == static_cast<int>(cap));
    REQUIRE(std::cmp_equal(calls, cap));
    calls = 0;
    REQUIRE(cljonic::core::drop(cap, longer).count() == 0);
    REQUIRE(calls == 0);
    REQUIRE(cljonic::core::equal(
        cljonic::core::drop(0, cljonic::core::repeatedly(1, [&calls] noexcept { return ++calls; })),
        cljonic::core::vector(1)));
    REQUIRE(calls == 1);
}
