#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>

#include <cljonic-collection-vector.hpp>
#include <cljonic-concepts.hpp>

namespace cljonic::concepts_detail {

template <typename Source, typename Result>
constexpr auto copy_take_nth_stride(const Source& source, std::size_t stride, Result& result) noexcept -> void {
    auto iterator = source.begin();
    const auto end = source.end();
    const auto observed_count = std::min<std::size_t>(source.count(), Result::capacity());
    for (std::size_t index = 0; iterator != end && index < observed_count; ++index) {
        if (index % stride == 0) {
            result = result.conj(*iterator);
        }
        ++iterator;
    }
}

} // namespace cljonic::concepts_detail

namespace cljonic::core {

/** \anchor TakeNth
 * The \b take_nth free function returns elements at every requested zero-based stride from one cljonic source as a
 * bounded owning \ref Vector "Vector". Use \c take_nth(n, source) with a \ref Vector "Vector", \ref Map "Map", \ref
 * Set "Set", \ref Queue "Queue", or \ref String "String" collection, or a supported cljonic producer.
 *
 * The result begins with the first observed element and then includes offsets \c n, \c 2n, and so on within the
 * source's original bounded observation window. A stride of one returns the complete window. Zero or negative strides
 * return an empty typed vector. \ref Map "Map" and \ref Set "Set" traversal order is semantically unordered.
 *
 \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   constexpr auto values = vector(1, 2, 3, 4, 5);
   constexpr auto selected = take_nth(2, values);
   static_assert(equal(selected, vector(1, 3, 5)));
   static_assert(equal(take_nth(1, values), values));
   static_assert(is_empty(take_nth(0, values)));

   const auto source = queue(10, 20, 30, 40);
   return equal(take_nth(2, source), vector(10, 30)) &&
                  equal(source, queue(10, 20, 30, 40))
              ? 0
              : 1;
 }
 ~~~~~
 */
template <typename Count, typename Source>
    requires concepts_detail::CountedSourceArguments<Count, Source>
[[nodiscard]] constexpr auto take_nth(Count stride, const Source& source) noexcept
    -> Vector<std::ranges::range_value_t<const Source>, concepts_detail::SourceResultCapacity<Source>> {
    constexpr auto capacity = concepts_detail::SourceResultCapacity<Source>;
    using Element = std::ranges::range_value_t<const Source>;
    const auto normalized_stride = concepts_detail::normalize_producer_count(stride);
    Vector<Element, capacity> result{};
    if (normalized_stride == 0 || capacity == 0) {
        return result;
    }

    concepts_detail::copy_take_nth_stride(source, normalized_stride, result);
    return result;
}

template <typename Count, typename Source>
    requires(!concepts_detail::CountedSourceArguments<Count, Source>)
constexpr auto take_nth([[maybe_unused]] Count stride, [[maybe_unused]] const Source& source) noexcept -> void {
    static_assert(
        concepts_detail::dependent_false<Count, Source>,
        "cljonic::take_nth: expected a non-boolean, non-character integer stride whose nonnegative range fits "
        "std::size_t, followed by exactly one cljonic source: a Vector, Map, Set, Queue, or String "
        "collection, or a Range, Repeat, Cycle, Iterate, Repeatedly, TakeWhile, or DropWhile producer. "
        "MapEntry inputs and external C++ ranges are not supported.");
}

template <typename... Args>
    requires(sizeof...(Args) != 2)
constexpr auto take_nth([[maybe_unused]] const Args&... args) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Args...>,
                  "cljonic::take_nth: expected exactly two arguments: a non-boolean, non-character integer stride "
                  "whose nonnegative range fits std::size_t and one cljonic source: a Vector, Map, Set, Queue, or "
                  "String collection, or a Range, Repeat, Cycle, Iterate, Repeatedly, TakeWhile, or DropWhile "
                  "producer.");
}

} // namespace cljonic::core
