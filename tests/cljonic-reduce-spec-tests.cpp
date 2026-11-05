#include "cljonic-test-api.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string>

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

namespace {

struct IntegerAdder {
    [[nodiscard]] constexpr auto operator()(int lhs, int rhs) const noexcept -> int {
        return lhs + rhs;
    }
};

struct CharMaximum {
    [[nodiscard]] constexpr auto operator()(char lhs, char rhs) const noexcept -> char {
        return lhs > rhs ? lhs : rhs;
    }
};

} // namespace

TEST_CASE("reduce folds a cljonic source into one accumulator value", "[reduce][core]") {
    using cljonic::Range;
    using cljonic::Vector;
    using cljonic::concepts_detail::ReduceCallable;
    using cljonic::concepts_detail::ReduceCallableWithInitial;
    using cljonic::core::cycle;
    using cljonic::core::hash_map;
    using cljonic::core::hash_set;
    using cljonic::core::iterate;
    using cljonic::core::queue;
    using cljonic::core::reduce;
    using cljonic::core::repeat;
    using cljonic::core::repeatedly;
    using cljonic::core::string;
    using cljonic::core::vector;

    TRACE_ID("entity-fields.ReduceFunction");
    TRACE_ID("invariant.ReduceFunction.CanonicalReductionOperation");
    TRACE_ID("invariant.ReduceFunction.ReducingFunctionFirstArgumentRole");
    TRACE_ID("invariant.ReduceFunction.SourceIsCljonicDomainOnly");
    TRACE_ID("invariant.ReduceFunction.ProvidesNoInitialAndExplicitInitialForms");
    TRACE_ID("invariant.ReduceFunction.AccumulatorTypeIsFirstParameterWhenInspectable");
    TRACE_ID("invariant.ReduceFunction.AccumulatorTypeIsInitialValueWhenUninspectable");
    TRACE_ID("invariant.ReduceFunction.NoInitialFormRequiresInspectableSignature");
    TRACE_ID("invariant.ReduceFunction.ExplicitInitialFormAdmitsUninspectableFunction");
    TRACE_ID("invariant.ReduceFunction.ReducingFunctionCopyConstructible");
    TRACE_ID("invariant.ReduceFunction.ReducingFunctionInvokedNothrowAsAccumulatorAndElement");
    TRACE_ID("invariant.ReduceFunction.ReducingFunctionResultTypeIsAccumulator");
    TRACE_ID("invariant.ReduceFunction.AccumulatorSatisfiesNothrowElementContract");
    TRACE_ID("invariant.ReduceFunction.NoInitialFormSeedConvertsSafelyToAccumulator");
    TRACE_ID("invariant.ReduceFunction.NoInitialFormSeedsFromFirstElement");
    TRACE_ID("invariant.ReduceFunction.NoInitialFormFoldsRemainingLeftToRight");
    TRACE_ID("invariant.ReduceFunction.EmptyNoInitialSourceYieldsDefaultValue");
    TRACE_ID("invariant.ReduceFunction.ExplicitInitialFormDoesNotRequireElementToConvertToAccumulator");
    TRACE_ID("invariant.ReduceFunction.ExplicitInitialFormSeedsFromInitialValue");
    TRACE_ID("invariant.ReduceFunction.ExplicitInitialFormFoldsEveryObservedElement");
    TRACE_ID("invariant.ReduceFunction.InitialValueConvertsSafelyToAccumulator");
    TRACE_ID("invariant.ReduceFunction.ConsumedArgumentPositionsUseSafeImplicitConversion");
    TRACE_ID("invariant.ReduceFunction.NarrowingAndDomainCrossingConversionsRejectedAtCompileTime");
    TRACE_ID("invariant.ReduceFunction.ObservedCountSaturatedAtCollectionMaximum");
    TRACE_ID("invariant.ReduceFunction.UnboundedProducerFoldsOverObservationCap");
    TRACE_ID("invariant.ReduceFunction.MapSetTraversalOrderSemanticallyUnordered");
    TRACE_ID("invariant.ReduceFunction.StandardRangeContainerAndNonCljonicSourcesRejectedAtCompileTime");
    TRACE_ID("invariant.ReduceFunction.ThrowingFunctionOrUnstorableAccumulatorRejectedAtCompileTime");
    TRACE_ID("invariant.ReduceFunction.ResultIsAccumulatorValue");
    TRACE_ID("invariant.ReduceFunction.EverySourceValuePreserved");
    TRACE_ID("invariant.ReduceFunction.ConstexprNoexceptNonmutatingNonallocating");
    TRACE_ID("invariant.ReduceFunction.AlwaysTerminatesWithoutEarlyTerminationWrapper");

    // ------------------------------------------------------------------------
    // Both arities: the reducing function is the first argument, the source is
    // last, and the explicit-initial form inserts the initial value before it.
    // ------------------------------------------------------------------------
    constexpr auto add = [](int lhs, int rhs) noexcept { return lhs + rhs; };
    constexpr auto values = vector(1, 2, 3);
    STATIC_REQUIRE(reduce(add, values) == 6);
    STATIC_REQUIRE(reduce(add, 10, values) == 16);

    // ------------------------------------------------------------------------
    // Accumulator type: the first declared parameter type when the signature is
    // inspectable, otherwise the initial value's type.
    // ------------------------------------------------------------------------
    STATIC_REQUIRE(std::is_same_v<const int, const decltype(reduce(add, values))>);
    STATIC_REQUIRE(std::is_same_v<const int, const decltype(reduce(add, 10, values))>);

    constexpr auto add_long = [](long lhs, long rhs) noexcept { return lhs + rhs; };
    STATIC_REQUIRE(std::is_same_v<const long, const decltype(reduce(add_long, values))>);
    STATIC_REQUIRE(reduce(add_long, values) == 6L);

    constexpr auto generic_add = [](auto lhs, auto rhs) noexcept { return lhs + rhs; };
    STATIC_REQUIRE(std::is_same_v<const int, const decltype(reduce(generic_add, 5, values))>);
    STATIC_REQUIRE(reduce(generic_add, 5, values) == 11);

    // A generic callable has no inspectable signature, so the form without an
    // initial value is rejected and only the explicit-initial form admits it.
    STATIC_REQUIRE(!ReduceCallable<decltype(generic_add), Vector<int, 3>>);
    STATIC_REQUIRE(ReduceCallableWithInitial<decltype(generic_add), int, Vector<int, 3>>);

    // ------------------------------------------------------------------------
    // Reducing-function contract: copyable, invoked as (accumulator, element)
    // without throwing, returning exactly the accumulator type, which must be a
    // storable element type.
    // ------------------------------------------------------------------------
    STATIC_REQUIRE(std::is_nothrow_copy_constructible_v<decltype(add)>);
    STATIC_REQUIRE(reduce(IntegerAdder{}, values) == 6);
    STATIC_REQUIRE(std::is_same_v<const int, const decltype(reduce(IntegerAdder{}, values))>);
    STATIC_REQUIRE(cljonic::concepts::NothrowCollectionElement<std::remove_cvref_t<decltype(reduce(add, values))>>);

    // ------------------------------------------------------------------------
    // Seeding: the first element seeds the accumulator and the remaining
    // elements fold left to right; the explicit-initial form seeds from the
    // initial value and folds every observed element.
    // ------------------------------------------------------------------------
    constexpr auto subtract = [](int lhs, int rhs) noexcept { return lhs - rhs; };
    STATIC_REQUIRE(reduce(subtract, values) == -4);    // 1 - 2 - 3
    STATIC_REQUIRE(reduce(subtract, 10, values) == 4); // 10 - 1 - 2 - 3

    // An empty source without an initial value yields the accumulator type's
    // default value; with an initial value it yields that initial value.
    STATIC_REQUIRE(reduce(add, vector<int, 0>()) == 0);
    STATIC_REQUIRE(reduce(add, 42, vector<int, 0>()) == 42);

    // ------------------------------------------------------------------------
    // Safe conversions: an identical type or a value-preserving conversion is
    // admitted for each consumed position and for the initial value; narrowing
    // and domain crossings are rejected.
    // ------------------------------------------------------------------------
    STATIC_REQUIRE(reduce(add_long, 1, values) == 7L); // initial int -> long
    STATIC_REQUIRE(ReduceCallable<decltype(add_long), Vector<int, 3>>);

    constexpr auto takes_int = [](int lhs, int rhs) noexcept { return lhs + rhs; };
    constexpr auto takes_int_and_char = [](int lhs, char rhs) noexcept { return lhs + static_cast<int>(rhs); };
    constexpr auto int_acc_long_element = [](int accumulator, long element) noexcept {
        return accumulator + (element > 0L ? 1 : 0);
    };
    STATIC_REQUIRE(!ReduceCallable<decltype(takes_int), Vector<long, 2>>);
    STATIC_REQUIRE(!ReduceCallable<decltype(takes_int_and_char), cljonic::String<4>>);
    STATIC_REQUIRE(!ReduceCallable<decltype(int_acc_long_element), Vector<long, 2>>);
    STATIC_REQUIRE(ReduceCallableWithInitial<decltype(int_acc_long_element), int, Vector<long, 2>>);
    STATIC_REQUIRE(reduce(int_acc_long_element, 5, vector(10L, 20L)) == 7);

    constexpr auto char_accumulator = [](char accumulator, int element) noexcept {
        return static_cast<char>(accumulator + element);
    };
    STATIC_REQUIRE(ReduceCallableWithInitial<decltype(char_accumulator), char, Vector<int, 2>>);
    STATIC_REQUIRE(!ReduceCallable<decltype(char_accumulator), Vector<int, 2>>);
    STATIC_REQUIRE(reduce(char_accumulator, 'a', vector(1, 2)) == 'd');
    STATIC_REQUIRE(!ReduceCallableWithInitial<decltype(takes_int), long, Vector<int, 2>>);

    // ------------------------------------------------------------------------
    // Every supported source family.
    // ------------------------------------------------------------------------
    constexpr auto queue_source = queue(1, 2, 3);
    STATIC_REQUIRE(reduce(add, queue_source) == 6);
    STATIC_REQUIRE(reduce(CharMaximum{}, string<4>("abc")) == 'c');

    // Map and Set traversal order is not semantically significant, so a fold
    // over them yields the same total for any order.
    constexpr auto entry_total = [](int total, const auto& entry) noexcept { return total + entry.value; };
    constexpr auto map_source = hash_map(1, 10, 2, 20, 3, 30);
    STATIC_REQUIRE(reduce(entry_total, 0, map_source) == 60);
    constexpr auto set_source = hash_set(1, 2, 3);
    STATIC_REQUIRE(reduce(add, 0, set_source) == 6);

    // ------------------------------------------------------------------------
    // Producers, finite and unbounded. The observed count is the source count
    // saturated at the collection maximum, so an unbounded producer folds over
    // its observation cap and every call terminates without an early stop.
    // ------------------------------------------------------------------------
    STATIC_REQUIRE(reduce(add, Range{1, 4, 1}) == 6);
    STATIC_REQUIRE(reduce(add, repeat(3, 7)) == 21);
    STATIC_REQUIRE(reduce(add, 0, repeatedly(2, []() noexcept { return 9; })) == 18);

    constexpr auto cap = cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    STATIC_REQUIRE(cap == 1000);
    STATIC_REQUIRE(reduce(add, 0, cycle(vector(1, 2))) == 1500); // 500 ones + 500 twos
    STATIC_REQUIRE(reduce(add, 0, iterate([](int value) noexcept { return value + 1; }, 0)) == 499500);

    // ------------------------------------------------------------------------
    // The result is the accumulator value itself, and every source value
    // survives the operation.
    // ------------------------------------------------------------------------
    STATIC_REQUIRE(std::is_same_v<const int, const decltype(reduce(add, values))>);
    STATIC_REQUIRE(cljonic::core::equal(values, vector(1, 2, 3)));
    STATIC_REQUIRE(cljonic::core::count(queue_source) == 3);

    // noexcept on every admitted call.
    STATIC_REQUIRE(noexcept(reduce(add, values)));
    STATIC_REQUIRE(noexcept(reduce(add, 10, values)));

    // ------------------------------------------------------------------------
    // Compile-time gating: standard-library ranges and containers, throwing
    // functions, unstorable accumulators, and a result that is not the
    // accumulator type are rejected. The dedicated compile-fail harness
    // scripts/check-reduce-compile-failures.py exercises the diagnostics over
    // both builds.
    // ------------------------------------------------------------------------
    constexpr auto throwing_add = [](int lhs, int rhs) { return lhs + rhs; };
    constexpr auto returns_long = [](int lhs, int rhs) noexcept { return static_cast<long>(lhs) + rhs; };
    constexpr auto returns_reference = [](int& accumulator, int) noexcept -> int& { return accumulator; };
    STATIC_REQUIRE(!ReduceCallable<decltype(throwing_add), Vector<int, 2>>);
    STATIC_REQUIRE(!ReduceCallable<decltype(returns_long), Vector<int, 2>>);
    STATIC_REQUIRE(!ReduceCallable<decltype(returns_reference), Vector<int, 2>>);

    // An accumulator that cannot be stored is rejected even when the source and the element
    // conversion are admissible.
    constexpr auto acc_text = [](const std::string& accumulator, int) noexcept { return accumulator; };
    STATIC_REQUIRE(!ReduceCallableWithInitial<decltype(acc_text), std::string, Vector<int, 2>>);

    // ------------------------------------------------------------------------
    // Runtime coverage: volatile inputs defeat constant folding so the
    // assertions execute at runtime (gcov instrumentation).
    // ------------------------------------------------------------------------
    volatile int runtime_raw = 1;
    const auto runtime_base = runtime_raw;
    const auto runtime_values = vector(runtime_base, runtime_base + 1, runtime_base + 2);
    const auto runtime_empty = vector<int, 0>();
    const auto runtime_queue = queue(runtime_base, runtime_base + 1, runtime_base + 2);

    REQUIRE(reduce(add, runtime_empty) == 0);
    REQUIRE(reduce(add, runtime_values) == 6);
    REQUIRE(reduce(add, runtime_base * 10, runtime_values) == 16);
    REQUIRE(reduce(add, runtime_queue) == 6);
    REQUIRE(cljonic::core::equal(runtime_values, vector(1, 2, 3)));

    const auto runtime_set = hash_set(runtime_base, runtime_base + 1, runtime_base + 2);
    REQUIRE(reduce(add, 0, runtime_set) == 6);

    const auto runtime_generic = reduce(generic_add, runtime_base, runtime_values);
    REQUIRE(runtime_generic == 7);

    REQUIRE(noexcept(reduce(add, runtime_values)));
}
