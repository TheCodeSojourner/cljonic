#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <type_traits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

TEST_CASE("Predicate tails own exact types and omit outputs within original boundaries",
          "[next][rest][take-while][drop-while]") {
    using namespace cljonic;
    using namespace cljonic::core;
    TRACE_ID("invariant.Next.AdapterAndDependentCycleTailDomain");
    TRACE_ID("invariant.Next.NestedAdapterExactOwningType");
    TRACE_ID("invariant.Next.AdapterSkipsOutputsAfterOriginalTraversal");
    TRACE_ID("invariant.Next.AdapterPreservesOriginalWindowsAndEndpoint");
    TRACE_ID("invariant.Next.AdapterSkipsNeverReplenishBudget");
    TRACE_ID("invariant.Next.TakeWhileTailNeverResumesAfterFalse");
    TRACE_ID("invariant.Next.DropWhileTailDoesNotRestartPrefixRemoval");
    TRACE_ID("invariant.Next.AdapterTailPreservesPredicateAndElementAdmission");
    TRACE_ID("invariant.Rest.AdapterAndDependentCycleExactNextType");
    TRACE_ID("invariant.TakeWhile.TailAndDependentCycleAdmission");
    TRACE_ID("invariant.TakeWhile.DeferredOutputSkipsPreserveWindow");
    TRACE_ID("invariant.TakeWhile.TailPreservesFalseAndEmptyBoundaries");
    TRACE_ID("invariant.DropWhile.TailAndDependentCycleAdmission");
    TRACE_ID("invariant.DropWhile.DeferredOutputSkipsPreserveWindow");
    TRACE_ID("invariant.DropWhile.TailNeverRestartsPrefixRemoval");

    constexpr auto below_three = [limit = 3](int n) noexcept { return n < limit; };
    constexpr auto prefix = take_while(below_three, vector(1, 2, 4, 1));
    constexpr auto suffix = drop_while(below_three, vector(1, 2, 4, 1));
    constexpr auto prefix_tail = next(prefix);
    constexpr auto suffix_tail = rest(suffix);
    STATIC_REQUIRE((std::same_as<decltype(prefix), decltype(prefix_tail)>));
    STATIC_REQUIRE((std::same_as<decltype(suffix), decltype(suffix_tail)>));
    STATIC_REQUIRE(!std::is_default_constructible_v<decltype(below_three)>);
    STATIC_REQUIRE(!std::is_copy_assignable_v<decltype(below_three)>);
    STATIC_REQUIRE(noexcept(next(prefix)));
    STATIC_REQUIRE(noexcept(rest(suffix)));
    STATIC_REQUIRE(equal(prefix_tail, vector(2)));
    STATIC_REQUIRE(equal(suffix_tail, vector(1)));
    STATIC_REQUIRE(equal(prefix, vector(1, 2)));
    STATIC_REQUIRE(equal(suffix, vector(4, 1)));
    STATIC_REQUIRE(count(next(prefix_tail)) == 0);
    STATIC_REQUIRE(count(next(next(prefix_tail))) == 0);
    STATIC_REQUIRE(count(next(suffix_tail)) == 0);
    STATIC_REQUIRE(prefix_tail.is_finite());
    STATIC_REQUIRE(fits_into(Vector<int, 1>{}, prefix_tail));
    STATIC_REQUIRE(into(Vector<int, 1>{}, suffix_tail)(0) == 1);
    STATIC_REQUIRE(count(next(take_while(below_three, vector(4, 1)))) == 0);
    STATIC_REQUIRE(count(next(drop_while(below_three, vector(1, 2)))) == 0);
    STATIC_REQUIRE(count(next(take_while(below_three, Vector<int, 0>{}))) == 0);
    STATIC_REQUIRE(first(next(take_while(below_three, drop_while(below_three, vector(1, 4, 2))))) == 0);
    STATIC_REQUIRE(first(next(drop_while(
                       below_three, take_while([](int n) noexcept { return n < 5; }, vector(1, 4, 2, 5, 1))))) == 2);
    constexpr auto map_tail = next(take_while([](auto) noexcept { return true; }, hash_map(1, 10, 2, 20)));
    STATIC_REQUIRE(count(map_tail) == 1);
    STATIC_REQUIRE(first(map_tail) == map_entry(1, 10) || first(map_tail) == map_entry(2, 20));
    STATIC_REQUIRE(first(rest(drop_while([](char c) noexcept { return c < 'b'; }, string("abc")))) == 'c');
    STATIC_REQUIRE(first(next(take_while(below_three, queue(1, 2, 4)))) == 2);
    constexpr auto set_tail = next(take_while([](auto) noexcept { return true; }, hash_set(1, 2)));
    STATIC_REQUIRE(count(set_tail) == 1);
    STATIC_REQUIRE(first(set_tail) == 1 || first(set_tail) == 2);

    auto source = vector(1, 2, 4);
    const auto owned = next(take_while(below_three, source));
    source = vector(8, 9, 10);
    REQUIRE(first(owned) == 2);
}

