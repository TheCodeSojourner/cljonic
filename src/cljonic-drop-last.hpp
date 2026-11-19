#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>

#include <cljonic-collection-vector.hpp>
#include <cljonic-concepts.hpp>

namespace cljonic::concepts_detail {

template <typename Source, typename Window>
constexpr auto copy_drop_last_window(const Source& source, std::size_t observed_count, Window& window) noexcept
    -> void {
    auto iterator = source.begin();
    const auto end = source.end();
    for (std::size_t index = 0; iterator != end && index < observed_count; ++index) {
        window = window.conj(*iterator);
        ++iterator;
    }
}

template <typename Window, typename Result>
constexpr auto copy_drop_last_prefix(const Window& window, std::size_t requested_count, Result& result) noexcept
    -> void {
    const auto retained_count = requested_count >= window.count() ? 0 : window.count() - requested_count;
    for (std::size_t index = 0; index < retained_count; ++index) {
        result = result.conj(window(index));
    }
}

} // namespace cljonic::concepts_detail

namespace cljonic::core {

/** \anchor DropLast
 * The \b drop_last free function removes the requested final elements from one cljonic source and returns the
 * remaining prefix as a bounded owning \ref Vector "Vector". Use \c drop_last(count, source) with a \ref Vector
 * "Vector", \ref Map "Map", \ref Set "Set", \ref Queue "Queue", or \ref String "String" collection, or a \ref Range
 * "Range", \ref Repeat "Repeat", \ref Cycle "Cycle", \ref Iterate "Iterate", \ref Repeatedly "Repeatedly", \ref
 * TakeWhile "TakeWhile", or \ref DropWhile "DropWhile" producer.
 *
 * Counts follow the \ref implicit-conversions "safe count conversion rules": signed or unsigned integers whose
 * nonnegative ranges fit std::size_t, excluding Boolean and character types. Zero or negative counts retain the
 * complete original observation window. The result has the source element type and the source-bound result capacity,
 * independent of the runtime count.
 *
 * The result retains the prefix that remains after removing the normalized count from the end of the original bounded
 * observation window, as presented by source iteration. A count at least the observation-window bound returns an empty
 * typed vector without traversal or callbacks. Empty and zero-capacity sources also return empty vectors without
 * callbacks. For \ref Map "Map" and \ref Set "Set" sources, logical traversal order is semantically unordered.
 *
 * The source and callback state are preserved according to existing cljonic traversal rules.
 *
 \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   constexpr auto values = vector(1, 2, 3, 4);
   constexpr auto prefix = drop_last(2, values);
   static_assert(equal(prefix, vector(1, 2)));
   static_assert(equal(drop_last(0, values), values));
   static_assert(is_empty(drop_last(100, values)));
   static_assert(equal(values, vector(1, 2, 3, 4)));

   const auto requested = 2;
   const auto source = queue(10, 20, 30, 40);
   const auto runtime_prefix = drop_last(requested, source);
   const auto generated = drop_last(requested, range(1, 5));
   return equal(runtime_prefix, vector(10, 20)) &&
                  equal(generated, vector(1, 2)) &&
                  equal(source, queue(10, 20, 30, 40))
              ? 0
              : 1;
 }
 ~~~~~
 */
template <typename Count, typename Source>
    requires concepts_detail::CountedSourceArguments<Count, Source>
[[nodiscard]] constexpr auto drop_last(Count count, const Source& source) noexcept
    -> Vector<std::ranges::range_value_t<const Source>, concepts_detail::SourceResultCapacity<Source>> {
    constexpr auto capacity = concepts_detail::SourceResultCapacity<Source>;
    using Element = std::ranges::range_value_t<const Source>;
    const auto requested_count = concepts_detail::normalize_producer_count(count);
    Vector<Element, capacity> result{};
    if (capacity == 0) {
        return result;
    }

    const auto observed_count = std::min<std::size_t>(source.count(), capacity);
    if (requested_count >= observed_count) {
        return result;
    }

    Vector<Element, capacity> window{};
    concepts_detail::copy_drop_last_window(source, observed_count, window);
    concepts_detail::copy_drop_last_prefix(window, requested_count, result);
    return result;
}

template <typename Count, typename Source>
    requires(!concepts_detail::CountedSourceArguments<Count, Source>)
constexpr auto drop_last([[maybe_unused]] Count count, [[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(
        concepts_detail::dependent_false<Count, Source>,
        "cljonic::drop_last: expected a non-boolean, non-character integer count whose nonnegative range fits "
        "std::size_t, followed by exactly one cljonic source: a Vector, Map, Set, Queue, or String "
        "collection, or a Range, Repeat, Cycle, Iterate, Repeatedly, TakeWhile, or DropWhile producer. "
        "MapEntry inputs and external C++ ranges are not supported.");
}

template <typename... Args>
    requires(sizeof...(Args) != 2)
constexpr auto drop_last([[maybe_unused]] const Args&... args) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Args...>,
                  "cljonic::drop_last: expected exactly two arguments: a non-boolean, non-character integer count "
                  "whose nonnegative range fits std::size_t and one cljonic source: a Vector, Map, Set, Queue, or "
                  "String collection, or a Range, Repeat, Cycle, Iterate, Repeatedly, TakeWhile, or DropWhile "
                  "producer.");
}

} // namespace cljonic::core
