#ifndef CLJONIC_CORE_NTH_HPP
#define CLJONIC_CORE_NTH_HPP

#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Nth
* The \b Nth function returns the element from its first parameter, which must be a \b cljonic \b collection other than
* a \b cljonic \b Set, at the \b index given by its second parameter. If its second parameter is an invalid index for
* its first parameter then either the second parameter's \b default \b element is returned, or the third parameter is
* returned, which must be of the same type as the values in the first parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto n0{Nth(a, 0)};      // 0
    constexpr auto n1{Nth(a, 4)};      // 4
    constexpr auto n2{Nth(a, 9)};      // 9
    constexpr auto n3{Nth(a, 10)};     // 0, a's default value
    constexpr auto n4{Nth(a, 10, 11)}; // 11

    // Compiler Error: Nth's first parameter must be a cljonic collection other than a Set
    // constexpr auto t{Nth("Hello", 10)};

    // Compiler Error: Nth's first parameter must be a cljonic collection other than a Set
    // constexpr auto t{Nth(Set{1, 2, 3}, 10)};

    // Compiler Error: Nth's third parameter must have the same type as the values in the first parameter
    // const auto t{Nth(Array{1,2,3}, 10, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Nth(const C& c, const SizeType index) noexcept
{
    static_assert(IsCljonicCollection<C> and (not IsCljonicSet<C>),
                  "Nth's first parameter must be a cljonic collection other than a Set");

    return c[index];
}

template <typename C, typename T>
constexpr auto Nth(const C& c, const SizeType index, const T& t) noexcept
{
    static_assert(IsCljonicCollection<C> and (not IsCljonicSet<C>),
                  "Nth's first parameter must be a cljonic collection other than a Set");

    static_assert(std::same_as<typename C::value_type, T>,
                  "Nth's third parameter must have the same type as the values in the first parameter");

    return (index < c.Count()) ? c[index] : t;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_NTH_HPP
