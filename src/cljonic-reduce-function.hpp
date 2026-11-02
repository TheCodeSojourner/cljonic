#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <ranges>
#include <tuple>
#include <type_traits>

#include <cljonic-concepts.hpp>
#include <cljonic-core-collection-maximum-element-count.hpp>

namespace cljonic {

namespace concepts_detail {

template <typename Source>
using ReduceElement = std::ranges::range_value_t<const Source>;

template <typename Source>
using ReduceElementReference = std::ranges::range_reference_t<const Source>;

inline constexpr std::size_t ReduceRequiredParameterCount = 2U;

// The accumulator type a reducing function declares through its first parameter
// position. A function whose parameter types cannot be inspected reports `void`,
// so it fails the accumulator storage contract instead of forming an ill-formed
// type.
template <typename F>
struct ReduceAccumulatorFromSignature {
    using type = void;
};

template <typename F>
    requires(CallableParameters<std::decay_t<F>>::known &&
             (std::tuple_size_v<CallableParameterTypes<F>> >= ReduceRequiredParameterCount))
struct ReduceAccumulatorFromSignature<F> {
    using type = std::remove_cvref_t<std::tuple_element_t<0, CallableParameterTypes<F>>>;
};

// The accumulator type for the explicit-initial form: the function's first
// parameter type when it is inspectable, otherwise the initial value's type,
// which is the only remaining source for that decision (REQ-FN-034).
template <typename F, typename Initial>
struct ReduceAccumulator {
    using type = std::remove_cvref_t<Initial>;
};

template <typename F, typename Initial>
    requires(CallableParameters<std::decay_t<F>>::known &&
             (std::tuple_size_v<CallableParameterTypes<F>> >= ReduceRequiredParameterCount))
struct ReduceAccumulator<F, Initial> {
    using type = ReduceAccumulatorFromSignature<F>::type;
};

// The reducing function is invoked as (accumulator, element) and must return
// exactly the accumulator type, so the fold step is a nothrow assignment of a
// nothrow element value. The invocability test is the exact call the fold makes.
template <typename F, typename Accumulator, typename Source>
struct ReduceResultMatches : std::false_type {};

template <typename F, typename Accumulator, typename Source>
    requires std::is_nothrow_invocable_v<const std::decay_t<F>&, Accumulator&, ReduceElementReference<Source>>
struct ReduceResultMatches<F, Accumulator, Source>
    : std::bool_constant<std::same_as<
          std::invoke_result_t<const std::decay_t<F>&, Accumulator&, ReduceElementReference<Source>>, Accumulator>> {};

template <typename Parameters, typename Source>
struct ReduceParametersSafe : std::false_type {};

template <typename Parameters, typename Source>
    requires(std::tuple_size_v<Parameters> >= 2)
struct ReduceParametersSafe<Parameters, Source>
    : std::bool_constant<concepts::SafeImplicitConversion<std::tuple_element_t<1, Parameters>, ReduceElement<Source>>> {
};

template <typename Parameters, typename Source>
struct ReduceSeedSafe : std::false_type {};

template <typename Parameters, typename Source>
    requires(std::tuple_size_v<Parameters> >= 2)
struct ReduceSeedSafe<Parameters, Source>
    : std::bool_constant<
          concepts::SafeImplicitConversion<std::decay_t<std::tuple_element_t<0, Parameters>>, ReduceElement<Source>>> {
};

// An uninspectable function has no per-position rule to evaluate: it is admitted
// on nothrow invocability alone (REQ-FN-034).
template <typename F, typename Source>
inline constexpr bool reduce_parameters_safe_v = [] constexpr {
    if constexpr (!CallableParameters<std::decay_t<F>>::known) {
        return true;
    } else {
        return ReduceParametersSafe<CallableParameterTypes<F>, Source>::value;
    }
}();

// Guarded admission for the no-initial form: the accumulator type exists only
// when the function's signature is inspectable, so an uninspectable function is
// rejected here and is admitted only by the explicit-initial form.
template <typename F, typename Source>
concept ReduceCallable = concepts::CljonicSource<Source> && std::copy_constructible<std::decay_t<F>> &&
                         std::is_nothrow_copy_constructible_v<std::decay_t<F>> &&
                         concepts::NothrowCollectionElement<typename ReduceAccumulatorFromSignature<F>::type> &&
                         ReduceResultMatches<F, typename ReduceAccumulatorFromSignature<F>::type, Source>::value &&
                         ReduceParametersSafe<CallableParameterTypes<F>, Source>::value &&
                         ReduceSeedSafe<CallableParameterTypes<F>, Source>::value;

template <typename F, typename Initial, typename Source>
concept ReduceCallableWithInitial =
    concepts::CljonicSource<Source> && std::copy_constructible<std::decay_t<F>> &&
    std::is_nothrow_copy_constructible_v<std::decay_t<F>> &&
    concepts::NothrowCollectionElement<typename ReduceAccumulator<F, Initial>::type> &&
    concepts::SafeImplicitConversion<typename ReduceAccumulator<F, Initial>::type, std::remove_cvref_t<Initial>> &&
    ReduceResultMatches<F, typename ReduceAccumulator<F, Initial>::type, Source>::value &&
    reduce_parameters_safe_v<F, Source>;

// The observed element count is the source's count saturated at the collection
// maximum, so an unbounded producer folds over its observation cap and the
// operation always terminates (REQ-FN-034).
template <typename Source>
[[nodiscard]] constexpr auto reduce_observed_count(const Source& source) noexcept -> std::size_t {
    return std::min<std::size_t>(static_cast<std::size_t>(source.count()),
                                 CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
}

template <typename F, typename Iterator, typename Accumulator>
[[nodiscard]] constexpr auto reduce_fold(const F& function, Accumulator accumulator, Iterator iterator,
                                         std::size_t remaining) noexcept -> Accumulator {
    for (std::size_t step = 0U; step < remaining; ++step) {
        accumulator = std::invoke(function, accumulator, *iterator);
        ++iterator;
    }
    return accumulator;
}

} // namespace concepts_detail

/** \anchor ReduceFunction
 * The \b reduce free function folds a cljonic source into a single value. Two forms are available: `reduce(f, source)`
 * starts from the source's first element, and `reduce(f, initial, source)` starts from an initial value. The source is
 * a \ref Vector "Vector", \ref Map "Map", \ref Set "Set", \ref Queue "Queue", or \ref String "String" collection, or a
 * \ref Range "Range", \ref Repeat "Repeat", \ref Cycle "Cycle", \ref Iterate "Iterate", or \ref Repeatedly
 * "Repeatedly" producer.
 *
 * The reducing function is called as `f(accumulator, element)` from left to right and must return the accumulator
 * type. Its result type is therefore always the same as its first parameter type. The accumulator must be a storable
 * element type, and the function must be copyable and non-throwing.
 *
 * The accumulator type is the function's first parameter type when its parameters can be inspected. Because the first
 * form has no initial value to supply that type, its function must be inspectable: a generic callable whose parameter
 * types cannot be inspected must use the form with an explicit initial value, where the initial value's type becomes
 * the accumulator type.
 *
 * `reduce` visits `count(source)` elements, capped at `CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE`, so an
 * unbounded producer is folded over its observable traversal cap and every call terminates. It never stops early. Over
 * an empty source, the form without an initial value returns the accumulator's \ref DefaultElement "default value";
 * the form with an initial value returns that initial value. `Map` and `Set` traversal order is not semantically
 * significant, so a fold over those sources depends on the function's own commutativity. Every source value is
 * preserved.
 *
 * Without an initial value, the first source element must convert safely to the accumulator type. In either form, each
 * callback parameter must safely accept its corresponding argument; when an initial value is supplied, it must also
 * convert safely to the accumulator type. Safe conversion means an identical type or a value-preserving conversion
 * such as `int` to `long`. A narrowing conversion such as `long` to `int`, and any conversion that crosses the
 * Boolean, character, enumeration, pointer, or user-defined domains, is rejected at compile time. Standard-library
 * ranges and containers, functions that can throw, and accumulators that cannot be stored are rejected at compile
 * time.
 *
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;

 struct Summary {
   int items{};
   int sum{};
 };

 int main() {
   // Compile-time demonstration.
   constexpr auto add = [](int lhs, int rhs) noexcept { return lhs + rhs; };
   constexpr auto v = vector(1, 2, 3);
   static_assert(reduce(add, v) == 6);      // 1 + 2 + 3
   static_assert(reduce(add, 10, v) == 16); // 10 + 1 + 2 + 3
   static_assert(reduce(add, 0, vector<int, 0>()) ==
                 0); // empty explicit-initial form
   static_assert(reduce(add, vector<int, 0>()) == 0); // default accumulator

   constexpr auto summarize = [](Summary accumulator, int value) noexcept {
     return Summary{accumulator.items + 1, accumulator.sum + value};
   };
   constexpr auto summary = reduce(summarize, Summary{}, v);
   static_assert(summary.items == 3 && summary.sum == 6);

   // A wider accumulator: each element converts safely from int to long.
   constexpr auto add_long = [](long lhs, long rhs) noexcept {
     return lhs + rhs;
   };
   static_assert(reduce(add_long, v) == 6L);

   // An explicit initial value can select an accumulator unlike the element
   // type.
   constexpr auto count_positive = [](int accumulator, long value) noexcept {
     return accumulator + (value > 0L ? 1 : 0);
   };
   static_assert(reduce(count_positive, 0, vector(10L, 20L)) == 2);

   // Producers are folded over their observable elements.
   static_assert(reduce(add, Range{1, 4, 1}) == 6); // produces 1, 2, 3

   // Map and Set traversal order is not semantically significant.
   constexpr auto s = hash_set(1, 2, 3);
   static_assert(reduce(add, 0, s) == 6);

   // Runtime demonstration.
   const auto runtime_base = 1;
   const auto runtime_v =
       vector(runtime_base, runtime_base + 1, runtime_base + 2);
   const auto runtime_sum = reduce(add, runtime_v);
   const auto runtime_seeded = reduce(add, runtime_base * 10, runtime_v);

   return (runtime_sum == 6 && runtime_seeded == 16) ? 0 : 1;
 }
 ~~~~~
 */
template <typename F, typename Source>
    requires concepts_detail::ReduceCallable<F, Source>
[[nodiscard]] constexpr auto reduce(const F& function, const Source& source) noexcept
    -> concepts_detail::ReduceAccumulatorFromSignature<F>::type {
    using Accumulator = concepts_detail::ReduceAccumulatorFromSignature<F>::type;

    const auto observed = concepts_detail::reduce_observed_count(source);
    if (observed == 0U) {
        return Accumulator{};
    }
    auto iterator = std::ranges::begin(source);
    auto accumulator = static_cast<Accumulator>(*iterator);
    ++iterator;
    return concepts_detail::reduce_fold(function, accumulator, iterator, observed - 1U);
}

template <typename F, typename Initial, typename Source>
    requires concepts_detail::ReduceCallableWithInitial<F, Initial, Source>
[[nodiscard]] constexpr auto reduce(const F& function, const Initial& initial, const Source& source) noexcept
    -> concepts_detail::ReduceAccumulator<F, Initial>::type {
    using Accumulator = concepts_detail::ReduceAccumulator<F, Initial>::type;

    return concepts_detail::reduce_fold(function, static_cast<Accumulator>(initial), std::ranges::begin(source),
                                        concepts_detail::reduce_observed_count(source));
}

// Diagnostic fallback (REQ-DIAG-009): a call outside the admitted domain names
// the domain instead of listing failed concept candidates. It produces no value.
template <typename F, typename Source>
    requires(!concepts_detail::ReduceCallable<F, Source>)
constexpr auto reduce([[maybe_unused]] const F& function, [[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(
        concepts_detail::dependent_false<F, Source>,
        "cljonic::reduce: expected a reducing function followed by one cljonic source, or a reducing function, an "
        "initial value, and one cljonic source. The source must be a Vector, Map, Set, Queue, or String collection, "
        "or a Range, Repeat, Cycle, Iterate, or Repeatedly producer. The function must be copy-constructible, invoked "
        "without throwing as (accumulator, element), and return the accumulator type. The form without an initial "
        "value requires a function whose parameter types can be inspected, because its first parameter type is the "
        "accumulator type; use the form with an initial value for a generic callable. The accumulator must be a "
        "storable element type. Each function parameter and the initial value must convert safely: an identical type "
        "or a value-preserving conversion, never a narrowing conversion and never a cross into or out of the Boolean, "
        "character, enumeration, pointer, or user-defined domains. Standard-library ranges and containers are not "
        "supported.");
}

template <typename F, typename Initial, typename Source>
    requires(!concepts_detail::ReduceCallableWithInitial<F, Initial, Source>)
constexpr auto reduce([[maybe_unused]] const F& function, [[maybe_unused]] const Initial& initial,
                      [[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(
        concepts_detail::dependent_false<F, Initial, Source>,
        "cljonic::reduce: expected a reducing function, an initial value, and one cljonic source. The source must be "
        "a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer. "
        "The function must be copy-constructible, invoked without throwing as (accumulator, element), and return the "
        "accumulator type. The initial value must convert safely to the accumulator type: an identical type or a "
        "value-preserving conversion, never a narrowing conversion and never a cross into or out of the Boolean, "
        "character, enumeration, pointer, or user-defined domains. The accumulator must be a storable element type. "
        "Standard-library ranges and containers are not supported.");
}

} // namespace cljonic
