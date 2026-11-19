#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>

#include <cljonic-collection-vector.hpp>
#include <cljonic-concepts.hpp>

namespace cljonic::concepts_detail {

template <typename Source>
concept ButLastSourceArguments =
    concepts::CljonicSource<Source> && concepts::NothrowCollectionElement<std::ranges::range_value_t<const Source>>;

template <typename Source, typename Window>
constexpr auto copy_butlast_window(const Source& source, std::size_t observed_count, Window& window) noexcept -> void {
    auto iterator = source.begin();
    const auto end = source.end();
    for (std::size_t index = 0; iterator != end && index < observed_count; ++index) {
        window = window.conj(*iterator);
        ++iterator;
    }
}

template <typename Window, typename Result>
constexpr auto copy_butlast_prefix(const Window& window, Result& result) noexcept -> void {
    for (std::size_t index = 0; index + 1 < window.count(); ++index) {
        result = result.conj(window(index));
    }
}

} // namespace cljonic::concepts_detail

namespace cljonic::core {

/** \anchor ButLast
 * The \b butlast free function removes the final element of one cljonic source's bounded observation window and
 * returns all preceding elements as a bounded owning \ref Vector "Vector". Use \c butlast(source) with a \ref Vector
 * "Vector", \ref Map "Map", \ref Set "Set", \ref Queue "Queue", or \ref String "String" collection, or a \ref Range
 * "Range", \ref Repeat "Repeat", \ref Cycle "Cycle", \ref Iterate "Iterate", \ref Repeatedly "Repeatedly", \ref
 * TakeWhile "TakeWhile", or \ref DropWhile "DropWhile" producer.
 *
 * The result has the source element type and the source-bound result capacity. The original observation window is
 * bounded by source count, result capacity, and actual traversal exhaustion. Sources with fewer than two observable
 * elements and zero-capacity sources return empty typed vectors without callbacks.
 *
 * For \ref Map "Map" and \ref Set "Set" sources, logical traversal order is semantically unordered. Queue and String
 * order follow their collection traversal, and Map yields value-semantic MapEntry elements.
 *
 \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   constexpr auto values = vector(1, 2, 3, 4);
   constexpr auto prefix = butlast(values);
   static_assert(equal(prefix, vector(1, 2, 3)));
   static_assert(is_empty(butlast(vector<int, 1>(1))));
   static_assert(equal(values, vector(1, 2, 3, 4)));

   const auto source = queue(10, 20, 30);
   const auto result = butlast(source);
   return equal(result, vector(10, 20)) && equal(source, queue(10, 20, 30)) ? 0
                                                                            : 1;
 }
 ~~~~~
 */
template <typename Source>
    requires concepts_detail::ButLastSourceArguments<Source>
[[nodiscard]] constexpr auto butlast(const Source& source) noexcept
    -> Vector<std::ranges::range_value_t<const Source>, concepts_detail::SourceResultCapacity<Source>> {
    constexpr auto capacity = concepts_detail::SourceResultCapacity<Source>;
    using Element = std::ranges::range_value_t<const Source>;
    Vector<Element, capacity> result{};
    if (capacity == 0) {
        return result;
    }

    const auto observed_count = std::min<std::size_t>(source.count(), capacity);
    if (observed_count < 2) {
        return result;
    }

    Vector<Element, capacity> window{};
    concepts_detail::copy_butlast_window(source, observed_count, window);
    concepts_detail::copy_butlast_prefix(window, result);
    return result;
}

template <typename Source>
    requires(!concepts_detail::ButLastSourceArguments<Source>)
constexpr auto butlast([[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Source>,
                  "cljonic::butlast: expected exactly one cljonic source: a Vector, Map, Set, Queue, or String "
                  "collection, or a Range, Repeat, Cycle, Iterate, Repeatedly, TakeWhile, or DropWhile producer. "
                  "MapEntry inputs and external C++ ranges are not supported.");
}

template <typename... Args>
    requires(sizeof...(Args) != 1)
constexpr auto butlast([[maybe_unused]] const Args&... args) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Args...>,
                  "cljonic::butlast: expected exactly one argument: a Vector, Map, Set, Queue, or String "
                  "collection, or a Range, Repeat, Cycle, Iterate, Repeatedly, TakeWhile, or DropWhile producer.");
}

} // namespace cljonic::core
