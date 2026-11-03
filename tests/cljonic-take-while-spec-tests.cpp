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

    constexpr auto prefix = cljonic::take_while(below_three, cljonic::vector(1, 2, 4, 1));
    STATIC_REQUIRE(cljonic::concepts::CljonicSource<decltype(prefix)>);
    STATIC_REQUIRE(cljonic::concepts::SequenceableProducer<decltype(prefix)>);
    STATIC_REQUIRE(std::same_as<typename decltype(prefix)::value_type, int>);
    STATIC_REQUIRE(prefix.is_finite());
    STATIC_REQUIRE(prefix.count() == 2);
    STATIC_REQUIRE(cljonic::equal(prefix, cljonic::vector(1, 2)));
    STATIC_REQUIRE(!cljonic::is_empty(prefix));
    STATIC_REQUIRE(cljonic::is_empty(cljonic::take_while(below_three, cljonic::vector(4, 1))));
    STATIC_REQUIRE(*prefix.begin() == 1);
    STATIC_REQUIRE(cljonic::first(cljonic::into(cljonic::vector<int, 1>(),
                                                cljonic::take_while(below_three, cljonic::vector(4)))) == 0);
    STATIC_REQUIRE(noexcept(cljonic::take_while(below_three, cljonic::vector(1))));
    STATIC_REQUIRE(cljonic::equal(cljonic::take_while(below_three, cljonic::queue(1, 2, 4)), prefix));
    STATIC_REQUIRE(
        cljonic::equal(cljonic::take_while([](char c) noexcept { return c < 'c'; }, cljonic::string<4>("abcd")),
                       cljonic::vector('a', 'b')));
    STATIC_REQUIRE(cljonic::take_while([](auto) noexcept { return true; }, cljonic::hash_map(1, 2)).count() == 1);
    STATIC_REQUIRE(cljonic::take_while([](auto) noexcept { return true; }, cljonic::hash_set(1, 2)).count() == 2);
    STATIC_REQUIRE(cljonic::take_while([](const UncomparedValue& v) noexcept { return v.value < 2; },
                                       cljonic::vector(UncomparedValue{1}, UncomparedValue{2}))
                       .count() == 1);

    volatile int initial_raw = 1;
    const auto initial = initial_raw;
    auto source = cljonic::vector(initial, initial + 1, initial + 3);
    const auto owned = cljonic::take_while(below_three, source);
    source = cljonic::vector(9, 9, 9);
    REQUIRE(cljonic::equal(owned, cljonic::vector(initial, initial + 1)));
    REQUIRE(cljonic::take_while(below_three, cljonic::range(initial, 5)).count() == 2);
    REQUIRE(cljonic::take_while(below_three, cljonic::repeat(3, initial)).count() == 3);
    REQUIRE(cljonic::take_while(below_three, cljonic::repeat(initial)).count() ==
            cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
    REQUIRE(cljonic::take_while(below_three, cljonic::cycle(cljonic::vector(1, 4))).count() == 1);
    REQUIRE(cljonic::take_while(below_three, cljonic::iterate([](int n) noexcept { return n + 1; }, initial)).count() ==
            2);
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
    const auto source = cljonic::repeatedly(4, [value = 0, &produced]() mutable noexcept {
        ++produced;
        return ++value;
    });
    const auto prefix = cljonic::take_while(
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
    const auto materialized = cljonic::into(cljonic::vector<int, 4>(), prefix);
    REQUIRE(cljonic::equal(materialized, cljonic::vector(1, 2)));
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

    const auto all =
        cljonic::take_while([](int) noexcept { return true; }, cljonic::repeatedly(2, [&produced]() noexcept {
                                ++produced;
                                return 1;
                            }));
    const auto before = produced;
    REQUIRE(all.count() == 2);
    REQUIRE(produced == before + 2);
    const auto empty_cycle = cljonic::take_while(
        [&tested](int) noexcept {
            ++tested;
            return true;
        },
        cljonic::cycle(cljonic::Vector<int, 0>{}));
    const auto before_test = tested;
    REQUIRE(empty_cycle.count() == 0);
    REQUIRE(tested == before_test);
    REQUIRE(cljonic::filter(below_three, empty_cycle).count() == 0);
    const auto empty_source = cljonic::take_while(below_three, cljonic::repeatedly(0, [&produced]() noexcept {
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
    TRACE_ID("invariant.TakeWhile.TailApisAndDependentCycleTailsRejected");
    TRACE_ID("invariant.TakeWhile.FirstApiRejectsProducer");
    TRACE_ID("invariant.TakeWhile.ExternalSourcesAndMapEntryRejected");
    TRACE_ID("invariant.TakeWhile.TargetedOperationDiagnostics");

    constexpr auto predicate = [](int n) noexcept { return n < 3; };
    constexpr auto prefix = cljonic::take_while(predicate, cljonic::vector(1, 2, 4));
    STATIC_REQUIRE(std::is_void_v<decltype(cljonic::first(prefix))>);
    STATIC_REQUIRE(cljonic::concepts::NothrowCollectionElement<std::remove_cv_t<decltype(prefix)>>);
    STATIC_REQUIRE(!cljonic::concepts::StableEqualityComparable<decltype(prefix)>);
    STATIC_REQUIRE(!cljonic::concepts_detail::NextSequenceableProducer<decltype(prefix)>);
    STATIC_REQUIRE(!cljonic::concepts_detail::NextSequenceableProducer<decltype(cljonic::cycle(prefix))>);
    STATIC_REQUIRE(cljonic::vector(prefix).count() == 1);
    STATIC_REQUIRE(cljonic::take_while(predicate, prefix).count() == 2);
    STATIC_REQUIRE(cljonic::take_while([](long n) noexcept { return n < 3; }, cljonic::vector(short{1})).count() == 1);
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
        cljonic::take_while([limit = 3](int n) noexcept { return n < limit; }, cljonic::vector(1, 2, 4));
    STATIC_REQUIRE(!std::is_default_constructible_v<decltype(captured)>);
    STATIC_REQUIRE(!cljonic::concepts::NothrowCollectionElement<decltype(captured)>);
    REQUIRE(cljonic::seq(captured).count() == 2);
    REQUIRE(cljonic::map([](int n) noexcept { return n * 2; }, captured).count() == 2);
    REQUIRE(cljonic::reduce([](int a, int b) noexcept { return a + b; }, 0, captured) == 3);
    REQUIRE(cljonic::filterv(predicate, captured).count() == 2);
    REQUIRE(cljonic::take(1, captured)(0) == 1);
    REQUIRE(cljonic::drop(1, captured)(0) == 2);
    REQUIRE(cljonic::cycle(captured).count() == cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
    REQUIRE(cljonic::take(3, cljonic::cycle(captured)).count() == 3);
    REQUIRE(cljonic::fits_into(cljonic::vector<int, 2>(), captured));
    REQUIRE(!cljonic::fits_into(cljonic::vector<int, 1>(), captured));
    REQUIRE(cljonic::fits_into(cljonic::queue<int, 2>(), captured));
    REQUIRE(cljonic::fits_into(cljonic::hash_set<int, 2>(), captured));
    REQUIRE(cljonic::equal(captured, cljonic::vector(1, 2)));
    REQUIRE(cljonic::not_equal(captured, cljonic::vector(1, 3)));
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
    const auto stateful = cljonic::take_while(StatefulPredicate{}, cljonic::vector(1, 2, 3, 4));
    REQUIRE(stateful.count() == 2);
    REQUIRE(stateful.count() == 2);
    STATIC_REQUIRE(cljonic::concepts_detail::contains_callable<std::remove_cv_t<decltype(stateful)>>::value);
    const auto floating = cljonic::take_while([](auto) noexcept { return true; }, cljonic::vector(1.0F));
    STATIC_REQUIRE(cljonic::concepts_detail::contains_floating_point<std::remove_cv_t<decltype(floating)>>::value);
    int value = 0;
    const auto changing =
        cljonic::take_while(below_three, cljonic::repeatedly(4, [&value]() noexcept { return ++value; }));
    REQUIRE(changing.count() == 2);
    REQUIRE(changing.count() == 0);
    REQUIRE(cljonic::filter(below_three, changing).count() == 0);
    REQUIRE(cljonic::seq(changing).count() == 0);
    REQUIRE(cljonic::map([](int n) noexcept { return n; }, changing).count() == 0);
}
