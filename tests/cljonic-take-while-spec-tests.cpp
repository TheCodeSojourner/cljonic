#include <catch2/catch_test_macros.hpp>

#include "cljonic-test-api.hpp"

#include <array>
#include <concepts>
#include <type_traits>

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {
constexpr auto below_three(int value) noexcept -> bool {
    return value < 3;
}

struct UncomparedValue {
    int value{};
};
} // namespace

TEST_CASE("TakeWhile owns finite source prefixes with exact counts", "[take-while]") {
    TRACE_ID("entity-fields.TakeWhile");
    TRACE_ID("invariant.TakeWhile.PredicateFirstSourceSecondExactTwoArity");
    TRACE_ID("invariant.TakeWhile.SourceIsClosedCljonicDomainIncludingAdapters");
    TRACE_ID("invariant.TakeWhile.SourceElementSatisfiesNothrowStorage");
    TRACE_ID("invariant.TakeWhile.ResultIsExplicitOwningProducer");
    TRACE_ID("invariant.TakeWhile.ResultElementTypeIsSourceElementType");
    TRACE_ID("invariant.TakeWhile.OwnedCopiesAreNothrow");
    TRACE_ID("invariant.TakeWhile.NoBorrowedSourceOrMaterializedCache");
    TRACE_ID("invariant.TakeWhile.NoCapacityParameterOrTransducerArity");
    TRACE_ID("invariant.TakeWhile.InitialTruePrefixIsYielded");
    TRACE_ID("invariant.TakeWhile.AllTrueWindowYieldedCompletely");
    TRACE_ID("invariant.TakeWhile.FiniteResultEvenOverUnboundedSource");
    TRACE_ID("invariant.TakeWhile.NoClaimOfUnderlyingUnboundedCompletion");
    TRACE_ID("invariant.TakeWhile.ExactCountByFreshBoundedTraversal");
    TRACE_ID("invariant.TakeWhile.IsEmptyRetainsExactCountZeroSemantics");
    TRACE_ID("invariant.TakeWhile.MapEntriesCharactersAndFifoPreserved");
    TRACE_ID("invariant.TakeWhile.MapSetMembershipDependsOnUnorderedTraversal");
    TRACE_ID("invariant.TakeWhile.NoAddedElementEqualityOrOrdering");
    TRACE_ID("invariant.TakeWhile.ConstexprNoexceptNoHeapNoRttiNonmutating");

    constexpr auto prefix = cljonic::core::take_while(below_three, cljonic::core::vector(1, 2, 4, 1));
    STATIC_REQUIRE(cljonic::concepts::CljonicSource<decltype(prefix)>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableProducer<decltype(prefix)>);
    STATIC_REQUIRE(std::same_as<typename decltype(prefix)::value_type, int>);
    STATIC_REQUIRE(prefix.is_finite());
    STATIC_REQUIRE(prefix.count() == 2);
    constexpr auto adapter_observation_cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    constexpr auto unbounded_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, cljonic::core::repeat(7));
    STATIC_REQUIRE(unbounded_prefix.is_finite());
    STATIC_REQUIRE(unbounded_prefix.count() == adapter_observation_cap);
    STATIC_REQUIRE(cljonic::core::equal(prefix, cljonic::core::vector(1, 2)));
    STATIC_REQUIRE(!cljonic::core::is_empty(prefix));
    STATIC_REQUIRE(cljonic::core::is_empty(cljonic::core::take_while(below_three, cljonic::core::vector(4, 1))));
    STATIC_REQUIRE(*prefix.begin() == 1);
    STATIC_REQUIRE(
        cljonic::core::first(cljonic::core::into(
            cljonic::core::vector<int, 1>(), cljonic::core::take_while(below_three, cljonic::core::vector(4)))) == 0);
    STATIC_REQUIRE(noexcept(cljonic::core::take_while(below_three, cljonic::core::vector(1))));
    STATIC_REQUIRE(cljonic::core::equal(cljonic::core::take_while(below_three, cljonic::core::queue(1, 2, 4)), prefix));
    STATIC_REQUIRE(cljonic::core::equal(
        cljonic::core::take_while([](char c) noexcept { return c < 'c'; }, cljonic::core::string<4>("abcd")),
        cljonic::core::vector('a', 'b')));
    constexpr auto keep_all = [](const auto&) noexcept { return true; };
    constexpr auto all_map_source = cljonic::core::hash_map(1, 10, 2, 20);
    constexpr auto retained_map =
        cljonic::core::into(cljonic::core::hash_map<int, int, 2>(),
                            cljonic::core::take_while(keep_all, all_map_source));
    STATIC_REQUIRE(retained_map.count() == 2);
    STATIC_REQUIRE(retained_map.contains(1));
    STATIC_REQUIRE(retained_map(1) == 10);
    STATIC_REQUIRE(retained_map.contains(2));
    STATIC_REQUIRE(retained_map(2) == 20);

    constexpr auto all_set_source = cljonic::core::hash_set(1, 2);
    constexpr auto retained_set =
        cljonic::core::into(cljonic::core::hash_set<int, 2>(),
                            cljonic::core::take_while(keep_all, all_set_source));
    STATIC_REQUIRE(retained_set.count() == 2);
    STATIC_REQUIRE(retained_set.contains(1));
    STATIC_REQUIRE(retained_set.contains(2));
    STATIC_REQUIRE(cljonic::core::take_while([](const UncomparedValue& v) noexcept { return v.value < 2; },
                                             cljonic::core::vector(UncomparedValue{1}, UncomparedValue{2}))
                       .count() == 1);

    volatile int initial_raw = 1;
    const auto initial = initial_raw;
    auto source = cljonic::core::vector(initial, initial + 1, initial + 3);
    const auto owned = cljonic::core::take_while(below_three, source);
    source = cljonic::core::vector(9, 9, 9);
    REQUIRE(cljonic::core::equal(owned, cljonic::core::vector(initial, initial + 1)));
    REQUIRE_FALSE(cljonic::core::is_empty(owned));
    const auto runtime_unbounded_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, cljonic::core::repeat(initial));
    REQUIRE(runtime_unbounded_prefix.is_finite());
    REQUIRE(runtime_unbounded_prefix.count() == adapter_observation_cap);
    REQUIRE(cljonic::core::take_while(below_three, cljonic::core::range(initial, 5)).count() == 2);
    REQUIRE(cljonic::core::take_while(below_three, cljonic::core::repeat(3, initial)).count() == 3);
    REQUIRE(cljonic::core::take_while(below_three, cljonic::core::repeat(initial)).count() ==
            cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
    REQUIRE(cljonic::core::take_while(below_three, cljonic::core::cycle(cljonic::core::vector(1, 4))).count() == 1);
    REQUIRE(
        cljonic::core::take_while(below_three, cljonic::core::iterate([](int n) noexcept { return n + 1; }, initial))
            .count() == 2);
}

