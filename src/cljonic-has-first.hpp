#pragma once

#include <cljonic-first.hpp>

#include <ranges>

namespace cljonic::core {

/** \anchor HasFirst
 * The \b has_first free function reports whether \ref First "first" has a result for a supported collection, producer,
 * or \ref MapEntry "MapEntry" value.
 *
 * A collection is nonempty when it contains at least one logical element. For a producer, this function observes only
 * what is needed to determine whether a first element exists. It does not count or materialize the producer. The
 * producer's callbacks may run as they do during \ref First "first" observation. At a zero observation limit, no
 * source or predicate callback runs. A \ref MapEntry "MapEntry" always has a first result: its key.
 *
 * \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   constexpr auto values = vector(10, 20);
   constexpr auto empty_values = vector<int, 0>();
   constexpr auto no_produced_values = repeat(0, 7);
   constexpr auto default_first_value = repeat(1, 7);
   constexpr auto entry = map_entry(1, 100);
   static_assert(has_first(values));
   static_assert(!has_first(empty_values));
   static_assert(!has_first(no_produced_values));
   static_assert(has_first(default_first_value));
   static_assert(has_first(entry));

   const int value = 40;
   const auto runtime_values = vector(value, value + 1);
   auto callback_calls = 0;
   const auto generated = repeatedly([&callback_calls, value]() noexcept {
     ++callback_calls;
     return value;
   });
   const bool has_generated_value = has_first(generated);
   const bool callback_result =
       cljonic::CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U
           ? !has_generated_value && callback_calls == 0
           : has_generated_value && callback_calls == 1;
   return has_first(runtime_values) && runtime_values(0) == value &&
                  callback_result
              ? 0
              : 1;
 }
 ~~~~~
 */
template <typename KeyType, typename ValueType>
[[nodiscard]] constexpr auto has_first([[maybe_unused]] const MapEntry<KeyType, ValueType>& entry) noexcept -> bool {
    return true;
}

/** Reports whether a Vector, Map, Set, Queue, or String contains a logical element. */
template <concepts::SequenceableCollection C>
[[nodiscard]] constexpr auto has_first(const C& collection) noexcept -> bool {
    return collection.count() != 0U;
}

/** Reports whether a producer has a first observable element. */
template <concepts::CljonicProducer P>
[[nodiscard]] constexpr auto has_first(const P& producer) noexcept -> bool {
    if constexpr (CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_VALUE == 0U) {
        return false;
    } else {
        const auto current = std::ranges::begin(producer);
        return current != std::ranges::end(producer);
    }
}

template <typename C>
    requires(!concepts::SequenceableCollection<C> && !concepts::CljonicProducer<C> &&
             !concepts_detail::is_cljonic_map_entry_v<C>)
constexpr auto has_first([[maybe_unused]] const C& value) noexcept -> void {
    static_assert(concepts_detail::dependent_false<C>,
                  "cljonic::has_first: expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, "
                  "TakeWhile, DropWhile, supported Cycle, or MapEntry value.");
}

} // namespace cljonic::core
