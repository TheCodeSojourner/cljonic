#pragma once

#include <algorithm>
#include <tuple>
#include <utility>

#include <cljonic-concepts.hpp>
#include <cljonic-core-collection-maximum-element-count.hpp>

namespace cljonic {

namespace concepts_detail {
// Static parameter-type extraction for the callables `map` can inspect: a
// function pointer, or a class type with one non-template operator(). A generic
// lambda or a templated operator() is not inspectable and reports known=false.
template <typename P>
struct CallableSignatureParameters {
    static constexpr bool known = false;
    using type = std::tuple<>;
};

template <typename R, typename C, typename... Args>
struct CallableSignatureParameters<R (C::*)(Args...) const noexcept> {
    static constexpr bool known = true;
    using type = std::tuple<Args...>;
};

template <typename R, typename C, typename... Args>
struct CallableSignatureParameters<R (C::*)(Args...) const> {
    static constexpr bool known = true;
    using type = std::tuple<Args...>;
};

template <typename R, typename C, typename... Args>
struct CallableSignatureParameters<R (C::*)(Args...) noexcept> {
    static constexpr bool known = true;
    using type = std::tuple<Args...>;
};

template <typename R, typename C, typename... Args>
struct CallableSignatureParameters<R (C::*)(Args...)> {
    static constexpr bool known = true;
    using type = std::tuple<Args...>;
};

template <typename R, typename... Args>
struct CallableSignatureParameters<R (*)(Args...) noexcept> {
    static constexpr bool known = true;
    using type = std::tuple<Args...>;
};

template <typename R, typename... Args>
struct CallableSignatureParameters<R (*)(Args...)> {
    static constexpr bool known = true;
    using type = std::tuple<Args...>;
};

template <typename F>
struct CallableParameters : CallableSignatureParameters<F> {};

template <typename F>
    requires requires { &F::operator(); }
struct CallableParameters<F> : CallableSignatureParameters<decltype(&F::operator())> {};

// Per-position parameter rule (REQ-FN-032): when the callback's parameter types
// are statically known, each parameter must accept its corresponding source
// element through a safe implicit conversion. Narrowing conversions and
// Boolean/character/enumeration/pointer/user-defined crossings are rejected. A
// callback with fewer parameters than sources is rejected; extra parameters may
// take defaults.
template <typename Params, typename IndexSequence, typename... Sources>
struct MapParametersSafe;

template <typename Params, std::size_t... Index, typename... Sources>
struct MapParametersSafe<Params, std::index_sequence<Index...>, Sources...>
    : std::bool_constant<(concepts::SafeImplicitConversion<std::tuple_element_t<Index, Params>,
                                                           std::ranges::range_value_t<const Sources>> &&
                          ...)> {};

template <typename F, typename... Sources>
inline constexpr bool map_parameters_safe_v = [] constexpr {
    using Parameters = CallableParameters<std::decay_t<F>>::type;
    if constexpr (!CallableParameters<std::decay_t<F>>::known) {
        return true;
    } else if constexpr (std::tuple_size_v<Parameters> < sizeof...(Sources)) {
        return false;
    } else {
        return MapParametersSafe<Parameters, std::index_sequence_for<Sources...>, Sources...>::value;
    }
}();

// Guarded admission for the multi-source `map` free function (REQ-FN-032):
// every source must be a cljonic source, the transforming function must be
// copy-constructible and invoked without throwing with one argument per
// source, its result must be a nothrow-storable element, and -- when its
// parameter types are inspectable -- every parameter must safely accept the
// corresponding source element.
template <typename F, typename... Sources>
concept MapCallable =
    (sizeof...(Sources) >= 1) && (concepts::CljonicSource<Sources> && ...) &&
    std::copy_constructible<std::decay_t<F>> && std::is_nothrow_copy_constructible_v<std::decay_t<F>> &&
    std::is_nothrow_invocable_v<const std::decay_t<F>&, std::ranges::range_reference_t<const Sources>...> &&
    concepts::NothrowCollectionElement<
        std::invoke_result_t<const std::decay_t<F>&, std::ranges::range_reference_t<const Sources>...>> &&
    map_parameters_safe_v<F, Sources...>;

template <typename F, typename... Sources>
using MapResultType = std::invoke_result_t<const std::decay_t<F>&, std::ranges::range_reference_t<const Sources>...>;

// Compile-time observable-count bound per source: a collection contributes its
// declared capacity; any producer contributes the configured observable
// traversal cap (REQ-FN-032 result-capacity policy).
template <typename Source>
[[nodiscard]] constexpr auto map_source_bound() noexcept -> std::size_t {
    if constexpr (concepts::CljonicCollection<Source>) {
        return Source::capacity();
    } else {
        return CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    }
}

template <typename... Sources>
inline constexpr std::size_t MapResultCapacity = [] constexpr {
    std::size_t minimum = CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    ((minimum = std::min(minimum, map_source_bound<Sources>())), ...);
    return minimum;
}();

} // namespace concepts_detail

/** \anchor MapFunction
 * The \b map free function applies its transforming function to corresponding elements of one or more cljonic sources.
 * The function is the first argument, followed by one or more sources: a \ref Vector "Vector", \ref Map "Map", \ref
 * Set "Set", \ref Queue "Queue", or \ref String "String" collection, or a \ref Range "Range", \ref Repeat "Repeat",
 * \ref Cycle "Cycle", \ref Iterate "Iterate", or \ref Repeatedly "Repeatedly" producer. A unary function is applied
 * over one source, a binary function over two sources, and so on; traversal terminates when the first source is
 * exhausted. Map and Set traversal order is not semantically significant, so the mapped result order carries no
 * semantic significance for those sources.
 *
 * The result is a bounded owning \ref Vector "Vector" of the function results. Its capacity is the compile-time
 * minimum of the sources' observable-count bounds: a collection contributes its declared capacity and any producer
 * contributes the configured observable traversal cap, every contribution saturated at
 * CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE. The runtime number of mapped elements is the minimum of the sources'
 * effective counts: a collection contributes its element count, a finite producer its count, and an unbounded producer
 * its configured observable traversal cap. Every source value is preserved.
 *
 * The transforming function must be copy-constructible, invoked without throwing with one argument per source, and
 * return a storable element result. Each parameter must accept the corresponding source element through a safe
 * implicit conversion: an identical type, or a value-preserving arithmetic conversion such as `int` to `long`. A
 * narrowing conversion such as `long` to `int`, and any conversion that crosses the Boolean, character, enumeration,
 * pointer, or user-defined domains, is rejected at compile time. A callback whose parameter types are not statically
 * inspectable -- a generic lambda, or a class with a templated `operator()` -- is admitted on nothrow invocation
 * alone. Standard-library ranges and containers, functions that can throw, and results that cannot be stored are
 * rejected at compile time.
 *
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;

 int main() {
   // Compile-time demonstration.
   constexpr auto inc = [](int value) noexcept { return value + 1; };
   constexpr auto add = [](int lhs, int rhs) noexcept { return lhs + rhs; };
   constexpr auto v = vector(1, 2, 3);
   constexpr auto singles = map(inc, v);
   static_assert(equal(singles, vector(2, 3, 4)));

   constexpr auto r = Range{1, 4, 1}; // produces 1, 2, 3
   constexpr auto q = queue(10, 20, 30, 40);
   constexpr auto pairs = map(add, v, q); // terminates at the shorter source
   static_assert(equal(pairs, vector(11, 22, 33)));

   constexpr auto triple = [](int a, int b, int c) noexcept {
     return a + b + c;
   };
   constexpr auto triples = map(triple, v, r, repeat(2, 100));
   static_assert(equal(triples, vector(102, 104)));

   constexpr auto over_producer = map(inc, repeat(3, 7));
   static_assert(equal(over_producer, vector(8, 8, 8)));

   constexpr auto entry_value = [](const auto &entry) noexcept {
     return entry.value;
   };
   constexpr auto over_map = map(entry_value, hash_map(1, 10, 2, 20));
   static_assert(equal(over_map, vector(10, 20)) ||
                 equal(over_map, vector(20, 10))); // Map order is unordered

   constexpr auto next_char = [](char c) noexcept {
     return static_cast<char>(c + 1);
   };
   constexpr auto over_string = map(next_char, string<4>("abc"));
   static_assert(equal(over_string, vector('b', 'c', 'd')));

   // Runtime demonstration.
   const auto runtime_base = 1;
   const auto runtime_v =
       vector(runtime_base, runtime_base + 1, runtime_base + 2);
   const auto runtime_q = queue(runtime_base * 10, (runtime_base + 1) * 10,
                                (runtime_base + 2) * 10);
   const auto runtime_pairs = map(add, runtime_v, runtime_q);

   return (count(runtime_pairs) == 3 && runtime_pairs(0) == 11 &&
           runtime_pairs(1) == 22 && runtime_pairs(2) == 33)
              ? 0
              : 1;
 }
 ~~~~~
 */
template <typename F, typename... Sources>
    requires concepts_detail::MapCallable<F, Sources...>
[[nodiscard]] constexpr auto map(const F& function, const Sources&... sources) noexcept
    -> Vector<concepts_detail::MapResultType<F, Sources...>, concepts_detail::MapResultCapacity<Sources...>> {
    constexpr auto capacity = concepts_detail::MapResultCapacity<Sources...>;
    using Result = concepts_detail::MapResultType<F, Sources...>;

    const std::size_t mapped_count =
        std::min({static_cast<std::size_t>(sources.count())..., static_cast<std::size_t>(capacity)});

    auto iterators = std::tuple{sources.begin()...};
    Vector<Result, capacity> result{};
    for (std::size_t index = 0U; index < mapped_count; ++index) {
        const auto value =
            std::apply([&function](const auto&... iterator) { return std::invoke(function, *iterator...); }, iterators);
        result = result.conj(value);
        std::apply([](auto&... iterator) { (++iterator, ...); }, iterators);
    }
    return result;
}

// Diagnostic fallback (REQ-DIAG-009): a call outside the admitted domain names
// the domain instead of listing failed concept candidates. It produces no value.
template <typename F, typename... Sources>
    requires(!concepts_detail::MapCallable<F, Sources...>)
constexpr auto map([[maybe_unused]] const F& function, [[maybe_unused]] const Sources&... sources) noexcept -> void {
    static_assert(
        concepts_detail::dependent_false<F, Sources...>,
        "cljonic::map: expected a transforming function followed by one or more compatible cljonic sources. Each "
        "source must be a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, Cycle, Iterate, or "
        "Repeatedly producer. The function must be copy-constructible, invoked without throwing with one argument "
        "per source, and return a storable element result. Each function parameter must accept the corresponding "
        "source element through a safe implicit conversion: an identical type or a value-preserving arithmetic "
        "conversion, never a narrowing conversion and never a cross into or out of the Boolean, character, "
        "enumeration, pointer, or user-defined domains. At least one source is required. Standard-library ranges "
        "and containers are not supported.");
}

} // namespace cljonic
