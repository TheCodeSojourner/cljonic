#include <catch2/catch_test_macros.hpp>

#include <concepts>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename T>
concept HasAdmittedLast =
    requires(const T& value) { requires !std::same_as<decltype(cljonic::core::last(value)), void>; };
} // namespace

TEST_CASE("Last free function operations", "[last]") {
    using cljonic::Map;
    using cljonic::MapEntry;
    using cljonic::Queue;
    using cljonic::Set;
    using cljonic::String;
    using cljonic::Vector;
    using cljonic::core::assoc;
    using cljonic::core::conj;
    using cljonic::core::count;
    using cljonic::core::first;
    using cljonic::core::last;

    TRACE_ID("entity-fields.Last");
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Vector<int, 4>>);

    TRACE_ID("invariant.Last.MinimumArityIsOne");
    STATIC_REQUIRE(last(Vector<int, 4>{10}) == 10);

    TRACE_ID("invariant.Last.MaximumArityIsOne");
    STATIC_REQUIRE(last(Vector<int, 4>{10, 20}) == 20);

    TRACE_ID("invariant.Last.SourceIsTheOnlyArgument");
    STATIC_REQUIRE(last(Vector<int, 4>{10, 20, 30}) == 30);

    TRACE_ID("invariant.Last.AllCljonicCollectionsProducersAndMapEntryAdmitted");
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Vector<int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Map<int, int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Set<int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Queue<int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<String<4>>);
    STATIC_REQUIRE(HasAdmittedLast<MapEntry<int, int>>);
    STATIC_REQUIRE(HasAdmittedLast<decltype(cljonic::core::repeat(9))>);

    TRACE_ID("invariant.Last.ResultIsIndependentlyOwningSingleValueOfSourceElementType");
    STATIC_REQUIRE(std::same_as<decltype(last(Vector<int, 4>{10, 20})), int>);
    STATIC_REQUIRE(std::same_as<decltype(last(Map<int, int, 4>{})), MapEntry<int, int>>);
    STATIC_REQUIRE(std::same_as<decltype(last(Set<int, 4>{})), int>);
    STATIC_REQUIRE(std::same_as<decltype(last(Queue<int, 4>{})), int>);
    STATIC_REQUIRE(std::same_as<decltype(last(String<4>{})), char>);
    STATIC_REQUIRE(std::same_as<decltype(last(MapEntry<int, int>{3, 300})), int>);

    TRACE_ID("invariant.Last.VectorAndStringReturnLogicalElementAtFinalIndex");
    STATIC_REQUIRE(last(Vector<int, 4>{10, 20, 30, 40}) == 40);
    STATIC_REQUIRE(last(String<5>{"abcd"}) == 'd');

    TRACE_ID("invariant.Last.QueueReturnsFinalFifoElementWithoutRemoval");
    constexpr auto queue_value = Queue<int, 4>{10, 20, 30, 40};
    STATIC_REQUIRE(last(queue_value) == 40);
    STATIC_REQUIRE(count(queue_value) == 4U);
    STATIC_REQUIRE(last(queue_value) == 40);
    STATIC_REQUIRE(last(Queue<int, 4>{10, 20}) == 20);

    TRACE_ID("invariant.Last.MapReturnsOneValueSemanticMapEntry");
    constexpr auto map_value = assoc(assoc(Map<int, int, 4>{}, 1, 100), 2, 200);
    STATIC_REQUIRE((last(map_value) == MapEntry<int, int>{1, 100} || last(map_value) == MapEntry<int, int>{2, 200}));

    TRACE_ID("invariant.Last.SetReturnsOneStoredElementPerTraversalOrder");
    constexpr auto set_value = conj(conj(Set<int, 4>{}, 7), 8);
    STATIC_REQUIRE((last(set_value) == 7 || last(set_value) == 8));

    TRACE_ID("invariant.Last.MapEntryArgumentReturnsItsValue");
    STATIC_REQUIRE(last(MapEntry<int, int>{3, 300}) == 300);

    TRACE_ID("invariant.Last.LastOfFirstMapReturnsEntryValue");
    STATIC_REQUIRE((last(first(map_value)) == 100 || last(first(map_value)) == 200));
    STATIC_REQUIRE(last(first(Map<int, int, 4>{MapEntry<int, int>{3, 300}})) == 300);

    TRACE_ID("invariant.Last.EmptySequenceableCollectionReturnsSourceDefaultElement");
    STATIC_REQUIRE(last(Vector<int, 4>{}) == 0);
    STATIC_REQUIRE(last(Map<int, int, 4>{}) == MapEntry<int, int>{});
    STATIC_REQUIRE(last(Set<int, 4>{}) == 0);
    STATIC_REQUIRE(last(Queue<int, 4>{}) == 0);
    STATIC_REQUIRE(last(String<4>{}) == '\0');

    TRACE_ID("invariant.Last.MapSetOrderSemanticallyUnordered");
    STATIC_REQUIRE((last(map_value) == MapEntry<int, int>{1, 100} || last(map_value) == MapEntry<int, int>{2, 200}));
    STATIC_REQUIRE((last(set_value) == 7 || last(set_value) == 8));

    TRACE_ID("invariant.Last.NoNestedResultAbsenceSentinelStatusOrPreflight");
    STATIC_REQUIRE(last(Vector<int, 0>{}) == 0);
    STATIC_REQUIRE(std::same_as<decltype(last(Vector<int, 0>{})), int>);

    TRACE_ID("invariant.Last.InvalidSourcesAndAritiesHaveTargetedDiagnosticsNamingLast");
    STATIC_REQUIRE_FALSE(HasAdmittedLast<int>);
    STATIC_REQUIRE_FALSE(HasAdmittedLast<char>);

    TRACE_ID("invariant.Last.ConstexprNoexceptNoHeapNoRttiNonmutating");
    constexpr Vector<int, 4> source{10, 20};
    STATIC_REQUIRE(last(source) == 20);
    STATIC_REQUIRE(count(source) == 2U);
    STATIC_REQUIRE(source(1) == 20);
    STATIC_REQUIRE(noexcept(last(source)));
    STATIC_REQUIRE(noexcept(last(map_value)));
    STATIC_REQUIRE(noexcept(last(set_value)));
    STATIC_REQUIRE(noexcept(last(MapEntry<int, int>{3, 300})));
    STATIC_REQUIRE(count(map_value) == 2U);
    STATIC_REQUIRE(count(set_value) == 2U);

    volatile int vector_value_raw = 42;
    int vector_value = vector_value_raw;
    auto rv = Vector<int, 4>{vector_value, 20};
    REQUIRE(last(rv) == 20);
    REQUIRE(count(rv) == 2U);

    auto rm = assoc(assoc(Map<int, int, 4>{}, 5, 500), 6, 600);
    REQUIRE((last(rm) == MapEntry<int, int>{5, 500} || last(rm) == MapEntry<int, int>{6, 600}));
    REQUIRE(count(rm) == 2U);

    auto rs = conj(conj(Set<int, 4>{}, 7), 8);
    REQUIRE((last(rs) == 7 || last(rs) == 8));
    REQUIRE(count(rs) == 2U);

    volatile int rq_value_raw = 11;
    int rq_value = rq_value_raw;
    auto rq = Queue<int, 4>{rq_value, 12};
    REQUIRE(last(rq) == 12);

    auto rtext = String<4>{"AB"};
    REQUIRE(last(rtext) == 'B');

    auto re = MapEntry<int, int>{6, 600};
    REQUIRE(last(re) == 600);

    auto ev = Vector<int, 4>{};
    REQUIRE(last(ev) == 0);
    auto em = Map<int, int, 4>{};
    REQUIRE(last(em) == MapEntry<int, int>{});
    auto es = Set<int, 4>{};
    REQUIRE(last(es) == 0);
    auto eq = Queue<int, 4>{};
    REQUIRE(last(eq) == 0);
    auto etext = String<4>{};
    REQUIRE(last(etext) == '\0');
}

