#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <type_traits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename T>
concept HasAdmittedHasNext =
    requires(const T& value) { requires(!std::same_as<decltype(cljonic::core::has_next(value)), void>); };
} // namespace

TEST_CASE("has_next reports whether next has an observable result", "[has_next]") {
    using cljonic::Map;
    using cljonic::MapEntry;
    using cljonic::Queue;
    using cljonic::Range;
    using cljonic::Set;
    using cljonic::String;
    using cljonic::Vector;
    using cljonic::core::count;
    using cljonic::core::cycle;
    using cljonic::core::drop_while;
    using cljonic::core::has_next;
    using cljonic::core::hash_map;
    using cljonic::core::hash_set;
    using cljonic::core::iterate;
    using cljonic::core::next;
    using cljonic::core::repeat;
    using cljonic::core::repeatedly;
    using cljonic::core::take_while;

    TRACE_ID("entity-fields.HasNext");
    TRACE_ID("invariant.HasNext.HasNextFreeFunctionSupported");
    TRACE_ID("invariant.HasNext.SupportsNextCollectionDomain");
    TRACE_ID("invariant.HasNext.SupportsNextProducerDomain");
    TRACE_ID("invariant.Next.HasNextDomainMatchesNextProducerDomain");
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Vector<int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Map<int, int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Set<int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<Queue<int, 4>>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableCollection<String<4>>);

    TRACE_ID("invariant.HasNext.ReturnsBooleanPreflight");
    STATIC_REQUIRE(std::same_as<decltype(has_next(Vector<int, 4>{})), bool>);
    STATIC_REQUIRE(std::same_as<decltype(has_next(Range{0, 3})), bool>);

    TRACE_ID("invariant.HasNext.ReturnsTrueWhenNextResultHasObservableElement");
    constexpr auto vector = Vector<int, 4>{10, 20, 30};
    constexpr auto map = Map<int, int, 4>{MapEntry{1, 100}, MapEntry{2, 200}};
    constexpr auto set = Set<int, 4>{10, 20};
    constexpr auto queue = Queue<int, 4>{10, 20};
    constexpr auto text = String<4>{"AB"};
    constexpr auto finite_range = Range{0, 4};
    constexpr auto finite_repeat = repeat(3, 7);
    constexpr auto finite_repeatedly = repeatedly(3, []() noexcept { return 7; });
    constexpr auto iterated = iterate([](int value) noexcept { return value + 1; }, 0);
    constexpr auto taken = take_while([](int value) noexcept { return value < 30; }, vector);
    constexpr auto dropped = drop_while([](int value) noexcept { return value < 3; }, Vector<int, 4>{1, 2, 3, 4});
    constexpr auto unordered_map_source = hash_map(1, 10, 2, 20);
    constexpr auto unordered_map_taken =
        take_while([](const auto&) noexcept { return true; }, unordered_map_source);
    constexpr auto unordered_map_empty_take =
        take_while([](const auto&) noexcept { return false; }, unordered_map_source);
    constexpr auto unordered_map_dropped =
        drop_while([](const auto&) noexcept { return false; }, unordered_map_source);
    constexpr auto unordered_map_empty_drop =
        drop_while([](const auto&) noexcept { return true; }, unordered_map_source);
    constexpr auto unordered_set_source = hash_set(10, 20);
    constexpr auto unordered_set_taken = take_while([](int) noexcept { return true; }, unordered_set_source);
    constexpr auto unordered_set_empty_take = take_while([](int) noexcept { return false; }, unordered_set_source);
    constexpr auto unordered_set_dropped = drop_while([](int) noexcept { return false; }, unordered_set_source);
    constexpr auto unordered_set_empty_drop = drop_while([](int) noexcept { return true; }, unordered_set_source);
    constexpr auto repeated_cycle = cycle(Vector<int, 1>{10});
    STATIC_REQUIRE(has_next(vector));
    STATIC_REQUIRE(has_next(map));
    STATIC_REQUIRE(has_next(set));
    STATIC_REQUIRE(has_next(queue));
    STATIC_REQUIRE(has_next(text));
    STATIC_REQUIRE(has_next(finite_range));
    STATIC_REQUIRE(has_next(finite_repeat));
    STATIC_REQUIRE(has_next(finite_repeatedly));
    STATIC_REQUIRE(has_next(taken));
    STATIC_REQUIRE(has_next(dropped));
    STATIC_REQUIRE(has_next(unordered_map_taken));
    STATIC_REQUIRE_FALSE(has_next(unordered_map_empty_take));
    STATIC_REQUIRE(has_next(unordered_map_dropped));
    STATIC_REQUIRE_FALSE(has_next(unordered_map_empty_drop));
    STATIC_REQUIRE(has_next(unordered_set_taken));
    STATIC_REQUIRE_FALSE(has_next(unordered_set_empty_take));
    STATIC_REQUIRE(has_next(unordered_set_dropped));
    STATIC_REQUIRE_FALSE(has_next(unordered_set_empty_drop));
    STATIC_REQUIRE(has_next(repeated_cycle) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U));

    TRACE_ID("invariant.HasNext.ReturnsFalseForEmptyOrSingletonCollection");
    STATIC_REQUIRE_FALSE(has_next(Vector<int, 4>{}));
    STATIC_REQUIRE_FALSE(has_next(Map<int, int, 4>{}));
    STATIC_REQUIRE_FALSE(has_next(Set<int, 4>{}));
    STATIC_REQUIRE_FALSE(has_next(Queue<int, 4>{}));
    STATIC_REQUIRE_FALSE(has_next(String<4>{}));
    STATIC_REQUIRE_FALSE(has_next(Vector<int, 4>{10}));
    STATIC_REQUIRE_FALSE(has_next(Map<int, int, 4>{MapEntry{1, 100}}));
    STATIC_REQUIRE_FALSE(has_next(Set<int, 4>{10}));
    STATIC_REQUIRE_FALSE(has_next(Queue<int, 4>{10}));
    STATIC_REQUIRE_FALSE(has_next(String<4>{"A"}));
    STATIC_REQUIRE_FALSE(has_next(Vector<int, 0>{}));
    STATIC_REQUIRE_FALSE(has_next(Map<int, int, 0>{}));
    STATIC_REQUIRE_FALSE(has_next(Set<int, 0>{}));
    STATIC_REQUIRE_FALSE(has_next(Queue<int, 0>{}));
    STATIC_REQUIRE_FALSE(has_next(String<0>{}));

    TRACE_ID("invariant.HasNext.ReturnsFalseWhenProducerNextResultIsEmpty");
    STATIC_REQUIRE_FALSE(has_next(Range{0, 1}));
    STATIC_REQUIRE_FALSE(has_next(repeat(0, 7)));
    STATIC_REQUIRE_FALSE(has_next(repeat(1, 7)));
    STATIC_REQUIRE_FALSE(has_next(repeatedly(1, []() noexcept { return 7; })));
    STATIC_REQUIRE_FALSE(has_next(take_while([](int) noexcept { return true; }, Vector<int, 1>{1})));
    STATIC_REQUIRE_FALSE(has_next(drop_while([](int) noexcept { return true; }, Vector<int, 1>{1})));
    STATIC_REQUIRE_FALSE(has_next(cycle(Vector<int, 0>{})));
    STATIC_REQUIRE(has_next(iterated) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U));
    STATIC_REQUIRE(has_next(repeatedly([]() noexcept { return 7; })) ==
                   (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U));

    TRACE_ID("invariant.HasNext.CollectionPreflightObservesCountWithoutConstructingNextResult");
    STATIC_REQUIRE(has_next(vector) == (count(vector) > 1));
    STATIC_REQUIRE(has_next(map) == (count(map) > 1));
    STATIC_REQUIRE(has_next(set) == (count(set) > 1));
    STATIC_REQUIRE(has_next(queue) == (count(queue) > 1));
    STATIC_REQUIRE(has_next(text) == (count(text) > 1));

    TRACE_ID("invariant.HasNext.ProducerPreflightConstructsNextAndChecksFirstObservableElement");
    STATIC_REQUIRE(has_next(finite_repeat) == (next(finite_repeat).begin() != next(finite_repeat).end()));
    STATIC_REQUIRE(has_next(taken) == (next(taken).begin() != next(taken).end()));

    TRACE_ID("invariant.HasNext.ProducerPreflightDoesNotCountEntireResult");
    STATIC_REQUIRE(has_next(iterated) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U));
    TRACE_ID("invariant.HasNext.ProducerCallbackBehaviorFollowsNextAndFirstObservation");

    TRACE_ID("invariant.HasNext.PreservesInputValueOwnedState");
    STATIC_REQUIRE(count(vector) == 3U);
    STATIC_REQUIRE(vector(0) == 10);
    STATIC_REQUIRE(count(map) == 2U);
    STATIC_REQUIRE(count(set) == 2U);
    STATIC_REQUIRE(count(queue) == 2U);
    STATIC_REQUIRE(count(text) == 2U);
    STATIC_REQUIRE(count(finite_repeat) == 3U);
    STATIC_REQUIRE(count(finite_repeatedly) == 3U);
    STATIC_REQUIRE(cljonic::core::first(iterated) == 0);

    TRACE_ID("invariant.HasNext.NoHeapAllocation");
    TRACE_ID("invariant.HasNext.NoRtti");
    TRACE_ID("invariant.HasNext.NoExceptions");
    TRACE_ID("invariant.HasNext.ConstexprCapable");
    STATIC_REQUIRE(noexcept(has_next(vector)));
    STATIC_REQUIRE(noexcept(has_next(map)));
    STATIC_REQUIRE(noexcept(has_next(set)));
    STATIC_REQUIRE(noexcept(has_next(queue)));
    STATIC_REQUIRE(noexcept(has_next(text)));
    STATIC_REQUIRE(noexcept(has_next(finite_range)));
    STATIC_REQUIRE(noexcept(has_next(finite_repeat)));
    STATIC_REQUIRE(noexcept(has_next(iterated)));

    TRACE_ID("invariant.HasNext.RejectsUnsupportedDomainWithTargetedDiagnostic");
    TRACE_ID("invariant.HasNext.DiagnosticNamesSupportedDomains");
    STATIC_REQUIRE_FALSE(HasAdmittedHasNext<int>);
    STATIC_REQUIRE_FALSE(HasAdmittedHasNext<MapEntry<int, int>>);
    STATIC_REQUIRE_FALSE(HasAdmittedHasNext<int[2]>);
    STATIC_REQUIRE(HasAdmittedHasNext<Vector<int, 4>>);
    STATIC_REQUIRE(HasAdmittedHasNext<Map<int, int, 4>>);
    STATIC_REQUIRE(HasAdmittedHasNext<Set<int, 4>>);
    STATIC_REQUIRE(HasAdmittedHasNext<Queue<int, 4>>);
    STATIC_REQUIRE(HasAdmittedHasNext<String<4>>);
    STATIC_REQUIRE(HasAdmittedHasNext<cljonic::Range<int>>);
    STATIC_REQUIRE(HasAdmittedHasNext<decltype(finite_repeat)>);
    STATIC_REQUIRE(HasAdmittedHasNext<decltype(iterated)>);
    STATIC_REQUIRE(HasAdmittedHasNext<decltype(finite_repeatedly)>);
    STATIC_REQUIRE(HasAdmittedHasNext<decltype(taken)>);
    STATIC_REQUIRE(HasAdmittedHasNext<decltype(dropped)>);
    STATIC_REQUIRE(HasAdmittedHasNext<decltype(repeated_cycle)>);

    volatile int raw = 40;
    const int first = raw;
    const int second = first + 1;
    const auto runtime_vector = Vector<int, 4>{first, second};
    const auto runtime_map = Map<int, int, 4>{MapEntry{first, second}, MapEntry{second, first}};
    const auto runtime_set = Set<int, 4>{first, second};
    const auto runtime_queue = Queue<int, 4>{first, second};
    const char runtime_characters[]{static_cast<char>('A' + first - 40), 'B', '\0'};
    const auto runtime_text = String<4>{runtime_characters};

    REQUIRE(has_next(runtime_vector));
    REQUIRE(has_next(runtime_map));
    REQUIRE(has_next(runtime_set));
    REQUIRE(has_next(runtime_queue));
    REQUIRE(has_next(runtime_text));
    REQUIRE_FALSE(has_next(Vector<int, 4>{}));
    REQUIRE_FALSE(has_next(Vector<int, 4>{first}));
    REQUIRE(count(runtime_vector) == 2U);
    REQUIRE(runtime_vector(0) == first);
    REQUIRE(count(runtime_map) == 2U);
    REQUIRE(count(runtime_set) == 2U);
    REQUIRE(count(runtime_queue) == 2U);
    REQUIRE(count(runtime_text) == 2U);

    volatile int raw_repeat_count = 0;
    const int runtime_repeat_count = raw_repeat_count;
    const auto runtime_range = Range{first, first + 3};
    const auto runtime_empty_repeat = repeat(runtime_repeat_count, first);
    const auto runtime_repeat = repeat(3, first);
    const auto runtime_unbounded_repeat = repeat(first);
    const auto runtime_adapter =
        take_while([](int value) noexcept { return value < 43; }, Vector<int, 4>{first, second, second + 1});
    const auto runtime_drop_while_with_next =
        drop_while([](int value) noexcept { return value < 42; },
                   Vector<int, 4>{first, second, second + 1, second + 2});
    const auto runtime_drop_while_singleton =
        drop_while([](int value) noexcept { return value < 42; }, Vector<int, 4>{first, second, second + 1});
    const auto runtime_drop_while_empty =
        drop_while([](int) noexcept { return true; }, Vector<int, 4>{first, second});
    const auto runtime_unordered_map_source = hash_map(first, first + 100, second, second + 100);
    const auto runtime_unordered_map_taken =
        take_while([](const auto&) noexcept { return true; }, runtime_unordered_map_source);
    const auto runtime_unordered_map_empty_take =
        take_while([](const auto&) noexcept { return false; }, runtime_unordered_map_source);
    const auto runtime_unordered_map_dropped =
        drop_while([](const auto&) noexcept { return false; }, runtime_unordered_map_source);
    const auto runtime_unordered_map_empty_drop =
        drop_while([](const auto&) noexcept { return true; }, runtime_unordered_map_source);
    const auto runtime_unordered_set_source = hash_set(first, second);
    const auto runtime_unordered_set_taken =
        take_while([](int) noexcept { return true; }, runtime_unordered_set_source);
    const auto runtime_unordered_set_empty_take =
        take_while([](int) noexcept { return false; }, runtime_unordered_set_source);
    const auto runtime_unordered_set_dropped =
        drop_while([](int) noexcept { return false; }, runtime_unordered_set_source);
    const auto runtime_unordered_set_empty_drop =
        drop_while([](int) noexcept { return true; }, runtime_unordered_set_source);
    REQUIRE(has_next(runtime_range));
    REQUIRE_FALSE(has_next(runtime_empty_repeat));
    REQUIRE(has_next(runtime_repeat));
    REQUIRE(has_next(runtime_unbounded_repeat));
    REQUIRE(has_next(runtime_adapter));
    REQUIRE(has_next(runtime_drop_while_with_next));
    REQUIRE_FALSE(has_next(runtime_drop_while_singleton));
    REQUIRE_FALSE(has_next(runtime_drop_while_empty));
    REQUIRE(has_next(runtime_unordered_map_taken));
    REQUIRE_FALSE(has_next(runtime_unordered_map_empty_take));
    REQUIRE(has_next(runtime_unordered_map_dropped));
    REQUIRE_FALSE(has_next(runtime_unordered_map_empty_drop));
    REQUIRE(has_next(runtime_unordered_set_taken));
    REQUIRE_FALSE(has_next(runtime_unordered_set_empty_take));
    REQUIRE(has_next(runtime_unordered_set_dropped));
    REQUIRE_FALSE(has_next(runtime_unordered_set_empty_drop));
    REQUIRE(count(runtime_repeat) == 3U);
    REQUIRE(cljonic::core::first(runtime_repeat) == first);

    auto callback_calls = 0;
    const auto callback_producer = repeatedly([&callback_calls]() noexcept { return ++callback_calls; });
    if constexpr (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U) {
        REQUIRE_FALSE(has_next(callback_producer));
        REQUIRE(callback_calls == 0);
    } else {
        REQUIRE(has_next(callback_producer));
        REQUIRE(callback_calls == 2);
    }
}
