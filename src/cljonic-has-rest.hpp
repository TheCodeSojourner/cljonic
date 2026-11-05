#pragma once

#include <cljonic-has-next.hpp>

namespace cljonic::core {

/** \anchor HasRest
 * The \b has_rest free function is an exact forwarding alias for \ref HasNext "has_next". It accepts exactly the same
 * collections and producers and uses the same preflight behavior, callback timing, and diagnostics.
 *
 * \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   constexpr auto vector_values = vector(10, 20);
   constexpr auto map_values = hash_map(1, 10, 2, 20);
   constexpr auto set_values = hash_set(10, 20);
   constexpr auto queue_values = queue(10, 20);
   constexpr auto string_values = string("AB");
   static_assert(has_rest(vector_values) == has_next(vector_values));
   static_assert(has_rest(map_values) == has_next(map_values));
   static_assert(has_rest(set_values) == has_next(set_values));
   static_assert(has_rest(queue_values) == has_next(queue_values));
   static_assert(has_rest(string_values) == has_next(string_values));
   static_assert(has_rest(range(0, 3)) == has_next(range(0, 3)));
   static_assert(has_rest(repeat(2, 7)) == has_next(repeat(2, 7)));

   const auto runtime_values = vector<int, 4>(40, 50);
   return has_rest(runtime_values) == has_next(runtime_values) ? 0 : 1;
 }
 ~~~~~
 */
template <concepts_detail::NextSequenceable C>
[[nodiscard]] constexpr auto has_rest(const C& value) noexcept -> bool {
    return has_next(value);
}

template <typename C>
    requires(!concepts_detail::NextSequenceable<C>)
constexpr auto has_rest([[maybe_unused]] const C& value) noexcept -> void {
    static_assert(concepts_detail::dependent_false<C>,
                  "cljonic::has_rest: expected a Vector, Map, Set, Queue, String, Range, Repeat, Iterate, Repeatedly, "
                  "TakeWhile, DropWhile, or supported Cycle value.");
}

} // namespace cljonic::core
