#include <catch2/catch_test_macros.hpp>

#include "cljonic-test-api.hpp"

#include <array>
#include <concepts>
#include <type_traits>

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

TEST_CASE("DropWhile owns finite original-window suffixes", "[drop-while]") {
    TRACE_ID("entity-fields.DropWhile");
    TRACE_ID("invariant.DropWhile.PredicateFirstSourceSecondExactTwoArity");
    TRACE_ID("invariant.DropWhile.SourceIsClosedCljonicDomainIncludingAdapters");
    TRACE_ID("invariant.DropWhile.SourceElementSatisfiesNothrowStorage");
    TRACE_ID("invariant.DropWhile.ResultIsExplicitOwningProducer");
    TRACE_ID("invariant.DropWhile.ResultElementTypeIsSourceElementType");
    TRACE_ID("invariant.DropWhile.OwnedCopiesAreNothrow");
    TRACE_ID("invariant.DropWhile.NoBorrowedSourceOrMaterializedCache");
    TRACE_ID("invariant.DropWhile.NoCapacityParameterOrTransducerArity");
    TRACE_ID("invariant.DropWhile.SkipsOnlyInitialTruePrefix");
    TRACE_ID("invariant.DropWhile.FirstFalseValueRetained");
    TRACE_ID("invariant.DropWhile.RemainingOriginalWindowYielded");
    TRACE_ID("invariant.DropWhile.AllTrueWindowYieldsEmpty");
    TRACE_ID("invariant.DropWhile.FiniteResultEvenOverUnboundedSource");
    TRACE_ID("invariant.DropWhile.NoClaimOfUnderlyingUnboundedCompletion");
    TRACE_ID("invariant.DropWhile.ExactCountByFreshBoundedTraversal");
    TRACE_ID("invariant.DropWhile.IsEmptyRetainsExactCountZeroSemantics");
    TRACE_ID("invariant.DropWhile.NoAddedElementEqualityOrOrdering");
    TRACE_ID("invariant.DropWhile.MapEntriesCharactersAndFifoPreserved");
    TRACE_ID("invariant.DropWhile.MapSetMembershipDependsOnUnorderedTraversal");
    TRACE_ID("invariant.DropWhile.ConstexprNoexceptNoHeapNoRttiNonmutating");

    constexpr auto predicate = [](int n) noexcept { return n < 3; };
    constexpr auto suffix = cljonic::core::drop_while(predicate, cljonic::core::vector(1, 2, 4, 1));
    STATIC_REQUIRE(cljonic::concepts::CljonicSource<decltype(suffix)>);
    STATIC_REQUIRE(std::same_as<typename decltype(suffix)::value_type, int>);
    STATIC_REQUIRE(suffix.is_finite());
    STATIC_REQUIRE(suffix.count() == 2);
    constexpr auto adapter_observation_cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    constexpr auto unbounded_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, cljonic::core::repeat(7));
    STATIC_REQUIRE(unbounded_suffix.is_finite());
    STATIC_REQUIRE(unbounded_suffix.count() == adapter_observation_cap);
    STATIC_REQUIRE(cljonic::core::equal(suffix, cljonic::core::vector(4, 1)));
    STATIC_REQUIRE(!cljonic::core::is_empty(suffix));
    STATIC_REQUIRE(*suffix.begin() == 4);
    STATIC_REQUIRE(cljonic::core::is_empty(cljonic::core::drop_while(predicate, cljonic::core::vector(1, 2))));
    STATIC_REQUIRE(
        cljonic::core::first(cljonic::core::into(
            cljonic::core::vector<int, 1>(), cljonic::core::drop_while(predicate, cljonic::core::vector(1, 2)))) == 0);
    STATIC_REQUIRE(noexcept(cljonic::core::drop_while(predicate, cljonic::core::vector(1))));
    STATIC_REQUIRE(
        cljonic::core::equal(cljonic::core::drop_while(predicate, cljonic::core::queue(1, 2, 4, 1)), suffix));
    STATIC_REQUIRE(cljonic::core::equal(
        cljonic::core::drop_while([](char c) noexcept { return c < 'c'; }, cljonic::core::string<4>("abcd")),
        cljonic::core::vector('c', 'd')));
    constexpr auto drop_none = [](const auto&) noexcept { return false; };
    constexpr auto all_map_source = cljonic::core::hash_map(1, 10, 2, 20);
    constexpr auto retained_map =
        cljonic::core::into(cljonic::core::hash_map<int, int, 2>(),
                            cljonic::core::drop_while(drop_none, all_map_source));
    STATIC_REQUIRE(retained_map.count() == 2);
    STATIC_REQUIRE(retained_map.contains(1));
    STATIC_REQUIRE(retained_map(1) == 10);
    STATIC_REQUIRE(retained_map.contains(2));
    STATIC_REQUIRE(retained_map(2) == 20);

    constexpr auto all_set_source = cljonic::core::hash_set(1, 2);
    constexpr auto retained_set =
        cljonic::core::into(cljonic::core::hash_set<int, 2>(),
                            cljonic::core::drop_while(drop_none, all_set_source));
    STATIC_REQUIRE(retained_set.count() == 2);
    STATIC_REQUIRE(retained_set.contains(1));
    STATIC_REQUIRE(retained_set.contains(2));
    struct Element {
        int value{};
    };
    STATIC_REQUIRE(cljonic::core::drop_while([](const Element& v) noexcept { return v.value < 2; },
                                             cljonic::core::vector(Element{1}, Element{2}))
                       .count() == 1);
    volatile int start_raw = 1;
    const auto start = start_raw;
    auto source = cljonic::core::vector(start, start + 1, start + 3);
    const auto owned = cljonic::core::drop_while(predicate, source);
    source = cljonic::core::vector(9, 9, 9);
    REQUIRE(cljonic::core::equal(owned, cljonic::core::vector(4)));
    REQUIRE_FALSE(cljonic::core::is_empty(owned));
    const auto runtime_unbounded_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, cljonic::core::repeat(start));
    REQUIRE(runtime_unbounded_suffix.is_finite());
    REQUIRE(runtime_unbounded_suffix.count() == adapter_observation_cap);
    REQUIRE(cljonic::core::drop_while(predicate, cljonic::core::range(start, 5)).count() == 2);
    REQUIRE(cljonic::core::drop_while(predicate, cljonic::core::repeat(3, start)).count() == 0);
    REQUIRE(cljonic::core::drop_while(predicate, cljonic::core::repeat(start)).count() == 0);
    const auto iterated =
        cljonic::core::drop_while(predicate, cljonic::core::iterate([](int n) noexcept { return n + 1; }, start));
    constexpr auto cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    REQUIRE(iterated.count() == cap - 2);
    REQUIRE(*iterated.begin() == 3);
    const auto cycled = cljonic::core::drop_while(predicate, cljonic::core::cycle(cljonic::core::vector(start, 4)));
    REQUIRE(cycled.count() == cap - 1);
    REQUIRE(*cycled.begin() == 4);
}

