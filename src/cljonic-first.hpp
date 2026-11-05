#pragma once

#include <cljonic-collection-map.hpp>
#include <cljonic-concepts.hpp>

namespace cljonic::core {

/** \anchor First
 * The \b first free function returns the first logical element of a `Vector`, `Map`, `Set`, `Queue`, or `String` value
 * or any cljonic producer, or the key of a \ref MapEntryFactory "map_entry" value.
 *
 * `Vector` and `String` yield the element at index zero, `Queue` yields its front element, `Map` yields one
 * value-semantic \ref MapEntryFactory "map_entry" value, and `Set` yields one stored element. `Map` and `Set`
 * traversal order is not semantically significant. An empty collection or producer observation yields its element
 * type's \ref DefaultElement "default value". A producer yielding map entries returns a whole entry, not its key.
 *
 * Producer access evaluates only what is needed to find the first value, without counting or materializing the
 * sequence first. A `drop_while` producer may examine its entire bounded matching prefix. Each call starts a fresh
 * traversal; source and predicate callbacks may run again. A zero observation limit invokes no callbacks.
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
   static_assert(first(v_const) == 10);
   static_assert(first(e_const) == 1);
   static_assert(first(numbers) == 7);
   static_assert(first(suffix) == 4);
   static_assert(first(cycle(suffix)) == 4);
   static_assert(first(empty) == 0);
   static_assert(count(v_const) == 3);

   // Runtime demonstration.
   const auto value = 100;
   const auto values = vector(value, 200);
   int calls = 0;
   const auto generated = repeatedly([&calls, value]() noexcept {
     ++calls;
     return value;
   });

   return (first(values) == value && count(values) == 2 &&
           first(generated) == value && calls == 1 && first(empty) == 0)
              ? 0
              : 1;
 }
 ~~~~~
 */
template <typename KeyType, typename ValueType>
[[nodiscard]] constexpr auto first(const MapEntry<KeyType, ValueType>& entry) noexcept -> KeyType {
    return entry.key;
}

/** Returns the first logical element of a Vector, Map, Set, Queue, or String value. */
template <concepts::SequenceableCollection C>
[[nodiscard]] constexpr auto first(const C& collection) noexcept -> C::value_type {
    if (collection.count() == 0U) {
        return typename C::value_type{};
    }
    return *collection.begin();
}

/** Returns the first observable producer element, or its default value when empty. */
template <concepts::CljonicProducer P>
[[nodiscard]] constexpr auto first(const P& producer) noexcept -> P::value_type {
    if constexpr (CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U) {
        return typename P::value_type{};
    } else {
        const auto current = std::ranges::begin(producer);
        if (current == std::ranges::end(producer)) {
            return typename P::value_type{};
        }
        return *current;
    }
}

template <typename C>
    requires(!concepts::SequenceableCollection<C> && !concepts::CljonicProducer<C> &&
             !concepts_detail::is_cljonic_map_entry_v<C>)
constexpr auto first([[maybe_unused]] const C& value) noexcept -> void {
    static_assert(concepts_detail::dependent_false<C>,
                  "cljonic::first: expected a cljonic collection, producer, or MapEntry value.");
}

} // namespace cljonic::core
