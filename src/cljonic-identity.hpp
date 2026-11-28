#pragma once

#include <cljonic-concepts.hpp>

#include <concepts>
#include <type_traits>

namespace cljonic::core {

/** \anchor Identity
 * The \b identity free function returns its argument unchanged.
 *
 * \b Examples
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 using namespace cljonic::core;

 int main() {
   constexpr auto number = identity(42);
   constexpr auto numbers = identity(vector(1, 2, 3));
   static_assert(number == 42);
   static_assert(numbers(0) == 1);
   static_assert(numbers(2) == 3);

   constexpr auto sc = identity(string("Hello"));
   static_assert(equal(sc, string("Hello")));

   const auto runtime_value = 7;
   const auto runtime_result = identity(runtime_value);

   constexpr auto rc = identity(string("Hello"));

   return runtime_result == 7 && equal(rc, string("Hello")) ? 0 : 1;
 }
 ~~~~~
 */
template <typename T>
    requires std::is_nothrow_copy_constructible_v<T>
[[nodiscard]] constexpr auto identity(const T& value) noexcept -> T {
    return T{value};
}

template <typename T>
    requires(!std::is_nothrow_copy_constructible_v<T>)
constexpr auto identity([[maybe_unused]] const T& value) noexcept -> void {
    static_assert(concepts_detail::dependent_false<T>,
                  "cljonic::identity: the argument must be nothrow copy constructible.");
}

template <typename... Args>
    requires(sizeof...(Args) != 1U)
constexpr auto identity([[maybe_unused]] const Args&... args) noexcept -> void {
    static_assert(concepts_detail::dependent_false<Args...>, "cljonic::identity: expected exactly one argument.");
}

} // namespace cljonic::core
