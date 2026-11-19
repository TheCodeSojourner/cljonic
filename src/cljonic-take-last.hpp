#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>

#include <cljonic-collection-vector.hpp>
#include <cljonic-concepts.hpp>

namespace cljonic::concepts_detail {

template <typename Source, typename Window>
constexpr auto copy_take_last_window(const Source& source, std::size_t observed_count, Window& window) noexcept
    -> void {
    auto iterator = source.begin();
    const auto end = source.end();
    for (std::size_t index = 0; iterator != end && index < observed_count; ++index) {
        window = window.conj(*iterator);
        ++iterator;
    }
}

template <typename Window, typename Result>
constexpr auto copy_take_last_suffix(const Window& window, std::size_t requested_count, Result& result) noexcept
    -> void {
    const auto retained_count = std::min(requested_count, window.count());
    const auto start = window.count() - retained_count;
    for (std::size_t index = start; index < window.count(); ++index) {
        result = result.conj(window(index));
    }
}

} // namespace cljonic::concepts_detail

namespace cljonic::core {

/** \anchor TakeLast
 * The \b take_last free function returns up to count elements from the end of one cljonic source's logical traversal
 * window as a bounded owning \ref Vector "Vector". Use \c take_last(count, source) with a \ref Vector "Vector", \ref
 * Map "Map", \ref Set "Set", \ref Queue "Queue", or \ref String "String" collection, or a \ref Range "Range", \ref
 * Repeat "Repeat", \ref Cycle "Cycle", \ref Iterate "Iterate", \ref Repeatedly "Repeatedly", \ref TakeWhile
 * "TakeWhile", or \ref DropWhile "DropWhile" producer.
 *
 * Counts follow the \ref implicit-conversions "safe count conversion rules": signed or unsigned integers whose
 * nonnegative ranges fit std::size_t, excluding Boolean and character types. Zero or negative counts return an empty
 * typed vector. The result has the source element type and the source-bound result capacity, independent of the
 * runtime count.
 *
 * The result contains the trailing portion of the original bounded observation window, as produced by iterating the
 * source. An oversized count returns the complete window. For \ref Map "Map" and \ref Set "Set" sources, logical
 * traversal order is semantically unordered. Empty and zero-capacity sources return empty vectors without traversal or
 * callbacks. The source and callback state are preserved according to existing cljonic traversal rules.
 *
 \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   constexpr auto values = vector(1, 2, 3, 4);
   constexpr auto suffix = take_last(2, values);
   static_assert(equal(suffix, vector(3, 4)));
   static_assert(is_empty(take_last(0, values)));
   static_assert(equal(take_last(100, values), values));
   static_assert(equal(values, vector(1, 2, 3, 4)));

   const auto requested = 2;
   const auto source = queue(10, 20, 30, 40);
   const auto runtime_suffix = take_last(requested, source);
   const auto generated = take_last(requested, range(1, 5));
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
[[nodiscard]] constexpr auto take_last(Count count, const Source& source) noexcept
    -> Vector<std::ranges::range_value_t<const Source>, concepts_detail::SourceResultCapacity<Source>> {
    constexpr auto capacity = concepts_detail::SourceResultCapacity<Source>;
    using Element = std::ranges::range_value_t<const Source>;
    const auto requested_count = concepts_detail::normalize_producer_count(count);
    Vector<Element, capacity> window{};
    if (requested_count == 0 || capacity == 0) {
        return {};
    }

    concepts_detail::copy_take_last_window(source, std::min<std::size_t>(source.count(), capacity), window);
    Vector<Element, capacity> result{};
    concepts_detail::copy_take_last_suffix(window, requested_count, result);
    return result;
}

template <typename Count, typename Source>
    requires(!concepts_detail::CountedSourceArguments<Count, Source>)
constexpr auto take_last([[maybe_unused]] Count count, [[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(
        concepts_detail::dependent_false<Count, Source>,
        "cljonic::take_last: expected a non-boolean, non-character integer count whose nonnegative range fits "
        "std::size_t, followed by exactly one cljonic source: a Vector, Map, Set, Queue, or String "
        "collection, or a Range, Repeat, Cycle, Iterate, Repeatedly, TakeWhile, or DropWhile producer. "
        "MapEntry inputs and external C++ ranges are not supported.");
}

template <typename... Args>
    requires(sizeof...(Args) != 2)
constexpr auto take_last([[maybe_unused]] const Args&... args) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Args...>,
                  "cljonic::take_last: expected exactly two arguments: a non-boolean, non-character integer count "
                  "whose nonnegative range fits std::size_t and one cljonic source: a Vector, Map, Set, Queue, or "
                  "String collection, or a Range, Repeat, Cycle, Iterate, Repeatedly, TakeWhile, or DropWhile "
                  "producer.");
}

} // namespace cljonic::core