TEST_CASE("Predicate tails preserve unordered Map and Set source membership", "[next][rest][take-while][drop-while]") {
    using namespace cljonic;
    using namespace cljonic::core;

    constexpr auto map_source = hash_map(1, 10, 2, 20);
    constexpr auto map_prefix = take_while([](const auto&) noexcept { return true; }, map_source);
    constexpr auto map_suffix = drop_while([](const auto&) noexcept { return false; }, map_source);
    constexpr auto map_prefix_next = next(map_prefix);
    constexpr auto map_prefix_rest = rest(map_prefix);
    constexpr auto map_suffix_next = next(map_suffix);
    constexpr auto map_suffix_rest = rest(map_suffix);
    STATIC_REQUIRE(count(map_prefix_next) == 1U);
    STATIC_REQUIRE(count(map_prefix_rest) == 1U);
    STATIC_REQUIRE(count(map_suffix_next) == 1U);
    STATIC_REQUIRE(count(map_suffix_rest) == 1U);
    STATIC_REQUIRE((first(map_prefix_next) == map_entry(1, 10) || first(map_prefix_next) == map_entry(2, 20)));
    STATIC_REQUIRE((first(map_prefix_rest) == map_entry(1, 10) || first(map_prefix_rest) == map_entry(2, 20)));
    STATIC_REQUIRE((first(map_suffix_next) == map_entry(1, 10) || first(map_suffix_next) == map_entry(2, 20)));
    STATIC_REQUIRE((first(map_suffix_rest) == map_entry(1, 10) || first(map_suffix_rest) == map_entry(2, 20)));

    constexpr auto set_source = hash_set(1, 2);
    constexpr auto set_prefix = take_while([](int) noexcept { return true; }, set_source);
    constexpr auto set_suffix = drop_while([](int) noexcept { return false; }, set_source);
    constexpr auto set_prefix_next = next(set_prefix);
    constexpr auto set_prefix_rest = rest(set_prefix);
    constexpr auto set_suffix_next = next(set_suffix);
    constexpr auto set_suffix_rest = rest(set_suffix);
    STATIC_REQUIRE(count(set_prefix_next) == 1U);
    STATIC_REQUIRE(count(set_prefix_rest) == 1U);
    STATIC_REQUIRE(count(set_suffix_next) == 1U);
    STATIC_REQUIRE(count(set_suffix_rest) == 1U);
    STATIC_REQUIRE((first(set_prefix_next) == 1 || first(set_prefix_next) == 2));
    STATIC_REQUIRE((first(set_prefix_rest) == 1 || first(set_prefix_rest) == 2));
    STATIC_REQUIRE((first(set_suffix_next) == 1 || first(set_suffix_next) == 2));
    STATIC_REQUIRE((first(set_suffix_rest) == 1 || first(set_suffix_rest) == 2));

    volatile int first_raw = 7;
    const int first_value = first_raw;
    const int second_value = first_value + 1;
    const auto runtime_map_source = hash_map(first_value, first_value + 10, second_value, second_value + 10);
    const auto runtime_map_prefix = take_while([](const auto&) noexcept { return true; }, runtime_map_source);
    const auto runtime_map_suffix = drop_while([](const auto&) noexcept { return false; }, runtime_map_source);
    const auto runtime_map_prefix_next = next(runtime_map_prefix);
    const auto runtime_map_prefix_rest = rest(runtime_map_prefix);
    const auto runtime_map_suffix_next = next(runtime_map_suffix);
    const auto runtime_map_suffix_rest = rest(runtime_map_suffix);
    REQUIRE(count(runtime_map_prefix_next) == 1U);
    REQUIRE(count(runtime_map_prefix_rest) == 1U);
    REQUIRE(count(runtime_map_suffix_next) == 1U);
    REQUIRE(count(runtime_map_suffix_rest) == 1U);
    REQUIRE((first(runtime_map_prefix_next) == map_entry(first_value, first_value + 10) ||
             first(runtime_map_prefix_next) == map_entry(second_value, second_value + 10)));
    REQUIRE((first(runtime_map_prefix_rest) == map_entry(first_value, first_value + 10) ||
             first(runtime_map_prefix_rest) == map_entry(second_value, second_value + 10)));
    REQUIRE((first(runtime_map_suffix_next) == map_entry(first_value, first_value + 10) ||
             first(runtime_map_suffix_next) == map_entry(second_value, second_value + 10)));
    REQUIRE((first(runtime_map_suffix_rest) == map_entry(first_value, first_value + 10) ||
             first(runtime_map_suffix_rest) == map_entry(second_value, second_value + 10)));

    const auto runtime_set_source = hash_set(first_value, second_value);
    const auto runtime_set_prefix = take_while([](int) noexcept { return true; }, runtime_set_source);
    const auto runtime_set_suffix = drop_while([](int) noexcept { return false; }, runtime_set_source);
    const auto runtime_set_prefix_next = next(runtime_set_prefix);
    const auto runtime_set_prefix_rest = rest(runtime_set_prefix);
    const auto runtime_set_suffix_next = next(runtime_set_suffix);
    const auto runtime_set_suffix_rest = rest(runtime_set_suffix);
    REQUIRE(count(runtime_set_prefix_next) == 1U);
    REQUIRE(count(runtime_set_prefix_rest) == 1U);
    REQUIRE(count(runtime_set_suffix_next) == 1U);
    REQUIRE(count(runtime_set_suffix_rest) == 1U);
    REQUIRE((first(runtime_set_prefix_next) == first_value || first(runtime_set_prefix_next) == second_value));
    REQUIRE((first(runtime_set_prefix_rest) == first_value || first(runtime_set_prefix_rest) == second_value));
    REQUIRE((first(runtime_set_suffix_next) == first_value || first(runtime_set_suffix_next) == second_value));
    REQUIRE((first(runtime_set_suffix_rest) == first_value || first(runtime_set_suffix_rest) == second_value));
}

