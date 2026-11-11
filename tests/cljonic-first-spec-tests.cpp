#include <catch2/catch_test_macros.hpp>

#include <concepts>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename T>
concept HasAdmittedFirst =
    requires(const T& value) { requires !std::same_as<decltype(cljonic::core::first(value)), void>; };
} // namespace

TEST_CASE("First free function operations", "[first]") {
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

    TRACE_ID("entity-fields.First");
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Vector<int, 4>>);

    TRACE_ID("invariant.First.FirstFreeFunctionSupported");
    STATIC_REQUIRE(std::same_as<decltype(first(Vector<int, 4>{10})), int>);

    TRACE_ID("invariant.First.ReturnsFirstLogicalElement");
    STATIC_REQUIRE(first(Vector<int, 4>{10, 20}) == 10);

    TRACE_ID("invariant.First.SupportsSequenceableCollectionDomain");
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Vector<int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Map<int, int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Set<int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Queue<int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<String<4>>);

    TRACE_ID("invariant.First.SupportsMapEntryDomain");
    STATIC_REQUIRE(first(MapEntry<int, int>{1, 100}) == 1);

    TRACE_ID("invariant.First.VectorAndStringUseIndexZero");
    STATIC_REQUIRE(first(Vector<int, 4>{10, 20, 30}) == 10);
    STATIC_REQUIRE(first(String<4>{"AB"}) == 'A');

    TRACE_ID("invariant.First.QueueUsesFrontElement");
    STATIC_REQUIRE(first(Queue<int, 4>{5, 6}) == 5);

    TRACE_ID("invariant.First.MapReturnsValueSemanticEntry");
    constexpr auto map_value = assoc(assoc(Map<int, int, 4>{}, 1, 100), 2, 200);
    STATIC_REQUIRE((first(map_value) == MapEntry<int, int>{1, 100} || first(map_value) == MapEntry<int, int>{2, 200}));

    TRACE_ID("invariant.First.SetReturnsStoredElement");
    constexpr auto set_value = conj(conj(Set<int, 4>{}, 7), 8);
    STATIC_REQUIRE((first(set_value) == 7 || first(set_value) == 8));

    TRACE_ID("invariant.First.MapEntryReturnsKey");
    STATIC_REQUIRE(first(MapEntry<int, int>{3, 300}) == 3);
    STATIC_REQUIRE(first(first(Map<int, int, 4>{MapEntry<int, int>{3, 300}})) == 3);

    TRACE_ID("invariant.First.EmptyYieldsDefaultElement");
    STATIC_REQUIRE(first(Vector<int, 4>{}) == 0);
    STATIC_REQUIRE(first(Map<int, int, 4>{}) == MapEntry<int, int>{});
    STATIC_REQUIRE(first(Set<int, 4>{}) == 0);
    STATIC_REQUIRE(first(Queue<int, 4>{}) == 0);
    STATIC_REQUIRE(first(String<4>{}) == '\0');

    TRACE_ID("invariant.First.MapAndSetTraversalOrderIsNotSemanticallySignificant");
    STATIC_REQUIRE((first(map_value) == MapEntry<int, int>{1, 100} || first(map_value) == MapEntry<int, int>{2, 200}));
    STATIC_REQUIRE((first(set_value) == 7 || first(set_value) == 8));

    TRACE_ID("invariant.First.LeavesInputUnchanged");
    constexpr Vector<int, 4> source{10, 20};
    STATIC_REQUIRE(first(source) == 10);
    STATIC_REQUIRE(count(source) == 2U);
    STATIC_REQUIRE(source(0) == 10);

    TRACE_ID("invariant.First.NoHeapAllocation");
    STATIC_REQUIRE(noexcept(first(source)));

    TRACE_ID("invariant.First.NoRtti");
    STATIC_REQUIRE(noexcept(first(map_value)));

    TRACE_ID("invariant.First.NoExceptions");
    STATIC_REQUIRE(noexcept(first(set_value)));

    TRACE_ID("invariant.First.SingleThreadedExecutionModel");
    STATIC_REQUIRE(first(MapEntry<int, int>{4, 400}) == 4);

    TRACE_ID("invariant.First.ReferentialTransparencyByDefaultUnlessSourceAuthorizesEffects");
    STATIC_REQUIRE(first(Vector<int, 4>{10, 20}) == first(Vector<int, 4>{10, 20}));

    TRACE_ID("invariant.First.RejectsUnsupportedDomainWithTargetedDiagnostic");
    STATIC_REQUIRE_FALSE(HasAdmittedFirst<int>);

    TRACE_ID("invariant.First.DiagnosticNamesSupportedDomains");
    STATIC_REQUIRE(HasAdmittedFirst<Vector<int, 4>>);
    STATIC_REQUIRE(HasAdmittedFirst<Map<int, int, 4>>);
    STATIC_REQUIRE(HasAdmittedFirst<Set<int, 4>>);
    STATIC_REQUIRE(HasAdmittedFirst<Queue<int, 4>>);
    STATIC_REQUIRE(HasAdmittedFirst<String<4>>);
    STATIC_REQUIRE(HasAdmittedFirst<MapEntry<int, int>>);

    volatile int vector_value_raw = 42;
    int vector_value = vector_value_raw;
    auto rv = Vector<int, 4>{vector_value, 20};
    REQUIRE(first(rv) == 42);
    REQUIRE(count(rv) == 2U);

    auto rm = assoc(assoc(Map<int, int, 4>{}, 5, 500), 6, 600);
    REQUIRE((first(rm) == MapEntry<int, int>{5, 500} || first(rm) == MapEntry<int, int>{6, 600}));
    REQUIRE(count(rm) == 2U);

    auto rs = conj(conj(Set<int, 4>{}, 7), 8);
    REQUIRE((first(rs) == 7 || first(rs) == 8));
    REQUIRE(count(rs) == 2U);

    volatile int queue_value_raw = 11;
    int queue_value = queue_value_raw;
    auto rq = Queue<int, 4>{queue_value, 12};
    REQUIRE(first(rq) == 11);

    auto rtext = String<4>{"AB"};
    REQUIRE(first(rtext) == 'A');

    auto re = MapEntry<int, int>{6, 600};
    REQUIRE(first(re) == 6);

    auto ev = Vector<int, 4>{};
    REQUIRE(first(ev) == 0);
    auto em = Map<int, int, 4>{};
    REQUIRE(first(em) == MapEntry<int, int>{});
    auto es = Set<int, 4>{};
    REQUIRE(first(es) == 0);
    auto eq = Queue<int, 4>{};
    REQUIRE(first(eq) == 0);
    auto etext = String<4>{};
    REQUIRE(first(etext) == '\0');
}

