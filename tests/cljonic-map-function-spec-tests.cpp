#include "cljonic-test-api.hpp"
#include <catch2/catch_test_macros.hpp>

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {

struct IntegerAdder {
    [[nodiscard]] constexpr auto operator()(int lhs, int rhs) const noexcept -> int {
        return lhs + rhs;
    }
};

} // namespace

TEST_CASE("map applies a transforming function over one or more cljonic sources", "[map][core]") {
    using cljonic::Range;
    using cljonic::Vector;
    using cljonic::core::count;
    using cljonic::core::cycle;
    using cljonic::core::equal;
    using cljonic::core::first;
    using cljonic::core::hash_map;
    using cljonic::core::hash_set;
    using cljonic::core::iterate;
    using cljonic::core::map;
    using cljonic::core::queue;
    using cljonic::core::repeat;
    using cljonic::core::repeatedly;
    using cljonic::core::string;
    using cljonic::core::vector;

    TRACE_ID("entity-fields.MapFunction");
    TRACE_ID("invariant.MapFunction.CanonicalMultiSourceMappingOperation");
    TRACE_ID("invariant.MapFunction.TransformFunctionFirstArgumentRole");
    TRACE_ID("invariant.MapFunction.OneOrMoreCljonicSources");
    TRACE_ID("invariant.MapFunction.UnarySourceFormMapsOverOneSource");
    TRACE_ID("invariant.MapFunction.VariadicSourcesMapCorrespondingElementsPerStep");
    TRACE_ID("invariant.MapFunction.TerminatesWhenFirstSourceExhausted");
    TRACE_ID("invariant.MapFunction.ResultIsBoundedOwningVector");
    TRACE_ID("invariant.MapFunction.ResultElementTypeIsCallbackResult");
    TRACE_ID("invariant.MapFunction.ResultCapacityIsMinimumOfSourceCompileTimeBounds");
    TRACE_ID("invariant.MapFunction.CollectionsContributeDeclaredCapacity");
    TRACE_ID("invariant.MapFunction.ProducersContributeObservationCap");
    TRACE_ID("invariant.MapFunction.RuntimeMappedElementCountIsMinimumOfSourceEffectiveCounts");
    TRACE_ID("invariant.MapFunction.CollectionsContributeElementCount");
    TRACE_ID("invariant.MapFunction.FiniteProducersContributeCount");
    TRACE_ID("invariant.MapFunction.UnboundedProducersContributeObservationCap");
    TRACE_ID("invariant.MapFunction.CapacityContributionsSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.MapFunction.DerivedCapacityExceedingMaximumFailsAtCompileTime");
    TRACE_ID("invariant.MapFunction.CallbackCopyConstructible");
    TRACE_ID("invariant.MapFunction.CallbackInvokedNothrowWithOneArgumentPerSource");
    TRACE_ID("invariant.MapFunction.CallbackResultNothrowStorable");
    TRACE_ID("invariant.MapFunction.CallbackParameterTypesSafelyAcceptCorrespondingElements");
    TRACE_ID("invariant.MapFunction.UninspectableCallablesFallBackToNothrowInvocability");
    TRACE_ID("invariant.MapFunction.ThrowingCallbackOrUnstorableResultRejectedAtCompileTime");
    TRACE_ID("invariant.MapFunction.MapSetTraversalOrderSemanticallyUnordered");
    TRACE_ID("invariant.MapFunction.SourcesFromCljonicDomainOnly");
    TRACE_ID("invariant.MapFunction.StandardRangeContainerAndNonCljonicSourcesRejectedAtCompileTime");
    TRACE_ID("invariant.MapFunction.EverySourceValuePreserved");
    TRACE_ID("invariant.MapFunction.ConstexprNoexceptNonmutatingNonallocating");

    // ------------------------------------------------------------------------
    // Unary form: the result is a bounded owning Vector of callback results in
    // traversal order, with the callback's result type as the element type.
    // ------------------------------------------------------------------------
    constexpr auto inc = [](int value) noexcept { return value + 1; };
    constexpr auto values = vector(1, 2, 3);
    STATIC_REQUIRE(equal(map(inc, values), vector(2, 3, 4)));
    STATIC_REQUIRE(std::is_same_v<const Vector<int, 3>, const decltype(map(inc, values))>);

    // Capacity derivation: a partially filled collection contributes its
    // declared capacity, and the runtime mapped count is its element count.
    constexpr auto wide = vector<int, 8>(1, 2, 3);
    STATIC_REQUIRE(equal(map(inc, wide), vector<int, 8>(2, 3, 4)));
    STATIC_REQUIRE(count(map(inc, wide)) == 3);

    // An empty source yields an empty result.
    STATIC_REQUIRE(count(map(inc, vector<int, 0>())) == 0);

    // Callback result type: a converting callback makes the mapped type.
    constexpr auto to_long = [](int value) noexcept { return static_cast<long>(value); };
    STATIC_REQUIRE(std::is_same_v<const Vector<long, 3>, const decltype(map(to_long, values))>);
    STATIC_REQUIRE(equal(map(to_long, values), vector(1L, 2L, 3L)));

    // ------------------------------------------------------------------------
    // Every supported collection source family.
    // ------------------------------------------------------------------------
    constexpr auto entry_value = [](const auto& entry) noexcept { return entry.value; };
    constexpr auto map_source = hash_map(1, 10, 2, 20);
    STATIC_REQUIRE(
        (equal(map(entry_value, map_source), vector(10, 20)) || equal(map(entry_value, map_source), vector(20, 10))));

    constexpr auto identity = [](int value) noexcept { return value; };
    constexpr auto set_source = hash_set(3, 1, 2);
    STATIC_REQUIRE(
        (equal(map(identity, set_source), vector(1, 2, 3)) || equal(map(identity, set_source), vector(3, 1, 2)) ||
         equal(map(identity, set_source), vector(1, 3, 2)) || equal(map(identity, set_source), vector(2, 1, 3)) ||
         equal(map(identity, set_source), vector(3, 2, 1)) || equal(map(identity, set_source), vector(2, 3, 1))));

    constexpr auto queue_source = queue(7, 8);
    STATIC_REQUIRE(equal(map(inc, queue_source), vector(8, 9)));

    constexpr auto char_succ = [](char value) noexcept { return static_cast<char>(value + 1); };
    STATIC_REQUIRE(equal(map(char_succ, string<4>("abc")), vector('b', 'c', 'd')));

    // ------------------------------------------------------------------------
    // Producer sources, finite and unbounded.
    // ------------------------------------------------------------------------
    STATIC_REQUIRE(equal(map(inc, repeat(3, 7)), vector(8, 8, 8)));
    STATIC_REQUIRE(equal(map(inc, Range{1, 4, 1}), vector(2, 3, 4)));
    constexpr auto cycle_pairs = map(identity, cycle(vector(1, 2)));
    STATIC_REQUIRE(count(cycle_pairs) == 1000); // unbounded Cycle: observation cap
    STATIC_REQUIRE(first(cycle_pairs) == 1);
    constexpr auto iterate_values = map(identity, iterate([](int value) noexcept { return value + 1; }, 0));
    STATIC_REQUIRE(count(iterate_values) == 1000);
    STATIC_REQUIRE(first(iterate_values) == 0);
    STATIC_REQUIRE(equal(map(inc, repeatedly(2, []() noexcept { return 9; })), vector(10, 10)));

    constexpr auto adapter_source = vector(1, 2, 4);
    constexpr auto adapter_prefix =
        cljonic::core::take_while([](int value) noexcept { return value < 3; }, adapter_source);
    constexpr auto adapter_suffix =
        cljonic::core::drop_while([](int value) noexcept { return value < 3; }, adapter_source);
    STATIC_REQUIRE(equal(map(inc, adapter_prefix), vector(2, 3)));
    STATIC_REQUIRE(equal(map(inc, adapter_suffix), vector(5)));

    constexpr auto map_adapter_prefix =
        cljonic::core::take_while([](const auto&) noexcept { return true; }, map_source);
    constexpr auto map_adapter_suffix =
        cljonic::core::drop_while([](const auto&) noexcept { return false; }, map_source);
    constexpr auto mapped_map_prefix = map(entry_value, map_adapter_prefix);
    constexpr auto mapped_map_suffix = map(entry_value, map_adapter_suffix);
    STATIC_REQUIRE(mapped_map_prefix.count() == 2U);
    STATIC_REQUIRE(mapped_map_suffix.count() == 2U);
    STATIC_REQUIRE((equal(mapped_map_prefix, vector(10, 20)) || equal(mapped_map_prefix, vector(20, 10))));
    STATIC_REQUIRE((equal(mapped_map_suffix, vector(10, 20)) || equal(mapped_map_suffix, vector(20, 10))));

    constexpr auto set_adapter_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, set_source);
    constexpr auto set_adapter_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, set_source);
    constexpr auto mapped_set_prefix = map(identity, set_adapter_prefix);
    constexpr auto mapped_set_suffix = map(identity, set_adapter_suffix);
    STATIC_REQUIRE(mapped_set_prefix.count() == 3U);
    STATIC_REQUIRE(mapped_set_suffix.count() == 3U);
    STATIC_REQUIRE(
        (equal(mapped_set_prefix, vector(1, 2, 3)) || equal(mapped_set_prefix, vector(1, 3, 2)) ||
         equal(mapped_set_prefix, vector(2, 1, 3)) || equal(mapped_set_prefix, vector(2, 3, 1)) ||
         equal(mapped_set_prefix, vector(3, 1, 2)) || equal(mapped_set_prefix, vector(3, 2, 1))));
    STATIC_REQUIRE(
        (equal(mapped_set_suffix, vector(1, 2, 3)) || equal(mapped_set_suffix, vector(1, 3, 2)) ||
         equal(mapped_set_suffix, vector(2, 1, 3)) || equal(mapped_set_suffix, vector(2, 3, 1)) ||
         equal(mapped_set_suffix, vector(3, 1, 2)) || equal(mapped_set_suffix, vector(3, 2, 1))));

    // Producer result capacity is the configured observation cap.
    STATIC_REQUIRE(std::is_same_v<const Vector<int, 1000>, const decltype(map(inc, repeat(3, 7)))>);

    // ------------------------------------------------------------------------
    // Multi-source forms: corresponding elements per step, termination when
    // the first source is exhausted.
    // ------------------------------------------------------------------------
    constexpr auto add = IntegerAdder{};
    constexpr auto queue_long = queue(10, 20, 30, 40);
    STATIC_REQUIRE(equal(map(add, values, queue_long), vector(11, 22, 33)));
    // Termination follows the shorter source regardless of position.
    STATIC_REQUIRE(equal(map(add, queue_long, values), vector(11, 22, 33)));
    constexpr auto triple = [](int lhs, int mid, int rhs) noexcept { return lhs + mid + rhs; };
    STATIC_REQUIRE(equal(map(triple, values, Range{1, 4, 1}, repeat(2, 100)), vector(102, 104)));

    // Mixed source families in one call.
    STATIC_REQUIRE(equal(map(add, values, Range{10, 40, 10}), vector(11, 22, 33)));

    // Source preservation: every source value survives the operation.
    STATIC_REQUIRE(equal(values, vector(1, 2, 3)));
    STATIC_REQUIRE(count(queue_long) == 4);
    STATIC_REQUIRE(first(queue_long) == 10);
    STATIC_REQUIRE(count(repeat(2, 100)) == 2);

    // ------------------------------------------------------------------------
    // Safe parameter conversion: each parameter accepts its corresponding
    // source element only through an identical type or a value-preserving
    // arithmetic conversion. Uninspectable callables (generic lambdas) are
    // admitted on nothrow invocability.
    // ------------------------------------------------------------------------
    constexpr auto widen_long = [](long value) noexcept { return value + 1L; };
    constexpr auto widen_double = [](double value) noexcept { return value; };
    STATIC_REQUIRE(equal(map(widen_long, values), vector(2L, 3L, 4L))); // int -> long
    STATIC_REQUIRE(std::is_same_v<const Vector<long, 3>, const decltype(map(widen_long, values))>);
    constexpr auto float_values = vector(1.0F, 2.0F, 3.0F);
    constexpr auto mapped_doubles = map(widen_double, float_values);
    STATIC_REQUIRE(std::is_same_v<const Vector<double, 3>, const decltype(mapped_doubles)>);
    STATIC_REQUIRE(mapped_doubles(0) == 1.0);
    STATIC_REQUIRE(mapped_doubles(1) == 2.0);
    STATIC_REQUIRE(mapped_doubles(2) == 3.0);

    constexpr auto generic_identity = [](auto value) noexcept { return value; };
    STATIC_REQUIRE(equal(map(generic_identity, vector<long, 2>(1L, 2L)), vector(1L, 2L)));

    // ------------------------------------------------------------------------
    // Compile-time gating: the rejection set (floating-point results,
    // non-cljonic sources, throwing callbacks, unstorable results, and the
    // zero-source arity) is exercised by the dedicated compile-fail harness
    // scripts/check-map-compile-failures.py over both builds; see the targeted
    // rejection diagnostics naming map and its supported domain.
    // ------------------------------------------------------------------------

    // ------------------------------------------------------------------------
    // Runtime coverage: volatile inputs defeat constant folding so the
    // assertions execute at runtime (gcov instrumentation).
    // ------------------------------------------------------------------------
    volatile int runtime_raw = 1;
    const auto runtime_base = runtime_raw;
    const auto runtime_values = vector(runtime_base, runtime_base + 1, runtime_base + 2);
    const auto runtime_queue = queue(runtime_base * 10, (runtime_base + 1) * 10, (runtime_base + 2) * 10);
    const auto runtime_pairs = map(add, runtime_values, runtime_queue);
    const auto runtime_singles = map(inc, runtime_values);

    REQUIRE(count(runtime_pairs) == 3);
    REQUIRE(runtime_pairs(0) == 11);
    REQUIRE(runtime_pairs(1) == 22);
    REQUIRE(runtime_pairs(2) == 33);
    REQUIRE(equal(runtime_singles, vector(2, 3, 4)));
    const auto runtime_adapter_source = vector(runtime_base, runtime_base + 1, runtime_base + 3);
    const auto runtime_adapter_prefix =
        cljonic::core::take_while([](int value) noexcept { return value < 3; }, runtime_adapter_source);
    const auto runtime_adapter_suffix =
        cljonic::core::drop_while([](int value) noexcept { return value < 3; }, runtime_adapter_source);
    REQUIRE(equal(map(inc, runtime_adapter_prefix), vector(2, 3)));
    REQUIRE(equal(map(inc, runtime_adapter_suffix), vector(5)));

    // Map runtime mapping preserves unordered traversal semantics.
    const auto runtime_map = hash_map(runtime_base, runtime_base * 10, runtime_base + 1, (runtime_base + 1) * 10);
    const auto runtime_mapped = map(entry_value, runtime_map);
    REQUIRE(count(runtime_mapped) == 2);
    REQUIRE((runtime_mapped(0) == 10 || runtime_mapped(0) == 20));
    const auto runtime_map_adapter_prefix =
        cljonic::core::take_while([](const auto&) noexcept { return true; }, runtime_map);
    const auto runtime_map_adapter_suffix =
        cljonic::core::drop_while([](const auto&) noexcept { return false; }, runtime_map);
    const auto runtime_mapped_map_prefix = map(entry_value, runtime_map_adapter_prefix);
    const auto runtime_mapped_map_suffix = map(entry_value, runtime_map_adapter_suffix);
    REQUIRE(count(runtime_mapped_map_prefix) == 2U);
    REQUIRE(count(runtime_mapped_map_suffix) == 2U);
    REQUIRE((equal(runtime_mapped_map_prefix, vector(runtime_base * 10, (runtime_base + 1) * 10)) ||
             equal(runtime_mapped_map_prefix, vector((runtime_base + 1) * 10, runtime_base * 10))));
    REQUIRE((equal(runtime_mapped_map_suffix, vector(runtime_base * 10, (runtime_base + 1) * 10)) ||
             equal(runtime_mapped_map_suffix, vector((runtime_base + 1) * 10, runtime_base * 10))));

    const auto runtime_set = hash_set(runtime_base, runtime_base + 1, runtime_base + 2);
    const auto runtime_set_adapter_prefix =
        cljonic::core::take_while([](int) noexcept { return true; }, runtime_set);
    const auto runtime_set_adapter_suffix =
        cljonic::core::drop_while([](int) noexcept { return false; }, runtime_set);
    const auto runtime_mapped_set_prefix = map(identity, runtime_set_adapter_prefix);
    const auto runtime_mapped_set_suffix = map(identity, runtime_set_adapter_suffix);
    REQUIRE(count(runtime_mapped_set_prefix) == 3U);
    REQUIRE(count(runtime_mapped_set_suffix) == 3U);
    REQUIRE((equal(runtime_mapped_set_prefix, vector(runtime_base, runtime_base + 1, runtime_base + 2)) ||
             equal(runtime_mapped_set_prefix, vector(runtime_base, runtime_base + 2, runtime_base + 1)) ||
             equal(runtime_mapped_set_prefix, vector(runtime_base + 1, runtime_base, runtime_base + 2)) ||
             equal(runtime_mapped_set_prefix, vector(runtime_base + 1, runtime_base + 2, runtime_base)) ||
             equal(runtime_mapped_set_prefix, vector(runtime_base + 2, runtime_base, runtime_base + 1)) ||
             equal(runtime_mapped_set_prefix, vector(runtime_base + 2, runtime_base + 1, runtime_base))));
    REQUIRE((equal(runtime_mapped_set_suffix, vector(runtime_base, runtime_base + 1, runtime_base + 2)) ||
             equal(runtime_mapped_set_suffix, vector(runtime_base, runtime_base + 2, runtime_base + 1)) ||
             equal(runtime_mapped_set_suffix, vector(runtime_base + 1, runtime_base, runtime_base + 2)) ||
             equal(runtime_mapped_set_suffix, vector(runtime_base + 1, runtime_base + 2, runtime_base)) ||
             equal(runtime_mapped_set_suffix, vector(runtime_base + 2, runtime_base, runtime_base + 1)) ||
             equal(runtime_mapped_set_suffix, vector(runtime_base + 2, runtime_base + 1, runtime_base))));

    // noexcept: every admitted call is non-throwing.
    STATIC_REQUIRE(noexcept(map(inc, values)));
    STATIC_REQUIRE(noexcept(map(add, values, queue_long)));
}