TEST_CASE("Predicate tails defer callbacks and replay fresh owned traversal state",
          "[next][rest][take-while][drop-while]") {
    using namespace cljonic;
    using namespace cljonic::core;
    TRACE_ID("invariant.Next.AdapterTailCreationDefersEvaluation");
    TRACE_ID("invariant.Next.ReplayPreservesStateAndEffectScopeWithoutSnapshot");
    TRACE_ID("invariant.Rest.AdapterTailsMatchDeferredNext");
    int produced = 0;
    int tested = 0;
    const auto source = repeatedly(4, [n = 0, &produced]() mutable noexcept {
        ++produced;
        return ++n;
    });
    const auto prefix = take_while(
        [&tested](int n) noexcept {
            ++tested;
            return n < 3;
        },
        source);
    const auto tail = rest(prefix);
    REQUIRE(produced == 0);
    REQUIRE(tested == 0);
    REQUIRE(first(tail) == 2);
    REQUIRE(produced == 2);
    REQUIRE(tested == 2);
    REQUIRE(first(tail) == 2);
    REQUIRE(produced == 4);
    REQUIRE(tested == 4);
    REQUIRE(first(prefix) == 1);
    REQUIRE(produced == 5);
    REQUIRE(tested == 5);
    REQUIRE(count(tail) == 1);
    REQUIRE(produced == 8);
    REQUIRE(tested == 8);
    produced = 0;
    tested = 0;
    const auto suffix = drop_while(
        [&tested](int n) noexcept {
            ++tested;
            return n < 3;
        },
        source);
    const auto suffix_tail = next(suffix);
    REQUIRE(produced == 0);
    REQUIRE(tested == 0);
    REQUIRE(first(suffix_tail) == 4);
    REQUIRE(produced == 4);
    REQUIRE(tested == 3);
    REQUIRE(first(suffix_tail) == 4);
    REQUIRE(produced == 8);
    REQUIRE(tested == 6);
}