TEST_CASE("TakeWhile defers evaluation and preserves traversal boundaries", "[take-while]") {
    TRACE_ID("invariant.TakeWhile.ConstructionEvaluatesNeitherPredicateNorSource");
    TRACE_ID("invariant.TakeWhile.OriginalWindowCappedWithActualEndpoint");
    TRACE_ID("invariant.TakeWhile.SmallerSourceWindowWithoutCountPrepass");
    TRACE_ID("invariant.TakeWhile.PredicateOncePerExaminedElement");
    TRACE_ID("invariant.TakeWhile.FirstFalseValueExaminedButExcluded");
    TRACE_ID("invariant.TakeWhile.NoAdvanceAfterFailureOrPastWindow");
    TRACE_ID("invariant.TakeWhile.EarlyConsumerStopTriggersNoMoreEvaluation");
    TRACE_ID("invariant.TakeWhile.EmptyTraversalAndZeroCapInvokeNoCallbacks");
    TRACE_ID("invariant.TakeWhile.CountMayRepeatCallbacks");
    TRACE_ID("invariant.TakeWhile.FreshPredicateStateWithoutCache");
    TRACE_ID("invariant.TakeWhile.PreservesStateAndAuthorizedEffectScope");
    TRACE_ID("invariant.TakeWhile.PredictionRequiresEquivalentTraversals");

    int produced = 0;
    int tested = 0;
    const auto source = cljonic::core::repeatedly(4, [value = 0, &produced]() mutable noexcept {
        ++produced;
        return ++value;
    });
    const auto prefix = cljonic::core::take_while(
        [&tested](int n) noexcept {
            ++tested;
            return n < 3;
        },
        source);
    REQUIRE(produced == 0);
    REQUIRE(tested == 0);
    REQUIRE(prefix.count() == 2);
    REQUIRE(produced == 3);
    REQUIRE(tested == 3);
    const auto materialized = cljonic::core::into(cljonic::core::vector<int, 4>(), prefix);
    REQUIRE(cljonic::core::equal(materialized, cljonic::core::vector(1, 2)));
    REQUIRE(produced == 6);
    REQUIRE(tested == 6);
    auto current = prefix.begin();
    REQUIRE(*current == 1);
    REQUIRE(produced == 7);
    REQUIRE(tested == 7);
    auto copied = current++;
    REQUIRE(*copied == 1);
    REQUIRE(*current == 2);
    copied = current;
    REQUIRE(*copied == 2);
    copied = copied;
    REQUIRE(*copied == 2);
    ++current;
    REQUIRE(current == prefix.end());
    REQUIRE(produced == 9);
    REQUIRE(tested == 9);
    ++current;
    REQUIRE(produced == 9);
    decltype(current) empty{};
    copied = empty;
    REQUIRE(copied == prefix.end());

    const auto all = cljonic::core::take_while([](int) noexcept { return true; },
                                               cljonic::core::repeatedly(2, [&produced]() noexcept {
                                                   ++produced;
                                                   return 1;
                                               }));
    const auto before = produced;
    REQUIRE(all.count() == 2);
    REQUIRE(produced == before + 2);
    const auto empty_cycle = cljonic::core::take_while(
        [&tested](int) noexcept {
            ++tested;
            return true;
        },
        cljonic::core::cycle(cljonic::Vector<int, 0>{}));
    const auto before_test = tested;
    REQUIRE(empty_cycle.count() == 0);
    REQUIRE(tested == before_test);
    REQUIRE(cljonic::core::filter(below_three, empty_cycle).count() == 0);
    const auto empty_source =
        cljonic::core::take_while(below_three, cljonic::core::repeatedly(0, [&produced]() noexcept {
                                      ++produced;
                                      return 1;
                                  }));
    REQUIRE(empty_source.count() == 0);
    REQUIRE(produced == before + 2);
}