TEST_CASE("DropWhile defers prefix scanning and stops predicate at first false", "[drop-while]") {
    TRACE_ID("invariant.DropWhile.ConstructionEvaluatesNeitherPredicateNorSource");
    TRACE_ID("invariant.DropWhile.OriginalWindowCappedWithActualEndpoint");
    TRACE_ID("invariant.DropWhile.SmallerSourceWindowWithoutCountPrepass");
    TRACE_ID("invariant.DropWhile.PredicateOnceThroughFirstFalse");
    TRACE_ID("invariant.DropWhile.NoPredicateAfterFirstFailure");
    TRACE_ID("invariant.DropWhile.SkipsConsumeOriginalBudgetWithoutRestart");
    TRACE_ID("invariant.DropWhile.FirstAccessMayScanEntireMatchingPrefix");
    TRACE_ID("invariant.DropWhile.NoAdvancePastWindowOrAfterConsumerStop");
    TRACE_ID("invariant.DropWhile.EmptyTraversalAndZeroCapInvokeNoCallbacks");
    TRACE_ID("invariant.DropWhile.CountMayRepeatCallbacks");
    TRACE_ID("invariant.DropWhile.FreshPredicateStateWithoutCache");
    TRACE_ID("invariant.DropWhile.PreservesStateAndAuthorizedEffectScope");
    TRACE_ID("invariant.DropWhile.PredictionRequiresEquivalentTraversals");

    int produced = 0;
    int tested = 0;
    const auto source = cljonic::core::repeatedly(4, [n = 0, &produced]() mutable noexcept {
        ++produced;
        return ++n;
    });
    const auto suffix = cljonic::core::drop_while(
        [&tested](int n) noexcept {
            ++tested;
            return n < 3;
        },
        source);
    REQUIRE(produced == 0);
    REQUIRE(tested == 0);
    auto iterator = suffix.begin();
    REQUIRE(*iterator == 3);
    REQUIRE(produced == 3);
    REQUIRE(tested == 3);
    auto prior = iterator++;
    REQUIRE(*prior == 3);
    REQUIRE(*iterator == 4);
    REQUIRE(produced == 4);
    REQUIRE(tested == 3);
    prior = iterator;
    REQUIRE(*prior == 4);
    prior = prior;
    REQUIRE(*prior == 4);
    decltype(iterator) empty{};
    prior = empty;
    REQUIRE(prior == suffix.end());
    ++iterator;
    REQUIRE(iterator == suffix.end());
    REQUIRE(produced == 4);
    ++iterator;
    REQUIRE(produced == 4);
    REQUIRE(suffix.count() == 2);
    REQUIRE(produced == 8);
    REQUIRE(tested == 6);
    REQUIRE(cljonic::core::into(cljonic::core::vector<int, 4>(), suffix).count() == 2);
    REQUIRE(produced == 12);
    REQUIRE(tested == 9);
    const auto no_match = cljonic::core::drop_while([](int) noexcept { return false; }, source);
    REQUIRE(no_match.count() == 4);
    const auto all_match = cljonic::core::drop_while([](int) noexcept { return true; }, source);
    REQUIRE(all_match.count() == 0);
    const auto before = tested;
    REQUIRE(cljonic::core::drop_while(
                [&tested](int) noexcept {
                    ++tested;
                    return true;
                },
                cljonic::core::cycle(cljonic::Vector<int, 0>{}))
                .count() == 0);
    REQUIRE(tested == before);
    const auto empty_source = cljonic::core::drop_while([](int) noexcept { return true; },
                                                        cljonic::core::repeatedly(0, [&produced]() noexcept {
                                                            ++produced;
                                                            return 1;
                                                        }));
    const auto before_produced = produced;
    REQUIRE(empty_source.count() == 0);
    REQUIRE(produced == before_produced);
}