TEST_CASE("Predicate tail offsets saturate without replenishing bounded observations", "[next][rest]") {
    using namespace cljonic;
    using namespace cljonic::core;
    TRACE_ID("invariant.Next.AdapterOffsetSaturatesAndEmptyStaysEmpty");
    TRACE_ID("invariant.Next.TailMetadataStaysCompact");
    constexpr auto yes = [](int) noexcept { return true; };
    auto tail = take_while(yes, repeat(7));
    const auto cap = CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    for (std::size_t n = 0; n < cap + 3; ++n) {
        REQUIRE(count(tail) == (n < cap ? cap - n : 0));
        tail = next(tail);
    }
    STATIC_REQUIRE(sizeof(tail) <= sizeof(repeat(7)) + sizeof(yes) + 2 * alignof(std::size_t) + sizeof(std::size_t));
    REQUIRE(count(next(take_while(yes, repeatedly(2, []() noexcept { return 7; })))) == 1);
    REQUIRE(count(next(drop_while([](int) noexcept { return false; }, repeatedly(1, []() noexcept { return 7; })))) ==
            0);
}

TEST_CASE("Adapter Cycle tails count eagerly and retain the whole source period", "[next][rest][cycle]") {
    using namespace cljonic;
    using namespace cljonic::core;
    TRACE_ID("invariant.Next.AdapterCycleTailEagerlyCountsBoundedSource");
    TRACE_ID("invariant.Next.AdapterCycleNormalizesBeforeAdvancing");
    TRACE_ID("invariant.Next.AdapterCycleRetainsCompleteSource");
    TRACE_ID("invariant.Next.NestedCycleCallbackTimingFollowsSourceTail");
    TRACE_ID("invariant.Next.FirstObservationStillAvoidsCountPrepass");
    TRACE_ID("invariant.Rest.DependentCycleTailsMatchEagerCompactNext");
    TRACE_ID("invariant.Cycle.AdapterAndNestedCycleTailAdmission");
    TRACE_ID("invariant.Cycle.AdapterTailEagerlyDeterminesBoundedLength");
    TRACE_ID("invariant.Cycle.AdapterLengthCountingMayInvokeCallbacks");
    TRACE_ID("invariant.Cycle.AdapterOffsetNormalizedBeforeSafeAdvance");
    TRACE_ID("invariant.Cycle.AdapterCycleRetainsCompleteSourcePeriod");
    TRACE_ID("invariant.Cycle.EmptyAdapterTailRemainsEmpty");
    TRACE_ID("invariant.Cycle.NestedCyclesRecursivelyTailSource");
    TRACE_ID("invariant.Cycle.RotationMetadataStaysCompact");
    TRACE_ID("invariant.Cycle.ReplayPreservesOwnedStateAndAuthorizedEffects");
    TRACE_ID("invariant.Cycle.FirstObservationStillAvoidsCountPrepass");
    constexpr auto prefix = take_while([](int n) noexcept { return n < 4; }, vector(1, 2, 3, 4));
    constexpr auto cycled = cycle(prefix);
    STATIC_REQUIRE((std::same_as<decltype(next(cycled)), std::remove_cv_t<decltype(cycled)>>));
    STATIC_REQUIRE(sizeof(cycled) <= sizeof(prefix) + 3 * sizeof(std::size_t));
    STATIC_REQUIRE(equal(into(Vector<int, 5>{}, next(cycled)), vector(2, 3, 1, 2, 3)));
    STATIC_REQUIRE(equal(into(Vector<int, 5>{}, rest(cycle(cycle(prefix)))), vector(2, 3, 1, 2, 3)));
    STATIC_REQUIRE(equal(into(Vector<int, 4>{}, next(cycle(next(prefix)))), vector(3, 2, 3, 2)));
    STATIC_REQUIRE(first(next(cycle(drop_while([](int n) noexcept { return n < 3; }, vector(1, 3, 1))))) == 1);
    STATIC_REQUIRE(first(next(cycle(take_while([](int) noexcept { return false; }, vector(1))))) == 0);
    STATIC_REQUIRE(!next(cycled).is_finite());
    STATIC_REQUIRE(!fits_into(Vector<int, 4>{}, next(cycled)));

    auto rotated = cycled;
    for (std::size_t n = 0; n < CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE + 5; ++n) {
        REQUIRE(first(rotated) == static_cast<int>(n % 3 + 1));
        rotated = next(rotated);
    }
    int calls = 0;
    const auto observed = cycle(cycle(take_while(
        [&calls](int n) noexcept {
            ++calls;
            return n < 3;
        },
        vector(1, 2, 4))));
    const auto tail = rest(observed);
    REQUIRE(calls == 3);
    REQUIRE(first(tail) == 2);
    REQUIRE(calls == 5);
    REQUIRE(first(observed) == 1);
    REQUIRE(calls == 6);
}