TEST_CASE("First observes every producer family by owning element value", "[first]") {
    using namespace cljonic;
    using namespace cljonic::core;
    TRACE_ID("invariant.First.SupportsAllCljonicProducerFamilies");
    TRACE_ID("invariant.First.SupportsNestedAdaptersAndCycles");
    TRACE_ID("invariant.First.ProducerResultIsOwningElementCopy");
    TRACE_ID("invariant.First.ProducerMapEntryResultIsWholeEntry");
    TRACE_ID("invariant.First.ProducerAdmissionAddsNoUnrelatedCapabilities");
    TRACE_ID("invariant.First.EmptyProducerObservationYieldsDefaultElement");

    constexpr auto below_three = [](int n) noexcept { return n < 3; };
    STATIC_REQUIRE(first(range(7, 10)) == 7);
    STATIC_REQUIRE(first(repeat(9)) == 9);
    STATIC_REQUIRE(first(cycle(vector(4, 5))) == 4);
    STATIC_REQUIRE(first(iterate([](int n) noexcept { return n + 1; }, 8)) == 8);
    STATIC_REQUIRE(first(repeatedly([]() noexcept { return 6; })) == 6);
    STATIC_REQUIRE(first(take_while(below_three, vector(1, 2, 4))) == 1);
    STATIC_REQUIRE(first(drop_while(below_three, vector(1, 2, 4))) == 4);
    STATIC_REQUIRE(first(take_while(below_three, drop_while(below_three, vector(1, 4)))) == 0);
    STATIC_REQUIRE(first(cycle(cycle(drop_while(below_three, vector(1, 4))))) == 4);
    STATIC_REQUIRE(first(next(cycle(vector(4, 5)))) == 5);
    STATIC_REQUIRE(first(next(next(cycle(vector(4, 5))))) == 4);
    STATIC_REQUIRE(first(range(2, 2)) == 0);
    STATIC_REQUIRE(first(repeat(0, 7)) == 0);
    STATIC_REQUIRE(first(repeatedly(0, []() noexcept { return 7; })) == 0);
    STATIC_REQUIRE(first(cycle(vector<int>())) == 0);
    STATIC_REQUIRE(first(take_while(below_three, vector(4))) == 0);
    STATIC_REQUIRE(first(drop_while(below_three, vector(1, 2))) == 0);
    STATIC_REQUIRE(first(cycle(cycle(take_while(below_three, vector(4))))) == 0);
    STATIC_REQUIRE(std::same_as<decltype(first(repeat(map_entry(2, 20)))), MapEntry<int, int>>);
    STATIC_REQUIRE(first(repeat(map_entry(2, 20))) == map_entry(2, 20));
    STATIC_REQUIRE(first(take_while([](auto) noexcept { return true; }, hash_map(2, 20))) == map_entry(2, 20));
    STATIC_REQUIRE(first(take_while([](char) noexcept { return true; }, string("AB"))) == 'A');
    STATIC_REQUIRE(first(drop_while([](int n) noexcept { return n < 5; }, queue(3, 5, 6))) == 5);
    STATIC_REQUIRE(first(take_while([](int) noexcept { return true; }, hash_set(7))) == 7);
    constexpr auto unordered_map_source = hash_map(1, 10, 2, 20);
    STATIC_REQUIRE(
        (first(take_while([](const auto&) noexcept { return true; }, unordered_map_source)) == map_entry(1, 10) ||
         first(take_while([](const auto&) noexcept { return true; }, unordered_map_source)) == map_entry(2, 20)));
    STATIC_REQUIRE(
        (first(drop_while([](const auto&) noexcept { return false; }, unordered_map_source)) == map_entry(1, 10) ||
         first(drop_while([](const auto&) noexcept { return false; }, unordered_map_source)) == map_entry(2, 20)));
    constexpr auto unordered_set_source = hash_set(1, 2);
    STATIC_REQUIRE((first(take_while([](int) noexcept { return true; }, unordered_set_source)) == 1 ||
                    first(take_while([](int) noexcept { return true; }, unordered_set_source)) == 2));
    STATIC_REQUIRE((first(drop_while([](int) noexcept { return false; }, unordered_set_source)) == 1 ||
                    first(drop_while([](int) noexcept { return false; }, unordered_set_source)) == 2));
    STATIC_REQUIRE(first(repeat(vector(1, 2)))(0) == 1);
    struct Element {
        int value = 42;
    };
    STATIC_REQUIRE(first(repeat(Element{7})).value == 7);
    STATIC_REQUIRE(first(repeat(0, Element{7})).value == 42);
    STATIC_REQUIRE(std::same_as<decltype(first(repeat(7L))), long>);

    volatile int raw = 7;
    const int value = raw;
    const auto captured = [limit = value](int n) noexcept { return n < limit; };
    const auto adapter = take_while(captured, vector(value - 1, value));
    STATIC_REQUIRE(!std::is_default_constructible_v<std::remove_cv_t<decltype(adapter)>>);
    STATIC_REQUIRE(!std::is_copy_assignable_v<std::remove_cv_t<decltype(adapter)>>);
    REQUIRE(first(adapter) == value - 1);
    REQUIRE(first(cycle(adapter)) == value - 1);
    volatile int first_key_raw = 11;
    volatile int first_value_raw = 110;
    const int first_key = first_key_raw;
    const int first_value = first_value_raw;
    const auto runtime_map_source = hash_map(first_key, first_value, first_key + 1, first_value + 10);
    const auto runtime_map_take = take_while([](const auto&) noexcept { return true; }, runtime_map_source);
    const auto runtime_map_drop = drop_while([](const auto&) noexcept { return false; }, runtime_map_source);
    REQUIRE((first(runtime_map_take) == map_entry(first_key, first_value) ||
             first(runtime_map_take) == map_entry(first_key + 1, first_value + 10)));
    REQUIRE((first(runtime_map_drop) == map_entry(first_key, first_value) ||
             first(runtime_map_drop) == map_entry(first_key + 1, first_value + 10)));
    const auto runtime_set_source = hash_set(first_key, first_key + 1);
    const auto runtime_set_take = take_while([](int) noexcept { return true; }, runtime_set_source);
    const auto runtime_set_drop = drop_while([](int) noexcept { return false; }, runtime_set_source);
    REQUIRE((first(runtime_set_take) == first_key || first(runtime_set_take) == first_key + 1));
    REQUIRE((first(runtime_set_drop) == first_key || first(runtime_set_drop) == first_key + 1));
    const auto source = repeat(vector(value, value + 1));
    auto result = first(source);
    result = assoc(result, 0, 99);
    REQUIRE(first(source)(0) == value);
    REQUIRE(result(0) == 99);
    const auto reserved = repeat(vector<int, 2>(value));
    const auto appended = assoc(first(reserved), 1, value + 1);
    REQUIRE(count(appended) == 2);
    REQUIRE(appended(1) == value + 1);
    REQUIRE(count(first(reserved)) == 1);
    REQUIRE(first(repeat(value)) == value);
    REQUIRE(first(range(value, value + 2)) == value);
}

