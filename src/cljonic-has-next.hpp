#pragma once

#include <cljonic-count.hpp>
#include <cljonic-next.hpp>

namespace cljonic {

/** \anchor HasNext
 * The \b has_next free function checks whether a \ref Next "next" call has a nonempty observable result.
 *
 * For a supported \ref Vector "Vector", \ref Map "Map", \ref Set "Set", \ref Queue "Queue", or \ref String "String",
 * it returns true when more than one logical element is present, without constructing the result of \ref Next "next".
 * For any producer supported by \ref Next, it constructs the producer tail and checks whether its first observable
 * element exists. This follows the producer's observation policy and callback rules, so callback-backed producers may
 * be traversed. Map and Set traversal order does not affect the result. Unsupported inputs are rejected at compile
 * time.
 *
 * \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;

 int main() {
   // Compile-time demonstration.
   constexpr auto values = vector(10, 20, 30);
   constexpr auto singleton = queue(10);
   constexpr auto empty_values = vector<int, 0>();
   constexpr auto repeated_values = repeat(2, 7);
   constexpr auto final_repeat = repeat(1, 7);
   constexpr auto repeated_cycle = cycle(vector(10));
   static_assert(has_next(values));
   static_assert(!has_next(singleton));
   static_assert(!has_next(empty_values));
   static_assert(has_next(repeated_values));
   static_assert(!has_next(final_repeat));
   static_assert(has_next(repeated_cycle));

   // Runtime demonstration.
   const int first = 40;
   const int second = first + 1;
   const auto runtime_values = vector<int, 4>(first, second);
   const auto runtime_singleton = hash_set(first);
   const auto runtime_repeated_values = repeat(3, first);
   const auto runtime_cycle = cycle(vector(first));

   return (has_next(runtime_values) && !has_next(runtime_singleton) &&
           has_next(runtime_repeated_values) && has_next(runtime_cycle) &&
           count(runtime_values) == 2 && runtime_values(0) == first)
              ? 0
              : 1;
 }
 ~~~~~
 */
template <concepts_detail::NextSequenceableCollection C>
[[nodiscard]] constexpr auto has_next(const C& collection) noexcept -> bool {
    return count(collection) > 1U;
}

template <concepts_detail::NextSequenceableProducer P>
[[nodiscard]] constexpr auto has_next(const P& producer) noexcept -> bool {
    const auto tail = next(producer);
    return tail.begin() != tail.end();
}

template <typename C>
    requires(!concepts_detail::NextSequenceable<C>)
constexpr auto has_next([[maybe_unused]] const C& value) noexcept -> void {
    static_assert(concepts_detail::dependent_false<C>,
                  "cljonic::has_next: expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, "
                  "TakeWhile, DropWhile, or supported Cycle value.");
}

} // namespace cljonic
