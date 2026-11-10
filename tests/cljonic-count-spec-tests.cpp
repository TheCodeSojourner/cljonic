#include <catch2/catch_test_macros.hpp>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

TEST_CASE("Count free function operations", "[count]") {
    using cljonic::Map;
    using cljonic::Queue;
    using cljonic::Range;
    using cljonic::Repeat;
    using cljonic::Set;
    using cljonic::String;
    using cljonic::Vector;
    using cljonic::core::assoc;
    using cljonic::core::conj;
    using cljonic::core::count;

    TRACE_ID("entity-fields.Count");
    TRACE_ID("invariant.Count.CountFreeFunctionSupported");
    TRACE_ID("invariant.Count.SupportsSequenceableCollectionDomain");
    TRACE_ID("invariant.Count.SupportsSequenceableProducerDomain");
    TRACE_ID("invariant.Count.ReturnsExactCollectionCount");
    TRACE_ID("invariant.Count.RejectsUnsupportedDomainWithTargetedDiagnostic");
    TRACE_ID("invariant.Count.NoHeapAllocation");
    TRACE_ID("invariant.Count.NoRtti");
    TRACE_ID("invariant.Count.NoExceptions");
    TRACE_ID("invariant.Count.SingleThreadedExecutionModel");
    TRACE_ID("invariant.Count.ReferentialTransparency");
    TRACE_ID("invariant.Count.ReturnsLogicalSize");

    STATIC_REQUIRE_FALSE(cljonic::concepts::SequenceableCollection<int>);
    STATIC_REQUIRE_FALSE(cljonic::concepts::SequenceableProducer<int>);

    constexpr Vector<int, 4> v{1, 2, 3};
    STATIC_REQUIRE(count(v) == 3U);
    constexpr String<8> text{"abc"};
    STATIC_REQUIRE(count(text) == 3U);
    STATIC_REQUIRE(count(String<8>{}) == 0U);

    constexpr Set<int, 4> s{};
    constexpr auto s1 = conj(s, 10);
    STATIC_REQUIRE(count(s1) == 1U);

    constexpr Map<int, int, 4> m{};
    constexpr auto m1 = assoc(m, 1, 2);
    STATIC_REQUIRE(count(m1) == 1U);

    constexpr Queue<int, 4> q{};
    constexpr auto q1 = conj(q, 7);
    STATIC_REQUIRE(count(q1) == 1U);

    STATIC_REQUIRE(count(s) == 0U); // empty input
    STATIC_REQUIRE(count(m) == 0U);
    STATIC_REQUIRE(count(q) == 0U);
    // Runtime tests for code coverage instrumentation
    volatile int val_raw = 42;
    int val = val_raw;
    auto rv = Vector<int, 4>{1, 2};
    REQUIRE(count(rv) == 2U);
    REQUIRE(count(String<8>{"xy"}) == 2U);
    REQUIRE(count(String<8>{}) == 0U);
    auto rs = Set<int, 4>{};
    REQUIRE(count(conj(rs, val)) == 1U);
}

TEST_CASE("Count free function reports producer observations", "[count][producer]") {
    using cljonic::Range;
    using cljonic::Repeat;
    using cljonic::core::count;
    using cljonic::core::cycle;
    using cljonic::core::drop_while;
    using cljonic::core::iterate;
    using cljonic::core::repeatedly;
    using cljonic::core::take_while;

    TRACE_ID("invariant.Count.FiniteProducerCountIsExact");
    TRACE_ID("invariant.Count.UnboundedProducerCountIsObservableCap");

    constexpr auto finite_range = Range{0, 5};
    constexpr auto finite_repeat = Repeat{3U, 7};
    constexpr auto finite_repeatedly = repeatedly(3U, []() constexpr noexcept { return 42; });
    constexpr auto below_three = [](int value) constexpr noexcept { return value < 3; };
    constexpr auto finite_prefix = take_while(below_three, cljonic::core::vector(1, 2, 4));
    constexpr auto finite_suffix = drop_while(below_three, cljonic::core::vector(1, 2, 4));
    STATIC_REQUIRE(count(finite_range) == 5U);
    STATIC_REQUIRE(count(finite_repeat) == 3U);
    STATIC_REQUIRE(count(finite_repeatedly) == 3U);
    STATIC_REQUIRE(count(finite_prefix) == 2U);
    STATIC_REQUIRE(count(finite_suffix) == 1U);

    constexpr auto observation_cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    constexpr auto unbounded_range = Range{0, 0, 0};
    constexpr auto unbounded_repeat = Repeat{7};
    constexpr auto unbounded_cycle = cycle(Range{1, 4});
    constexpr auto unbounded_iterate = iterate([](int value) constexpr noexcept { return value + 1; }, 0);
    constexpr auto unbounded_repeatedly = repeatedly([]() constexpr noexcept { return 42; });
    STATIC_REQUIRE(count(unbounded_range) == observation_cap);
    STATIC_REQUIRE(count(unbounded_repeat) == observation_cap);
    STATIC_REQUIRE(count(unbounded_cycle) == observation_cap);
    STATIC_REQUIRE(count(unbounded_iterate) == observation_cap);
    STATIC_REQUIRE(count(unbounded_repeatedly) == observation_cap);

    const auto runtime_producer = Range{2, 6};
    REQUIRE(count(runtime_producer) == 4U);
    const auto runtime_prefix = take_while(below_three, cljonic::core::vector(1, 2, 4));
    const auto runtime_suffix = drop_while(below_three, cljonic::core::vector(1, 2, 4));
    REQUIRE(count(runtime_prefix) == 2U);
    REQUIRE(count(runtime_suffix) == 1U);
}

