#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <tuple>
#include <type_traits>

#include <cljonic-collection-vector.hpp>
#include <cljonic-concepts.hpp>
#include <cljonic-core-collection-maximum-element-count.hpp>

namespace cljonic {

namespace concepts_detail {

template <typename Params, typename Source>
struct FilterParametersSafe : std::false_type {};

template <typename Params, typename Source>
    requires(std::tuple_size_v<Params> >= 1)
struct FilterParametersSafe<Params, Source>
    : std::bool_constant<
          concepts::SafeImplicitConversion<std::tuple_element_t<0, Params>, std::ranges::range_value_t<const Source>>> {
};

template <typename F, typename Source>
inline constexpr bool filter_parameters_safe_v = [] constexpr {
    if constexpr (!CallableParameters<std::decay_t<F>>::known) {
        return true;
    } else {
        return FilterParametersSafe<CallableParameterTypes<F>, Source>::value;
    }
}();

template <typename F, typename Source, typename = void>
struct FilterPredicateReturnsBool : std::false_type {};

template <typename F, typename Source>
struct FilterPredicateReturnsBool<
    F, Source, std::void_t<std::invoke_result_t<const std::decay_t<F>&, std::ranges::range_reference_t<const Source>>>>
    : std::bool_constant<std::same_as<
          std::invoke_result_t<const std::decay_t<F>&, std::ranges::range_reference_t<const Source>>, bool>> {};

template <typename F, typename Source>
concept FilterCallable =
    concepts::CljonicSource<Source> && concepts::NothrowCollectionElement<std::ranges::range_value_t<const Source>> &&
    std::copy_constructible<std::decay_t<F>> &&
    std::is_nothrow_invocable_v<const std::decay_t<F>&, std::ranges::range_reference_t<const Source>> &&
    FilterPredicateReturnsBool<F, Source>::value && filter_parameters_safe_v<F, Source>;

template <typename Source>
[[nodiscard]] constexpr auto filter_source_bound() noexcept -> std::size_t {
    if constexpr (concepts::CljonicCollection<Source>) {
        return std::min<std::size_t>(Source::capacity(), CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
    } else {
        return CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    }
}

template <typename Source>
inline constexpr std::size_t FilterResultCapacity = filter_source_bound<Source>();

template <typename F, typename Element, typename Result>
constexpr void filter_append_if_matching(const F& predicate, const Element& element, Result& result) noexcept {
    if (predicate(element)) {
        result = result.conj(element);
    }
}

} // namespace concepts_detail

/** \anchor FilterFunction
 * The \b filter free function copies matching elements from one cljonic source into a bounded owning \ref Vector
 * "Vector". The source may be a \ref Vector "Vector", \ref Map "Map", \ref Set "Set", \ref Queue "Queue", or \ref
 * String "String" collection, or a \ref Range "Range", \ref Repeat "Repeat", \ref Cycle "Cycle", \ref Iterate
 * "Iterate", or \ref Repeatedly "Repeatedly" producer. Its result element type is the source element type. Its
 * capacity is the source's declared collection capacity, or the configured observation cap for a producer, saturated
 * at CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE. Because that capacity equals the source's observable-count bound,
 * every observed element fits; traversal ends when the source is exhausted. Map and Set traversal order is not
 * semantically significant.
 *
 * The predicate should be a pure function (i.e., with no side effects) returning a bool. It must be
 * copy-constructible, callable without throwing on each const source element, and return exactly bool. When its
 * parameter type is statically inspectable, it must accept the source element through a safe implicit conversion: an
 * identical type or a value-preserving arithmetic conversion. Narrowing conversions and conversions into or out of the
 * Boolean, character, enumeration, pointer, or user-defined domains are rejected. Generic callables are admitted when
 * their invocation is non-throwing and returns exactly bool. Standard-library ranges and containers are not supported.
 *
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;

int main() {
  constexpr auto is_even = [](int value) noexcept { return value % 2 == 0; };
  constexpr auto values = vector(1, 2, 3, 4);
  constexpr auto evens = filter(is_even, values);
  static_assert(equal(evens, vector(2, 4)));
  constexpr auto entry_is_even = [](const auto &entry) noexcept {
    return entry.value % 2 == 0;
  };
  static_assert(count(filter(is_even, hash_set(1, 2, 3))) == 1);
  static_assert(count(filter(entry_is_even, hash_map(1, 2, 2, 4))) == 2);
  static_assert(equal(filter(is_even, queue(1, 2, 3)), vector(2)));
  static_assert(count(filter([](char value) noexcept { return value == 'b'; },
                             string<4>("abc"))) == 1);
  static_assert(equal(filter(is_even, Range<int>{1, 5, 1}), vector(2, 4)));
  static_assert(count(filter(is_even, repeat(3, 2))) == 3);
  static_assert(count(filter([](int) noexcept { return true; },
                             cycle(vector(1, 2)))) > 0);
  static_assert(
      count(filter(is_even, iterate([](int n) noexcept { return n + 1; }, 0))) >
      0);
  static_assert(
      count(filter(is_even, repeatedly(3, []() noexcept { return 2; }))) == 3);

  const auto base = 1;
  const auto runtime_values = vector(base, base + 1, base + 2, base + 3);
  const auto runtime_evens = filter(is_even, runtime_values);
  return count(runtime_evens) == 2 && runtime_evens(0) == 2 &&
                 runtime_evens(1) == 4
             ? 0
             : 1;
}
~~~~~
 */
template <typename F, typename Source>
    requires concepts_detail::FilterCallable<F, Source>
[[nodiscard]] constexpr auto filter(const F& predicate, const Source& source) noexcept
    -> Vector<std::ranges::range_value_t<const Source>, concepts_detail::FilterResultCapacity<Source>> {
    constexpr auto capacity = concepts_detail::FilterResultCapacity<Source>;
    using Element = std::ranges::range_value_t<const Source>;
    const auto observed_count = std::min<std::size_t>(static_cast<std::size_t>(source.count()), capacity);
    Vector<Element, capacity> result{};
    auto iterator = source.begin();
    const auto end = source.end();
    for (std::size_t index = 0U; index < observed_count && iterator != end; ++index) {
        const Element element = *iterator;
        concepts_detail::filter_append_if_matching(predicate, element, result);
        // Advance only while another observed element remains, so a producer's
        // step is never advanced past the last element it contributes.
        if (index + 1U < observed_count) {
            ++iterator;
        }
    }
    return result;
}

template <typename F, typename... Sources>
    requires(sizeof...(Sources) != 1)
constexpr auto filter([[maybe_unused]] const F& predicate, [[maybe_unused]] const Sources&... sources) noexcept
    -> void {
    static_assert(concepts_detail::dependent_false<F, Sources...>,
                  "cljonic::filter: expected a copy-constructible predicate and exactly one cljonic "
                  "source. The source must be a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, "
                  "Cycle, Iterate, or Repeatedly producer.");
}

template <typename F, typename Source>
    requires(!concepts_detail::FilterCallable<F, Source>)
constexpr auto filter([[maybe_unused]] const F& predicate, [[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(concepts_detail::dependent_false<F, Source>,
                  "cljonic::filter: expected a copy-constructible predicate and one cljonic source. "
                  "The source must be a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, Cycle, "
                  "Iterate, or Repeatedly producer. The predicate must be non-throwing on a const source element "
                  "and return exactly bool. An inspectable parameter must accept the source element through a safe "
                  "implicit conversion; standard-library ranges and containers are not supported.");
}

/** \anchor FiltervFunction
 * The \b filterv free function is an alias for \ref FilterFunction "filter". It accepts the same predicate and cljonic
 * source, has the same admission rules, and returns exactly the same bounded owning \ref Vector "Vector" type and
 * logical values.
 *
~~~~~{.cpp}
#include <concepts>

#include "cljonic.hpp"
using namespace cljonic;

int main() {
  constexpr auto is_even = [](int value) noexcept { return value % 2 == 0; };
  constexpr auto values = vector(1, 2, 3, 4);
  constexpr auto filtered = filterv(is_even, values);
  static_assert(equal(filtered, filter(is_even, values)));

  const auto base = 1;
  const auto runtime_values = vector(base, base + 1, base + 2, base + 3);
  const auto alias_result = filterv(is_even, runtime_values);
  const auto filter_result = filter(is_even, runtime_values);
  return equal(alias_result, filter_result) &&
                 equal(runtime_values,
                       vector(base, base + 1, base + 2, base + 3))
             ? 0
             : 1;
}
~~~~~
 */
template <typename F, typename Source>
    requires concepts_detail::FilterCallable<F, Source>
[[nodiscard]] constexpr auto filterv(const F& predicate, const Source& source) noexcept
    -> Vector<std::ranges::range_value_t<const Source>, concepts_detail::FilterResultCapacity<Source>> {
    return filter(predicate, source);
}

template <typename F, typename... Sources>
    requires(sizeof...(Sources) != 1)
constexpr auto filterv([[maybe_unused]] const F& predicate, [[maybe_unused]] const Sources&... sources) noexcept
    -> void {
    static_assert(concepts_detail::dependent_false<F, Sources...>,
                  "cljonic::filterv: expected a copy-constructible predicate and exactly one cljonic "
                  "source. The source must be a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, "
                  "Cycle, Iterate, or Repeatedly producer.");
}

template <typename F, typename Source>
    requires(!concepts_detail::FilterCallable<F, Source>)
constexpr auto filterv([[maybe_unused]] const F& predicate, [[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(concepts_detail::dependent_false<F, Source>,
                  "cljonic::filterv: expected a copy-constructible predicate and one cljonic source. "
                  "The source must be a Vector, Map, Set, Queue, or String collection, or a Range, Repeat, Cycle, "
                  "Iterate, or Repeatedly producer. The predicate must be non-throwing on a const source element "
                  "and return exactly bool. An inspectable parameter must accept the source element through a safe "
                  "implicit conversion; standard-library ranges and containers are not supported.");
}

} // namespace cljonic