TEST_CASE("Last observes every producer family by owning element value", "[last]") {
    using namespace cljonic;
    using namespace cljonic::core;

    TRACE_ID("invariant.Last.ProducerReturnsOwningCopyOfFinalObservableElement");
    STATIC_REQUIRE(last(range(7, 10)) == 9);
    STATIC_REQUIRE(last(repeat(9)) == 9);
    STATIC_REQUIRE(last(repeat(3, 7)) == 7);
    STATIC_REQUIRE(last(cycle(vector(4, 5))) ==
                   ((CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - 1U) % 2U == 0U ? 4 : 5));
    STATIC_REQUIRE(last(iterate([](int n) noexcept { return n + 1; }, 8)) ==
                   8 + static_cast<int>(CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE) - 1);
    STATIC_REQUIRE(last(repeatedly([]() noexcept { return 6; })) == 6);

    TRACE_ID("invariant.Last.YieldedMapEntryReturnedWholeNotValueProjection");
    STATIC_REQUIRE(std::same_as<decltype(last(repeat(map_entry(2, 20)))), MapEntry<int, int>>);
    STATIC_REQUIRE(last(repeat(map_entry(2, 20))) == map_entry(2, 20));
    STATIC_REQUIRE(last(take_while([](auto) noexcept { return true; }, hash_map(2, 20))) == map_entry(2, 20));
    STATIC_REQUIRE(last(take_while([](char) noexcept { return true; }, string("AB"))) == 'B');

    TRACE_ID("invariant.Last.NoAdditionalElementEqualityOrderingOrAssignmentRequired");
    struct Element {
        int value = 42;
    };
    STATIC_REQUIRE(last(repeat(Element{7})).value == 7);
    STATIC_REQUIRE(last(repeat(0, Element{7})).value == 42);
    STATIC_REQUIRE(std::same_as<decltype(last(repeat(7L))), long>);

    TRACE_ID("invariant.Last.EmptyProducerObservationReturnsDefaultElement");
    constexpr auto below_three = [](int n) noexcept { return n < 3; };
    STATIC_REQUIRE(last(range(2, 2)) == 0);
    STATIC_REQUIRE(last(cycle(vector<int>())) == 0);
    STATIC_REQUIRE(last(take_while(below_three, vector(4))) == 0);
    STATIC_REQUIRE(last(drop_while(below_three, vector(1, 2))) == 0);

    TRACE_ID("invariant.Last.ZeroObservationCapReturnsDefaultElementWithoutTraversal");
    STATIC_REQUIRE(last(repeat(0, 7)) == 0);
    STATIC_REQUIRE(last(repeatedly(0, []() noexcept { return 7; })) == 0);
    int zero_calls = 0;
    REQUIRE(last(repeatedly(0, [&zero_calls]() noexcept {
                ++zero_calls;
                return 7;
            })) == 0);
    REQUIRE(zero_calls == 0);

    TRACE_ID("invariant.Last.OriginalWindowIsBoundedBySourceCountCapacityAndEndpoint");
    STATIC_REQUIRE(last(range(7, 10)) == 9);
    STATIC_REQUIRE(last(repeat(3, 7)) == 7);
    STATIC_REQUIRE(last(cycle(vector(4, 5))) ==
                   ((CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - 1U) % 2U == 0U ? 4 : 5));

    TRACE_ID("invariant.Last.SkippingNeverReplenishesWindowBudget");
    STATIC_REQUIRE(last(drop_while(below_three, vector(1, 2, 4))) == 4);
    STATIC_REQUIRE(last(drop_while([](int) noexcept { return true; }, vector(1, 2, 3))) == 0);
    STATIC_REQUIRE(last(drop_while([](int) noexcept { return true; }, range(1, 4))) == 0);

    TRACE_ID("invariant.Last.TakeWhileStopsAtFirstFalseCandidateOrExhaustion");
    STATIC_REQUIRE(last(take_while(below_three, vector(1, 2, 4))) == 2);
    STATIC_REQUIRE(last(take_while(below_three, vector(0, 1, 2))) == 2);

    TRACE_ID("invariant.Last.DropWhileReturnsFinalObservedElementOfRemainingWindow");
    STATIC_REQUIRE(last(drop_while(below_three, vector(1, 2, 4))) == 4);
    STATIC_REQUIRE(last(drop_while([](int n) noexcept { return n < 5; }, queue(3, 5, 6))) == 6);

    volatile int raw = 7;
    const int value = raw;
    const auto source = repeat(vector(value, value + 1));
    auto result = last(source);
    result = assoc(result, 0, 99);
    REQUIRE(last(source)(0) == value);
    REQUIRE(result(0) == 99);

    const auto reserved = repeat(vector<int, 2>(value));
    const auto appended = assoc(last(reserved), 1, value + 1);
    REQUIRE(count(appended) == 2U);
    REQUIRE(appended(1) == value + 1);
    REQUIRE(count(last(reserved)) == 1U);

    REQUIRE(last(repeat(value)) == value);
    REQUIRE(last(range(value, value + 2)) == value + 1);

    volatile int raw_limit = 7;
    const int limit_value = raw_limit;
    const auto captured = [limit_value](int n) noexcept { return n < limit_value; };
    const auto adapter = take_while(captured, vector(limit_value - 1, limit_value));
    REQUIRE(last(adapter) == limit_value - 1);
}