TEST_CASE("Adapter Cycle tails safely respect changing external traversal endpoints", "[next][cycle]") {
    using namespace cljonic;
    using namespace cljonic::core;
    TRACE_ID("invariant.Cycle.RotationPredictionsRequireEquivalentTraversals");
    auto limit = 4;
    const auto source = cycle(take_while([&limit](int n) noexcept { return n < limit; }, vector(1, 2, 3)));
    const auto tail = next(next(source));
    limit = 2;
    REQUIRE(first(tail) == 0);
    limit = 4;
    REQUIRE(first(tail) == 3);
    limit = 3;
    REQUIRE(first(next(tail)) == 2);
    limit = 4;
    const auto no_offset = next(next(next(source)));
    auto iterator = no_offset.begin();
    REQUIRE(*iterator == 1);
    ++iterator;
    ++iterator;
    limit = 0;
    ++iterator;
    REQUIRE(iterator == no_offset.end());
}

TEST_CASE("Empty predicate tail boundaries invoke no callbacks", "[next][rest][cycle]") {
    // The producer cap probes exercise the same paths with caps 0, 1, and 3.
    TRACE_ID("invariant.Next.ZeroCapTailsDoNotEvaluate");
    TRACE_ID("invariant.Cycle.ZeroCapTailDoesNotEvaluate");
    auto calls = 0;
    const auto empty = cljonic::core::repeatedly(0, [&calls]() noexcept { return ++calls; });
    const auto prefix = cljonic::core::take_while(
        [&calls](int) noexcept {
            ++calls;
            return true;
        },
        empty);
    const auto suffix = cljonic::core::drop_while(
        [&calls](int) noexcept {
            ++calls;
            return true;
        },
        empty);
    REQUIRE(cljonic::core::first(cljonic::core::next(prefix)) == 0);
    REQUIRE(cljonic::core::first(cljonic::core::rest(suffix)) == 0);
    REQUIRE(cljonic::core::first(cljonic::core::rest(cljonic::core::cycle(cljonic::core::cycle(prefix)))) == 0);
    REQUIRE(calls == 0);
}