TEST_CASE("DropWhile integrates nested producers storage and consumers", "[drop-while]") {
    TRACE_ID("invariant.DropWhile.PredicatePurityAndNoAllocation");
    TRACE_ID("invariant.DropWhile.PredicateNothrowConstElementExactBool");
    TRACE_ID("invariant.DropWhile.InspectableParameterConversionsAreSafe");
    TRACE_ID("invariant.DropWhile.GenericPredicateNothrowExactBoolFallback");
    TRACE_ID("invariant.DropWhile.CapturingPredicatesDoNotRequireDefaultOrAssignment");
    TRACE_ID("invariant.DropWhile.ConditionalNothrowProducerStorage");
    TRACE_ID("invariant.DropWhile.AdapterSourceConsumerAdmission");
    TRACE_ID("invariant.DropWhile.FinitePreflightUsesExistingSemantics");
    TRACE_ID("invariant.DropWhile.LogicalEqualityUsesSequentialElementRules");
    TRACE_ID("invariant.DropWhile.ParameterEqualityAndKeyElementAdmissionUnavailable");
    TRACE_ID("invariant.DropWhile.FirstApiScansThroughFirstFalseOrWindowEnd");
    TRACE_ID("invariant.DropWhile.ExternalSourcesAndMapEntryRejected");
    TRACE_ID("invariant.DropWhile.TargetedOperationDiagnostics");

    constexpr auto predicate = [](int n) noexcept { return n < 3; };
    constexpr auto suffix = cljonic::core::drop_while(predicate, cljonic::core::vector(1, 2, 4, 1));
    STATIC_REQUIRE(cljonic::core::first(suffix) == 4);
    STATIC_REQUIRE(cljonic::concepts::NothrowCollectionElement<std::remove_cv_t<decltype(suffix)>>);
    STATIC_REQUIRE(!cljonic::concepts::StableEqualityComparable<decltype(suffix)>);
    STATIC_REQUIRE(cljonic::concepts_detail::NextSequenceableProducer<decltype(suffix)>);
    STATIC_REQUIRE(cljonic::concepts_detail::NextSequenceableProducer<decltype(cljonic::core::cycle(suffix))>);
    STATIC_REQUIRE(cljonic::core::vector(suffix).count() == 1);
    STATIC_REQUIRE(cljonic::core::drop_while(predicate, suffix).count() == 2);
    STATIC_REQUIRE(cljonic::core::take_while([](int n) noexcept { return n > 2; }, suffix).count() == 1);
    STATIC_REQUIRE(cljonic::core::drop_while(predicate, cljonic::core::take_while([](int n) noexcept { return n < 5; },
                                                                                  cljonic::core::vector(1, 2, 4, 1)))
                       .count() == 2);
    STATIC_REQUIRE(
        cljonic::core::drop_while([](long n) noexcept { return n < 3; }, cljonic::core::vector(short{4})).count() == 1);
    STATIC_REQUIRE(!cljonic::concepts_detail::PredicateProducerArguments<decltype(predicate), std::array<int, 2>>);
    STATIC_REQUIRE(
        !cljonic::concepts_detail::PredicateProducerArguments<decltype(predicate), cljonic::MapEntry<int, int>>);
    const auto captured =
        cljonic::core::drop_while([limit = 3](int n) noexcept { return n < limit; }, cljonic::core::vector(1, 2, 4, 1));
    STATIC_REQUIRE(!std::is_default_constructible_v<decltype(captured)>);
    STATIC_REQUIRE(!cljonic::concepts::NothrowCollectionElement<std::remove_cv_t<decltype(captured)>>);
    REQUIRE(cljonic::core::seq(captured).count() == 2);
    REQUIRE(cljonic::core::map([](int n) noexcept { return n * 2; }, captured).count() == 2);
    REQUIRE(cljonic::core::reduce([](int a, int b) noexcept { return a + b; }, 0, captured) == 5);
    REQUIRE(cljonic::core::filterv([](int n) noexcept { return n > 2; }, captured).count() == 1);
    REQUIRE(cljonic::core::take(1, captured)(0) == 4);
    REQUIRE(cljonic::core::drop(1, captured)(0) == 1);
    REQUIRE(cljonic::core::take(3, cljonic::core::cycle(captured)).count() == 3);
    REQUIRE(cljonic::core::fits_into(cljonic::core::vector<int, 2>(), captured));
    REQUIRE(!cljonic::core::fits_into(cljonic::core::vector<int, 1>(), captured));
    REQUIRE(cljonic::core::fits_into(cljonic::core::queue<int, 2>(), captured));
    REQUIRE(cljonic::core::fits_into(cljonic::core::hash_set<int, 2>(), captured));
    REQUIRE(cljonic::core::equal(captured, cljonic::core::vector(4, 1)));
    REQUIRE(cljonic::core::not_equal(captured, cljonic::core::vector(4, 2)));
    const auto chars =
        cljonic::core::drop_while([](char c) noexcept { return c < 'c'; }, cljonic::core::string<4>("abcd"));
    REQUIRE(cljonic::core::fits_into(cljonic::core::string<2>(), chars));
    const auto entries =
        cljonic::core::drop_while([](const auto&) noexcept { return false; }, cljonic::core::hash_map(1, 2));
    REQUIRE(cljonic::core::fits_into(cljonic::core::hash_map<int, int, 1>(), entries));
    auto defaulted = std::remove_cv_t<decltype(suffix)>{};
    REQUIRE(defaulted.count() == 0);
    defaulted = suffix;
    REQUIRE(defaulted.count() == 2);
    struct StatefulPredicate {
        mutable int tested{};
        auto operator()(int) const noexcept -> bool {
            return ++tested < 3;
        }
    };
    const auto stateful = cljonic::core::drop_while(StatefulPredicate{}, cljonic::core::vector(1, 2, 3, 4));
    REQUIRE(stateful.count() == 2);
    REQUIRE(stateful.count() == 2);
    int value = 0;
    const auto changing = cljonic::core::drop_while(
        [](int n) noexcept { return n >= 3; }, cljonic::core::repeatedly(4, [&value]() noexcept { return ++value; }));
    REQUIRE(changing.count() == 4);
    REQUIRE(changing.count() == 0);
    REQUIRE(cljonic::core::filter([](int) noexcept { return true; }, changing).count() == 0);
    REQUIRE(cljonic::core::seq(changing).count() == 0);
    REQUIRE(cljonic::core::map([](int n) noexcept { return n; }, changing).count() == 0);
}
