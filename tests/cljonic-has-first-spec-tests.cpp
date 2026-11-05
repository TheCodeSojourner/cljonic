#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <type_traits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
template <typename T>
concept HasAdmittedHasFirst =
    requires(const T& value) { requires !std::same_as<decltype(cljonic::core::has_first(value)), void>; };
} // namespace

TEST_CASE("has_first reports whether first has an observable result", "[has_first]") {
    using cljonic::Map;
    using cljonic::MapEntry;
    using cljonic::Queue;
    using cljonic::Set;
    using cljonic::String;
    using cljonic::Vector;
    using cljonic::core::count;
    using cljonic::core::cycle;
    using cljonic::core::drop_while;
    using cljonic::core::has_first;
    using cljonic::core::iterate;
    using cljonic::core::range;
    using cljonic::core::repeat;
    using cljonic::core::repeatedly;
    using cljonic::core::take_while;

    TRACE_ID("entity-fields.HasFirst");
    TRACE_ID("invariant.HasFirst.HasFirstFreeFunctionSupported");
    TRACE_ID("invariant.HasFirst.SupportsSequenceableCollectionDomain");
    TRACE_ID("invariant.HasFirst.SupportsFirstProducerDomain");
    TRACE_ID("invariant.HasFirst.SupportsMapEntryDomain");
    TRACE_ID("invariant.HasFirst.ReturnsBooleanPresence");
    STATIC_REQUIRE(std::same_as<decltype(has_first(Vector<int, 4>{})), bool>);
    STATIC_REQUIRE(std::same_as<decltype(has_first(range(0, 3))), bool>);
    STATIC_REQUIRE(std::same_as<decltype(has_first(MapEntry<int, int>{})), bool>);

    TRACE_ID("invariant.HasFirst.ReturnsFalseForEmptyCollection");
    TRACE_ID("invariant.HasFirst.ReturnsTrueForNonemptyCollection");
    constexpr auto values = Vector<int, 4>{10, 20};
    constexpr auto map = Map<int, int, 4>{MapEntry{1, 100}};
    constexpr auto set = Set<int, 4>{10};
    constexpr auto queue = Queue<int, 4>{10};
    constexpr auto text = String<4>{"A"};
    STATIC_REQUIRE(has_first(values));
    STATIC_REQUIRE(has_first(map));
    STATIC_REQUIRE(has_first(set));
    STATIC_REQUIRE(has_first(queue));
    STATIC_REQUIRE(has_first(text));
    STATIC_REQUIRE_FALSE(has_first(Vector<int, 4>{}));
    STATIC_REQUIRE_FALSE(has_first(Map<int, int, 4>{}));
    STATIC_REQUIRE_FALSE(has_first(Set<int, 4>{}));
    STATIC_REQUIRE_FALSE(has_first(Queue<int, 4>{}));
    STATIC_REQUIRE_FALSE(has_first(String<4>{}));
    STATIC_REQUIRE_FALSE(has_first(Vector<int, 0>{}));
    STATIC_REQUIRE_FALSE(has_first(Map<int, int, 0>{}));
    STATIC_REQUIRE_FALSE(has_first(Set<int, 0>{}));
    STATIC_REQUIRE_FALSE(has_first(Queue<int, 0>{}));
    STATIC_REQUIRE_FALSE(has_first(String<0>{}));

    TRACE_ID("invariant.HasFirst.MapAndSetResultIgnoresTraversalOrder");
    STATIC_REQUIRE(has_first(map) == (count(map) > 0U));
    STATIC_REQUIRE(has_first(set) == (count(set) > 0U));

    TRACE_ID("invariant.HasFirst.ProducerResultMatchesFirstObservationPresence");
    constexpr auto yes = [](int) noexcept { return true; };
    constexpr auto no = [](int) noexcept { return false; };
    constexpr auto bounded_range = range(0, 3);
    constexpr auto finite_repeat = repeat(2, 7);
    constexpr auto finite_iterate = iterate([](int value) noexcept { return value + 1; }, 0);
    constexpr auto finite_repeatedly = repeatedly(2, []() noexcept { return 7; });
    constexpr auto prefix = take_while(yes, Vector<int, 2>{1, 2});
    constexpr auto empty_prefix = take_while(no, Vector<int, 2>{1, 2});
    constexpr auto suffix = drop_while([](int value) noexcept { return value < 2; }, Vector<int, 2>{1, 2});
    constexpr auto empty_suffix = drop_while(yes, Vector<int, 2>{1, 2});
    STATIC_REQUIRE(has_first(bounded_range));
    STATIC_REQUIRE(has_first(finite_repeat));
    STATIC_REQUIRE(has_first(finite_repeatedly));
    STATIC_REQUIRE(has_first(prefix));
    STATIC_REQUIRE(has_first(suffix));
    STATIC_REQUIRE(has_first(cycle(Vector<int, 1>{1})) ==
                   (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U));
    STATIC_REQUIRE(has_first(finite_iterate) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U));

    TRACE_ID("invariant.HasFirst.ProducerObservationStopsWhenPresenceIsKnown");
    TRACE_ID("invariant.HasFirst.ProducerObservationDoesNotCountOrMaterialize");
    STATIC_REQUIRE_FALSE(has_first(range(2, 2)));
    STATIC_REQUIRE_FALSE(has_first(repeat(0, 7)));
    STATIC_REQUIRE_FALSE(has_first(repeatedly(0, []() noexcept { return 7; })));
    STATIC_REQUIRE_FALSE(has_first(empty_prefix));
    STATIC_REQUIRE_FALSE(has_first(empty_suffix));
    STATIC_REQUIRE_FALSE(has_first(cycle(Vector<int, 0>{})));
    STATIC_REQUIRE(has_first(repeat(1, 7)));

    TRACE_ID("invariant.HasFirst.ZeroProducerCapReturnsFalseWithoutCallbacks");
    STATIC_REQUIRE(has_first(finite_repeatedly) == (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE > 0U));

    TRACE_ID("invariant.HasFirst.ProducerCallbackBehaviorFollowsFirst");
    TRACE_ID("invariant.HasFirst.PreservesInputValueOwnedState");
    STATIC_REQUIRE(count(values) == 2U);
    STATIC_REQUIRE(values(0) == 10);
    STATIC_REQUIRE(count(bounded_range) == 3U);
    STATIC_REQUIRE(count(finite_repeat) == 2U);

    TRACE_ID("invariant.HasFirst.MapEntryResultIsTrue");
    STATIC_REQUIRE(has_first(MapEntry<int, int>{1, 100}));
    STATIC_REQUIRE(has_first(MapEntry<int, int>{}));

    TRACE_ID("invariant.HasFirst.NoHeapAllocation");
    TRACE_ID("invariant.HasFirst.NoRtti");
    TRACE_ID("invariant.HasFirst.NoExceptions");
    TRACE_ID("invariant.HasFirst.ConstexprCapable");
    STATIC_REQUIRE(noexcept(has_first(values)));
    STATIC_REQUIRE(noexcept(has_first(bounded_range)));
    STATIC_REQUIRE(noexcept(has_first(MapEntry<int, int>{})));

    TRACE_ID("invariant.HasFirst.RejectsUnsupportedDomainWithTargetedDiagnostic");
    TRACE_ID("invariant.HasFirst.DiagnosticNamesSupportedDomains");
    STATIC_REQUIRE_FALSE(HasAdmittedHasFirst<int>);
    STATIC_REQUIRE_FALSE(HasAdmittedHasFirst<int[2]>);
    STATIC_REQUIRE(HasAdmittedHasFirst<Vector<int, 4>>);
    STATIC_REQUIRE(HasAdmittedHasFirst<Map<int, int, 4>>);
    STATIC_REQUIRE(HasAdmittedHasFirst<Set<int, 4>>);
    STATIC_REQUIRE(HasAdmittedHasFirst<Queue<int, 4>>);
    STATIC_REQUIRE(HasAdmittedHasFirst<String<4>>);
    STATIC_REQUIRE(HasAdmittedHasFirst<MapEntry<int, int>>);
    STATIC_REQUIRE(HasAdmittedHasFirst<cljonic::Range<int>>);
    STATIC_REQUIRE(HasAdmittedHasFirst<decltype(finite_repeat)>);
    STATIC_REQUIRE(HasAdmittedHasFirst<decltype(finite_iterate)>);
    STATIC_REQUIRE(HasAdmittedHasFirst<decltype(finite_repeatedly)>);
    STATIC_REQUIRE(HasAdmittedHasFirst<decltype(prefix)>);
    STATIC_REQUIRE(HasAdmittedHasFirst<decltype(suffix)>);

    volatile int raw = 40;
    const int value = raw;
    const auto runtime_values = Vector<int, 4>{value, value + 1};
    const auto runtime_map = Map<int, int, 4>{MapEntry{value, value + 1}};
    const auto runtime_set = Set<int, 4>{value};
    const auto runtime_queue = Queue<int, 4>{value};
    const auto runtime_text = String<4>{"A"};
    REQUIRE(has_first(runtime_values));
    REQUIRE(has_first(runtime_map));
    REQUIRE(has_first(runtime_set));
    REQUIRE(has_first(runtime_queue));
    REQUIRE(has_first(runtime_text));
    REQUIRE_FALSE(has_first(Vector<int, 4>{}));
    REQUIRE_FALSE(has_first(Map<int, int, 4>{}));
    REQUIRE_FALSE(has_first(Set<int, 4>{}));
    REQUIRE_FALSE(has_first(Queue<int, 4>{}));
    REQUIRE_FALSE(has_first(String<4>{}));
    REQUIRE(runtime_values(0) == value);
    REQUIRE(count(runtime_map) == 1U);

    auto callback_calls = 0;
    const auto callback_producer = repeatedly([&callback_calls, value]() noexcept {
        ++callback_calls;
        return value;
    });
    if constexpr (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U) {
        REQUIRE_FALSE(has_first(callback_producer));
        REQUIRE(callback_calls == 0);
    } else {
        REQUIRE(has_first(callback_producer));
        REQUIRE(callback_calls == 1);
    }

    auto predicate_calls = 0;
    const auto filtered = take_while(
        [&predicate_calls](int) noexcept {
            ++predicate_calls;
            return true;
        },
        runtime_values);
    if constexpr (cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U) {
        REQUIRE_FALSE(has_first(filtered));
        REQUIRE(predicate_calls == 0);
    } else {
        REQUIRE(has_first(filtered));
        REQUIRE(predicate_calls == 1);
    }

    REQUIRE(has_first(MapEntry<int, int>{value, value + 1}));
    REQUIRE(has_first(MapEntry<int, int>{}));
}
