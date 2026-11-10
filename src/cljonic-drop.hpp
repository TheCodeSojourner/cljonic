#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>

#include <cljonic-collection-vector.hpp>
#include <cljonic-concepts.hpp>

namespace cljonic::core {} // namespace cljonic::core

namespace cljonic::concepts_detail {

template <typename Source, typename Result>
constexpr auto copy_drop_suffix(const Source& source, std::size_t observed_count, Result& result,
                                std::size_t skipped_count) noexcept -> void {
    auto iterator = source.begin();
    const auto end = source.end();
    for (std::size_t index = 0; iterator != end; ++index) {
        if (index >= skipped_count) {
            result = result.conj(*iterator);
        }
        if (index + 1 == observed_count) {
            break;
        }
        std::ranges::advance(iterator, 1);
    }
}

} // namespace cljonic::concepts_detail

namespace cljonic::core {

/** \anchor Drop
 * The \b drop free function removes a requested prefix from one cljonic source and returns the remaining observable
 * elements as a bounded owning \ref Vector "Vector". Use \c drop(count, source) with a \ref Vector "Vector", \ref Map
 * "Map", \ref Set "Set", \ref Queue "Queue", or \ref String "String" collection, or a \ref Range "Range", \ref Repeat
 * "Repeat", \ref Cycle "Cycle", \ref Iterate "Iterate", \ref Repeatedly "Repeatedly", \ref TakeWhile "TakeWhile", or
 * \ref DropWhile "DropWhile" producer.
 *
 * Counts follow the \ref implicit-conversions "safe count conversion rules": signed or unsigned integers whose
 * nonnegative ranges fit std::size_t, excluding Boolean and character types. Zero or negative counts drop nothing. The
 * result has the source element type and declared collection capacity, or the configured observation cap for a
 * producer, saturated at CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE. Capacity does not depend on the runtime
 * count.
 *
 * Dropping consumes the original observation window; it does not start a fresh window for a producer. For example,
 * dropping three elements from an unbounded producer with observation cap 100 returns 97 elements, not 100. Requests
 * covering the original window's count bound return an empty vector without traversal or callbacks. Otherwise skipped
 * and retained values follow the producer's existing traversal callback timing, without advancing beyond the original
 * window. Empty sources invoke no callbacks, including Cycle over an empty source.
 *
 * The result follows source traversal order; Map and Set order is not semantically significant. Queue traversal is
 * FIFO, String yields characters, and Map yields MapEntry values. The source value is preserved. Repeatedly callback
 * state held by value is preserved; referenced or static state remains shared. MapEntry inputs and external C++ ranges
 * are not supported. The operation is constexpr, non-throwing, and non-allocating.
 *
 \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   constexpr auto values = vector(1, 2, 3, 4);
   constexpr auto suffix = drop(2, values);
   static_assert(equal(suffix, vector(3, 4)));
   static_assert(equal(drop(-1, values), values));
   static_assert(is_empty(drop(10, values)));
   static_assert(equal(values, vector(1, 2, 3, 4)));

   const auto requested = 2;
   const auto source = queue(10, 20, 30, 40);
   const auto runtime_suffix = drop(requested, source);
   const auto generated = drop(requested, range(1, 5));
   return equal(runtime_suffix, vector(30, 40)) &&
                  equal(generated, vector(3, 4)) &&
                  equal(source, queue(10, 20, 30, 40))
              ? 0
              : 1;
 }
 ~~~~~
 */
template <typename Count, typename Source>
    requires concepts_detail::CountedSourceArguments<Count, Source>
[[nodiscard]] constexpr auto drop(Count count, const Source& source) noexcept
    -> Vector<std::ranges::range_value_t<const Source>, concepts_detail::SourceResultCapacity<Source>> {
    constexpr auto capacity = concepts_detail::SourceResultCapacity<Source>;
    const auto observed_count = std::min<std::size_t>(source.count(), capacity);
    const auto skipped_count = concepts_detail::normalize_producer_count(count);
    Vector<std::ranges::range_value_t<const Source>, capacity> result{};
    if (skipped_count >= observed_count) {
        return result;
    }
    concepts_detail::copy_drop_suffix(source, observed_count, result, skipped_count);
    return result;
}

template <typename Count, typename Source>
    requires(!concepts_detail::CountedSourceArguments<Count, Source>)
constexpr auto drop([[maybe_unused]] Count count, [[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Count, Source>,
                  "cljonic::drop: expected a non-boolean, non-character integer count whose nonnegative range fits "
                  "std::size_t, followed by exactly one cljonic source: a Vector, Map, Set, Queue, or String "
                  "collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer. MapEntry inputs and "
                  "external C++ ranges are not supported.");
}

template <typename... Args>
    requires(sizeof...(Args) != 2)
constexpr auto drop([[maybe_unused]] const Args&... args) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Args...>,
                  "cljonic::drop: expected exactly two arguments: a non-boolean, non-character integer count whose "
                  "nonnegative range fits std::size_t and one cljonic source: a Vector, Map, Set, Queue, or String "
                  "collection, or a Range, Repeat, Cycle, Iterate, or Repeatedly producer.");
}

} // namespace cljonic::core