TEST_CASE("Last performs only needed producer evaluation with fresh traversal", "[last]") {
    using namespace cljonic;
    using namespace cljonic::core;

    TRACE_ID("invariant.Last.OriginalWindowIsTraversedOnce");
    TRACE_ID("invariant.Last.RepeatedlyCallbackOncePerVisitedValue");
    int produced = 0;
    const auto generator = repeatedly(4, [&produced]() noexcept { return ++produced; });
    REQUIRE(last(generator) == 4);
    REQUIRE(produced == 4);

    TRACE_ID("invariant.Last.NoPrepassMaterializationCacheOrSecondObservation");
    REQUIRE(produced == 4);

    TRACE_ID("invariant.Last.NoAdvancePastWindowBoundOrActualEndpoint");
    REQUIRE(last(generator) == 8);
    REQUIRE(produced == 8);

    TRACE_ID("invariant.Last.IterateStepsOnlyBetweenVisitedValues");
    int steps = 0;
    const auto stepped = iterate(
        [&steps](int n) noexcept {
            ++steps;
            return n + 1;
        },
        1);
    REQUIRE(last(stepped) == static_cast<int>(CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE));
    REQUIRE(steps == static_cast<int>(CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE) - 1);

    TRACE_ID("invariant.Last.CyclePreservesCurrentLogicalPositionAndExistingRestartPolicy");
    constexpr auto cycled_values = vector(4, 5, 6, 7);
    constexpr auto cycled = cycle(cycled_values);
    constexpr auto cycled_expected =
        cycled_values(static_cast<std::size_t>((CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE - 1U) % 4U));
    STATIC_REQUIRE(last(cycled) == cycled_expected);
    STATIC_REQUIRE(last(cycled) == cycled_expected);
    STATIC_REQUIRE(count(cycled_values) == 4U);

    TRACE_ID("invariant.Last.SourceAndCallbackStatePreserved");
    const auto stateful = repeatedly(4, [value = 1] mutable noexcept { return value++; });
    REQUIRE(last(stateful) == 4);
    REQUIRE(last(stateful) == 4);
    REQUIRE(noexcept(last(stateful)));

    TRACE_ID("invariant.Last.ReferencedOrStaticCallbackStateRemainsShared");
    int shared = 0;
    const auto shared_state = repeatedly(4, [&shared]() noexcept { return ++shared; });
    REQUIRE(last(shared_state) == 4);
    REQUIRE(last(shared_state) == 8);
    REQUIRE(shared == 8);

    TRACE_ID("invariant.Last.RepeatedCallsFreshTraversalNoCrossCallCache");
    REQUIRE(last(shared_state) == 12);
    REQUIRE(shared == 12);
}
