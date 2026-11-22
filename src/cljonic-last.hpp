#pragma once

#include <cljonic-collection-map.hpp>
#include <cljonic-concepts.hpp>

namespace cljonic::core {

/** \anchor Last
 * The \b last free function returns the last logical element of a `Vector`, `Map`, `Set`, `Queue`, or `String` value
 * or any cljonic producer, or the value of a \ref MapEntryFactory "map_entry" value.
 *
 * `Vector` and `String` yield the element at the final logical index, `Queue` yields its final FIFO element without
 * removal, `Map` yields one value-semantic \ref MapEntryFactory "map_entry" value, and `Set` yields one stored
 * element. `Map` and `Set` traversal order is not semantically significant. An empty collection or producer
 * observation yields its element type's \ref DefaultElement "default value". A producer yielding map entries returns a
 * whole entry, not its value.
 *
 * Producer access evaluates only what is needed to reach the final observable element, without counting or
 * materializing the sequence first. A `drop_while` producer may examine its entire bounded matching prefix. Each call
 * starts a fresh traversal; source and predicate callbacks may run again. A zero observation limit invokes no
 * callbacks.
 *
 * \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   // Compile-time demonstration.
   constexpr auto v_const = vector(10, 20, 30);
   constexpr auto e_const = map_entry(1, 100);
   constexpr auto numbers = range(7, 10);
   constexpr auto suffix =
       drop_while([](int n) noexcept { return n < 3; }, vector(1, 2, 4));
   constexpr auto empty = repeat(0, 7);
   static_assert(last(v_const) == 30);
   static_assert(last(e_const) == 100);
   static_assert(last(numbers) == 9);
   static_assert(last(suffix) == 4);
   static_assert(last(cycle(suffix)) == 4);
   static_assert(last(empty) == 0);
   static_assert(count(v_const) == 3);

   // Runtime demonstration.
   const auto value = 100;
   const auto values = vector(value, 200);
   int calls = 0;
   const auto generated = repeatedly(4, [&calls, value]() noexcept {
     ++calls;
     return value;
   });

   return (last(values) == 200 && count(values) == 2 &&
           last(generated) == value && calls == 4 && last(empty) == 0)
              ? 0
              : 1;
 }
 ~~~~~
 */
template <typename KeyType, typename ValueType>
[[nodiscard]] constexpr auto last(const MapEntry<KeyType, ValueType>& entry) noexcept -> ValueType {
    return entry.value;
}

/** Returns the last logical element of a Vector, Map, Set, Queue, or String value. */
template <concepts::SequenceableCollection C>
[[nodiscard]] constexpr auto last(const C& collection) noexcept -> C::value_type {
    if (collection.count() == 0U) {
        return typename C::value_type{};
    }
    auto current = collection.begin();
    const auto end = collection.end();
    auto result = *current;
    ++current;
    while (current != end) {
        result = *current;
        ++current;
    }
    return result;
}

/** Returns the last observable producer element, or its default value when empty. */
template <concepts::CljonicProducer P>
[[nodiscard]] constexpr auto last(const P& producer) noexcept -> P::value_type {
    if constexpr (CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U) {
        return typename P::value_type{};
    } else {
        auto current = std::ranges::begin(producer);
        const auto end = std::ranges::end(producer);
        if (current == end) {
            return typename P::value_type{};
        }
        auto result = *current;
        ++current;
        while (current != end) {
            result = *current;
            ++current;
        }
        return result;
    }
}

template <typename C>
    requires(!concepts::SequenceableCollection<C> && !concepts::CljonicProducer<C> &&
             !concepts_detail::is_cljonic_map_entry_v<C>)
constexpr auto last([[maybe_unused]] const C& value) noexcept -> void {
    static_assert(concepts_detail::dependent_false<C>,
                  "cljonic::last: expected a cljonic collection, producer, or MapEntry value.");
}

} // namespace cljonic::core