TEST_CASE("Count observes unordered-source TakeWhile and DropWhile adapters", "[count][adapter]") {
    using namespace cljonic;
    using namespace cljonic::core;

    constexpr auto map_source = hash_map(1, 10, 2, 20);
    constexpr auto map_prefix = take_while([](const auto&) constexpr noexcept { return true; }, map_source);
    constexpr auto map_empty_prefix = take_while([](const auto&) constexpr noexcept { return false; }, map_source);
    constexpr auto map_suffix = drop_while([](const auto&) constexpr noexcept { return false; }, map_source);
    constexpr auto map_empty_suffix = drop_while([](const auto&) constexpr noexcept { return true; }, map_source);
    constexpr auto set_source = hash_set(1, 2);
    constexpr auto set_prefix = take_while([](int) constexpr noexcept { return true; }, set_source);
    constexpr auto set_empty_prefix = take_while([](int) constexpr noexcept { return false; }, set_source);
    constexpr auto set_suffix = drop_while([](int) constexpr noexcept { return false; }, set_source);
    constexpr auto set_empty_suffix = drop_while([](int) constexpr noexcept { return true; }, set_source);

    STATIC_REQUIRE(count(map_prefix) == 2U);
    STATIC_REQUIRE(count(map_empty_prefix) == 0U);
    STATIC_REQUIRE(count(map_suffix) == 2U);
    STATIC_REQUIRE(count(map_empty_suffix) == 0U);
    STATIC_REQUIRE(count(set_prefix) == 2U);
    STATIC_REQUIRE(count(set_empty_prefix) == 0U);
    STATIC_REQUIRE(count(set_suffix) == 2U);
    STATIC_REQUIRE(count(set_empty_suffix) == 0U);

    volatile int raw = 7;
    const int value = raw;
    const auto runtime_map_source = hash_map(value, value + 10, value + 1, value + 20);
    const auto runtime_map_prefix =
        take_while([](const auto&) noexcept { return true; }, runtime_map_source);
    const auto runtime_map_empty_prefix =
        take_while([](const auto&) noexcept { return false; }, runtime_map_source);
    const auto runtime_map_suffix =
        drop_while([](const auto&) noexcept { return false; }, runtime_map_source);
    const auto runtime_map_empty_suffix =
        drop_while([](const auto&) noexcept { return true; }, runtime_map_source);
    const auto runtime_set_source = hash_set(value, value + 1);
    const auto runtime_set_prefix = take_while([](int) noexcept { return true; }, runtime_set_source);
    const auto runtime_set_empty_prefix = take_while([](int) noexcept { return false; }, runtime_set_source);
    const auto runtime_set_suffix = drop_while([](int) noexcept { return false; }, runtime_set_source);
    const auto runtime_set_empty_suffix = drop_while([](int) noexcept { return true; }, runtime_set_source);

    REQUIRE(count(runtime_map_prefix) == 2U);
    REQUIRE(count(runtime_map_empty_prefix) == 0U);
    REQUIRE(count(runtime_map_suffix) == 2U);
    REQUIRE(count(runtime_map_empty_suffix) == 0U);
    REQUIRE(count(runtime_set_prefix) == 2U);
    REQUIRE(count(runtime_set_empty_prefix) == 0U);
    REQUIRE(count(runtime_set_suffix) == 2U);
    REQUIRE(count(runtime_set_empty_suffix) == 0U);
}