TEST_CASE("TakeWhile integrates callbacks storage and source consumers", "[take-while]") {
    TRACE_ID("invariant.TakeWhile.PredicatePurityAndNoAllocation");
    TRACE_ID("invariant.TakeWhile.PredicateNothrowConstElementExactBool");
    TRACE_ID("invariant.TakeWhile.InspectableParameterConversionsAreSafe");
    TRACE_ID("invariant.TakeWhile.GenericPredicateNothrowExactBoolFallback");
    TRACE_ID("invariant.TakeWhile.CapturingPredicatesDoNotRequireDefaultOrAssignment");
    TRACE_ID("invariant.TakeWhile.ConditionalNothrowProducerStorage");
    TRACE_ID("invariant.TakeWhile.AdapterSourceConsumerAdmission");
    TRACE_ID("invariant.TakeWhile.FinitePreflightUsesExistingSemantics");
    TRACE_ID("invariant.TakeWhile.LogicalEqualityUsesSequentialElementRules");
    TRACE_ID("invariant.TakeWhile.ParameterEqualityAndKeyElementAdmissionUnavailable");
    TRACE_ID("invariant.TakeWhile.FirstApiObservesOnlyFirstCandidate");
    TRACE_ID("invariant.TakeWhile.ExternalSourcesAndMapEntryRejected");
    TRACE_ID("invariant.TakeWhile.TargetedOperationDiagnostics");

    constexpr auto predicate = [](int n) noexcept { return n < 3; };
    constexpr auto prefix = cljonic::core::take_while(predicate, cljonic::core::vector(1, 2, 4));
    STATIC_REQUIRE(cljonic::core::first(prefix) == 1);
    STATIC_REQUIRE(cljonic::concepts::NothrowCollectionElement<std::remove_cv_t<decltype(prefix)>>);
    STATIC_REQUIRE(!cljonic::concepts::StableEqualityComparable<decltype(prefix)>);
    STATIC_REQUIRE(cljonic::concepts_detail::NextSequenceableProducer<decltype(prefix)>);
    STATIC_REQUIRE(cljonic::concepts_detail::NextSequenceableProducer<decltype(cljonic::core::cycle(prefix))>);
    STATIC_REQUIRE(cljonic::core::vector(prefix).count() == 1);
    STATIC_REQUIRE(cljonic::core::take_while(predicate, prefix).count() == 2);
    STATIC_REQUIRE(
        cljonic::core::take_while([](long n) noexcept { return n < 3; }, cljonic::core::vector(short{1})).count() == 1);
    STATIC_REQUIRE(!cljonic::concepts_detail::PredicateProducerArguments<decltype(predicate), std::array<int, 2>>);
    STATIC_REQUIRE(
        !cljonic::concepts_detail::PredicateProducerArguments<decltype(predicate), cljonic::MapEntry<int, int>>);
    STATIC_REQUIRE(!cljonic::concepts_detail::PredicateProducerArguments<decltype([](int n) { return n < 3; }),
                                                                         cljonic::Vector<int, 2>>);
    STATIC_REQUIRE(!cljonic::concepts_detail::PredicateProducerArguments<decltype([](int n) noexcept { return n; }),
                                                                         cljonic::Vector<int, 2>>);
    STATIC_REQUIRE(!cljonic::concepts_detail::PredicateProducerArguments<decltype([](short) noexcept { return true; }),
                                                                         cljonic::Vector<long, 2>>);
    const auto captured =
        cljonic::core::take_while([limit = 3](int n) noexcept { return n < limit; }, cljonic::core::vector(1, 2, 4));
    STATIC_REQUIRE(!std::is_default_constructible_v<decltype(captured)>);
    STATIC_REQUIRE(!cljonic::concepts::NothrowCollectionElement<decltype(captured)>);
    REQUIRE(cljonic::core::seq(captured).count() == 2);
    REQUIRE(cljonic::core::map([](int n) noexcept { return n * 2; }, captured).count() == 2);
    REQUIRE(cljonic::core::reduce([](int a, int b) noexcept { return a + b; }, 0, captured) == 3);
    REQUIRE(cljonic::core::filterv(predicate, captured).count() == 2);
    REQUIRE(cljonic::core::take(1, captured)(0) == 1);
    REQUIRE(cljonic::core::drop(1, captured)(0) == 2);
    REQUIRE(cljonic::core::cycle(captured).count() == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
    REQUIRE(cljonic::core::take(3, cljonic::core::cycle(captured)).count() == 3);
    REQUIRE(cljonic::core::fits_into(cljonic::core::vector<int, 2>(), captured));
    REQUIRE(!cljonic::core::fits_into(cljonic::core::vector<int, 1>(), captured));
    REQUIRE(cljonic::core::fits_into(cljonic::core::queue<int, 2>(), captured));
    REQUIRE(cljonic::core::fits_into(cljonic::core::hash_set<int, 2>(), captured));
    REQUIRE(cljonic::core::equal(captured, cljonic::core::vector(1, 2)));
    REQUIRE(cljonic::core::not_equal(captured, cljonic::core::vector(1, 3)));
    auto defaulted = std::remove_cv_t<decltype(prefix)>{};
    REQUIRE(defaulted.count() == 0);
    defaulted = prefix;
    REQUIRE(defaulted.count() == 2);
    struct StatefulPredicate {
        mutable int tested{};
        auto operator()(int) const noexcept -> bool {
            return ++tested < 3;
        }
    };
    const auto stateful = cljonic::core::take_while(StatefulPredicate{}, cljonic::core::vector(1, 2, 3, 4));
    REQUIRE(stateful.count() == 2);
    REQUIRE(stateful.count() == 2);
    STATIC_REQUIRE(cljonic::concepts_detail::contains_callable<std::remove_cv_t<decltype(stateful)>>::value);
    const auto floating = cljonic::core::take_while([](auto) noexcept { return true; }, cljonic::core::vector(1.0F));
    STATIC_REQUIRE(cljonic::concepts_detail::contains_floating_point<std::remove_cv_t<decltype(floating)>>::value);
    int value = 0;
    const auto changing =
        cljonic::core::take_while(below_three, cljonic::core::repeatedly(4, [&value]() noexcept { return ++value; }));
    REQUIRE(changing.count() == 2);
    REQUIRE(changing.count() == 0);
    REQUIRE(cljonic::core::filter(below_three, changing).count() == 0);
    REQUIRE(cljonic::core::seq(changing).count() == 0);
    REQUIRE(cljonic::core::map([](int n) noexcept { return n; }, changing).count() == 0);
}

TEST_CASE("TakeWhile and DropWhile retain unordered source prefixes and suffixes", "[take-while][drop-while]") {
    using namespace cljonic;
    using namespace cljonic::core;

    TRACE_ID("invariant.TakeWhile.MapSetMembershipDependsOnUnorderedTraversal");
    TRACE_ID("invariant.DropWhile.MapSetMembershipDependsOnUnorderedTraversal");

    constexpr auto map_source = hash_map(1, 10, 2, 20);
    constexpr auto matches_first_map_entry = [](const auto& entry) noexcept { return entry == map_entry(1, 10); };
    constexpr auto map_prefix = into(hash_map<int, int, 2>(), take_while(matches_first_map_entry, map_source));
    constexpr auto map_suffix = into(hash_map<int, int, 2>(), drop_while(matches_first_map_entry, map_source));
    constexpr auto map_keeps_first_at_boundary =
        map_prefix.count() == 1U && map_prefix.contains(1) && map_prefix(1) == 10 && map_suffix.count() == 1U &&
        map_suffix.contains(2) && map_suffix(2) == 20;
    constexpr auto map_starts_after_boundary =
        map_prefix.count() == 0U && map_suffix.count() == 2U && map_suffix.contains(1) && map_suffix(1) == 10 &&
        map_suffix.contains(2) && map_suffix(2) == 20;
    STATIC_REQUIRE(map_keeps_first_at_boundary || map_starts_after_boundary);

    constexpr auto set_source = hash_set(1, 2);
    constexpr auto matches_first_set_element = [](int value) noexcept { return value == 1; };
    constexpr auto set_prefix = into(hash_set<int, 2>(), take_while(matches_first_set_element, set_source));
    constexpr auto set_suffix = into(hash_set<int, 2>(), drop_while(matches_first_set_element, set_source));
    constexpr auto set_keeps_first_at_boundary =
        set_prefix.count() == 1U && set_prefix.contains(1) && set_suffix.count() == 1U && set_suffix.contains(2);
    constexpr auto set_starts_after_boundary =
        set_prefix.count() == 0U && set_suffix.count() == 2U && set_suffix.contains(1) && set_suffix.contains(2);
    STATIC_REQUIRE(set_keeps_first_at_boundary || set_starts_after_boundary);

    volatile int first_key_raw = 7;
    const int first_key = first_key_raw;
    const int second_key = first_key + 1;
    const int first_value = first_key + 10;
    const int second_value = second_key + 10;
    const auto runtime_map_source = hash_map(first_key, first_value, second_key, second_value);
    const auto runtime_matches_first_map_entry =
        [first_key, first_value](const auto& entry) noexcept { return entry == map_entry(first_key, first_value); };
    const auto runtime_map_prefix =
        into(hash_map<int, int, 2>(), take_while(runtime_matches_first_map_entry, runtime_map_source));
    const auto runtime_map_suffix =
        into(hash_map<int, int, 2>(), drop_while(runtime_matches_first_map_entry, runtime_map_source));
    const auto runtime_map_keeps_first_at_boundary =
        runtime_map_prefix.count() == 1U && runtime_map_prefix.contains(first_key) &&
        runtime_map_prefix(first_key) == first_value && runtime_map_suffix.count() == 1U &&
        runtime_map_suffix.contains(second_key) && runtime_map_suffix(second_key) == second_value;
    const auto runtime_map_starts_after_boundary =
        runtime_map_prefix.count() == 0U && runtime_map_suffix.count() == 2U &&
        runtime_map_suffix.contains(first_key) && runtime_map_suffix(first_key) == first_value &&
        runtime_map_suffix.contains(second_key) && runtime_map_suffix(second_key) == second_value;
    REQUIRE((runtime_map_keeps_first_at_boundary || runtime_map_starts_after_boundary));

    const auto runtime_set_source = hash_set(first_key, second_key);
    const auto runtime_matches_first_set_element = [first_key](int value) noexcept { return value == first_key; };
    const auto runtime_set_prefix =
        into(hash_set<int, 2>(), take_while(runtime_matches_first_set_element, runtime_set_source));
    const auto runtime_set_suffix =
        into(hash_set<int, 2>(), drop_while(runtime_matches_first_set_element, runtime_set_source));
    const auto runtime_set_keeps_first_at_boundary =
        runtime_set_prefix.count() == 1U && runtime_set_prefix.contains(first_key) &&
        runtime_set_suffix.count() == 1U && runtime_set_suffix.contains(second_key);
    const auto runtime_set_starts_after_boundary =
        runtime_set_prefix.count() == 0U && runtime_set_suffix.count() == 2U &&
        runtime_set_suffix.contains(first_key) && runtime_set_suffix.contains(second_key);
    REQUIRE((runtime_set_keeps_first_at_boundary || runtime_set_starts_after_boundary));
}