TEST_CASE("First performs only needed producer evaluation with fresh traversal", "[first]") {
    using namespace cljonic;
    using namespace cljonic::core;
    TRACE_ID("invariant.First.ProducerObservationRespectsOriginalWindowAndEndpoint");
    TRACE_ID("invariant.First.SkipsDoNotReplenishObservationBudget");
    TRACE_ID("invariant.First.ProducerFirstAccessHasNoCountPrepassOrMaterialization");
    TRACE_ID("invariant.First.NoAdvanceAfterFirstElementFound");
    TRACE_ID("invariant.First.IterateReturnsInitialWithoutStep");
    TRACE_ID("invariant.First.RepeatedlyInvokesCallbackOnce");
    TRACE_ID("invariant.First.TakeWhileTestsOnlyFirstSourceCandidate");
    TRACE_ID("invariant.First.DropWhileScansThroughFirstFalseOrWindowEnd");
    TRACE_ID("invariant.First.CyclePreservesLogicalStartWithoutCountPrepass");
    TRACE_ID("invariant.First.RepeatedCallsUseFreshTraversalWithoutCache");
    TRACE_ID("invariant.First.PreservesStateAndAuthorizedEffectScope");
    TRACE_ID("invariant.First.ZeroProducerCapStartsNoTraversalOrCallbacks");

    int produced = 0;
    int tested = 0;
    const auto generator = repeatedly([&produced]() noexcept { return ++produced; });
    REQUIRE(first(generator) == 1);
    REQUIRE(produced == 1);
    REQUIRE(first(generator) == 2);
    REQUIRE(produced == 2);
    const auto iterative = iterate(
        [&produced](int n) noexcept {
            ++produced;
            return n + 1;
        },
        9);
    REQUIRE(first(iterative) == 9);
    REQUIRE(produced == 2);
    const auto prefix = take_while(
        [&tested](int) noexcept {
            ++tested;
            return true;
        },
        generator);
    REQUIRE(first(prefix) == 3);
    REQUIRE(produced == 3);
    REQUIRE(tested == 1);
    const auto suffix = drop_while(
        [&tested](int n) noexcept {
            ++tested;
            return n < 6;
        },
        generator);
    REQUIRE(first(suffix) == 6);
    REQUIRE(produced == 6);
    REQUIRE(tested == 4);
    const auto nested = cycle(cycle(take_while(
        [&tested](int) noexcept {
            ++tested;
            return true;
        },
        generator)));
    REQUIRE(first(nested) == 7);
    REQUIRE(produced == 7);
    REQUIRE(tested == 5);
    REQUIRE(first(nested) == 8);
    REQUIRE(produced == 8);
    REQUIRE(tested == 6);

    const auto finite = repeatedly(2, [&produced]() noexcept { return ++produced; });
    const auto exhausted = drop_while(
        [&tested](int) noexcept {
            ++tested;
            return true;
        },
        finite);
    REQUIRE(first(exhausted) == 0);
    REQUIRE(produced == 10);
    REQUIRE(tested == 8);
    REQUIRE(first(cycle(exhausted)) == 0);
    REQUIRE(produced == 12);
    REQUIRE(tested == 10);
    const auto empty = repeatedly(0, [&produced]() noexcept { return ++produced; });
    REQUIRE(first(cycle(empty)) == 0);
    REQUIRE(produced == 12);
    REQUIRE(first(take_while(
                [&tested](int) noexcept {
                    ++tested;
                    return true;
                },
                empty)) == 0);
    REQUIRE(tested == 10);
    const auto rejected = take_while(
        [&tested](int) noexcept {
            ++tested;
            return false;
        },
        generator);
    REQUIRE(first(rejected) == 0);
    REQUIRE(produced == 13);
    REQUIRE(tested == 11);
    const auto stopped = take_while(
        [&tested](int n) noexcept {
            ++tested;
            return n < 15;
        },
        generator);
    const auto bounded = drop_while([](int) noexcept { return true; }, stopped);
    REQUIRE(first(bounded) == 0);
    REQUIRE(produced == 15);
    REQUIRE(tested == 13);

    struct ValueState {
        mutable int current = 40;
        auto operator()() const noexcept -> int {
            return ++current;
        }
    };
    const auto owned = repeatedly(ValueState{});
    REQUIRE(first(owned) == 41);
    REQUIRE(first(owned) == 41);
    REQUIRE(noexcept(first(owned)));
}
