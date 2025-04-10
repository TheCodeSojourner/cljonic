#ifndef CLJONIC_CORE_EQUAL_HPP
#define CLJONIC_CORE_EQUAL_HPP

#include "cljonic-core-identity.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Identical
* The \b Identical function returns true if its two parameters have the same memory address.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a1{Array<int, 10>{1, 2, 3, 4}};
    constexpr auto a2{Array<int, 10>{1, 2, 3, 4}};
    constexpr auto i1{Identical(a1, a1)}; // true
    constexpr auto i2{Identical(a1, a2)}; // false

    constexpr auto i3{Identical(Range<5>{}, Range<5>{})}; // false

    constexpr auto x{12};
    constexpr auto y{1};
    constexpr auto i4{Identical(x, x)}; // true
    constexpr auto i5{Identical(x, y)}; // false
    constexpr auto i6{Identical(x, 12)}; // false
    constexpr auto i7{Identical(12, 12)}; // false

    return 0;
}
~~~~~
*/
template <typename T, typename U>
constexpr auto Identical(const T& t, const U& u) noexcept
{
    const auto tMemoryAddress{(void*)&t};
    const auto uMemoryAddress{(void*)&u};
    return tMemoryAddress == uMemoryAddress;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_EQUAL_HPP
