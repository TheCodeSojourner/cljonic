#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>

#include <cljonic-collection-vector.hpp>
#include <cljonic-concepts.hpp>
#include <cljonic-core-collection-maximum-element-count.hpp>

namespace cljonic {

namespace concepts_detail {

template <typename Count, typename Source>
concept TakeArguments = ProducerCount<Count> && concepts::CljonicSource<Source> &&
                        concepts::NothrowCollectionElement<std::ranges::range_value_t<const Source>>;

template <typename Source>
inline constexpr std::size_t TakeResultCapacity = [] constexpr {
    if constexpr (concepts::CljonicCollection<Source>) {
        return std::min<std::size_t>(Source::capacity(), CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE);
    } else {
        return CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE;
    }
}();

} // namespace concepts_detail

/** \anchor Take
 * The \b take free function returns the first requested elements of one cljonic source as a bounded owning \ref Vector
 * "Vector". Use \c take(count, source) with a \ref Vector "Vector", \ref Map "Map", \ref Set "Set", \ref Queue
 * "Queue", or \ref String "String" collection, or a \ref Range "Range", \ref Repeat "Repeat", \ref Cycle "Cycle", \ref
 * Iterate "Iterate", or \ref Repeatedly "Repeatedly" producer.
 *
 * Counts follow the \ref implicit-conversions "safe count conversion rules": signed or unsigned integers whose
 * nonnegative ranges fit std::size_t, excluding Boolean and character types. Zero or negative counts return an empty
 * vector. A request larger than the source's observable count returns all observable elements. The result has the
 * source element type and its declared collection capacity, or the configured observation cap for a producer,
 * saturated at CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE. Capacity does not depend on the runtime count.
 *
 * The result follows source traversal order; Map and Set order is not semantically significant. Queue traversal is
 * FIFO, String yields characters, and Map yields MapEntry values. Empty requests do not start traversal, and no
 * producer is advanced past the last returned element. The source value is preserved. Repeatedly callback state held
 * by value is preserved; referenced or static state remains shared. MapEntry inputs and external C++ ranges are not
 * supported. The operation is constexpr, non-throwing, and non-allocating.
 *
 \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;

 int main() {
   constexpr auto values = vector(1, 2, 3, 4);
   constexpr auto prefix = take(2, values);
   static_assert(equal(prefix, vector(1, 2)));
   static_assert(equal(take(10, values), values));
   static_assert(is_empty(take(-1, values)));
   static_assert(equal(values, vector(1, 2, 3, 4)));

   const auto requested = 3;
   const auto source = queue(10, 20, 30, 40);
   const auto runtime_prefix = take(requested, source);
   const auto generated =
       take(requested, iterate([](int value) noexcept { return value + 1; }, 5));
   return equal(runtime_prefix, vector(10, 20, 30)) &&
                  equal(generated, vector(5, 6, 7)) &&
                  equal(source, queue(10, 20, 30, 40))
              ? 0
              : 1;
 }
 ~~~~~
 */
template <typename Count, typename Source>
    requires concepts_detail::TakeArguments<Count, Source>
[[nodiscard]] constexpr auto take(Count count, const Source& source) noexcept
    -> Vector<std::ranges::range_value_t<const Source>, concepts_detail::TakeResultCapacity<Source>> {
    constexpr auto capacity = concepts_detail::TakeResultCapacity<Source>;
    const auto observed_count = std::min({concepts_detail::normalize_producer_count(count), source.count(), capacity});
    Vector<std::ranges::range_value_t<const Source>, capacity> result{};
    if (observed_count == 0) {
        return result;
    }
    auto iterator = source.begin();
    const auto end = source.end();
    for (std::size_t index = 0; iterator != end; ++index) {
        result = result.conj(*iterator);
        if (index + 1 == observed_count) {
            break;
        }
        ++iterator;
    }
    return result;
}

template <typename Count, typename Source>
    requires(!concepts_detail::TakeArguments<Count, Source>)
constexpr auto take([[maybe_unused]] Count count, [[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Count, Source>,
                  "cljonic::take: expected a non-boolean, non-character integer count whose nonnegative range fits "
                  "std::size_t, followed by exactly one cljonic source: a Vector, Map, Set, Queue, or String "
                  "collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer. MapEntry inputs and "
                  "external C++ ranges are not supported.");
}

template <typename... Args>
    requires(sizeof...(Args) != 2)
constexpr auto take([[maybe_unused]] const Args&... args) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Args...>,
                  "cljonic::take: expected exactly two arguments: a non-boolean, non-character integer count whose "
                  "nonnegative range fits std::size_t and one cljonic source: a Vector, Map, Set, Queue, or String "
                  "collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer.");
}

} // namespace cljonic
