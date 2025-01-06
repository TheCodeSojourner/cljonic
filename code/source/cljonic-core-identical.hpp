#ifndef CLJONIC_CORE_EQUAL_HPP
#define CLJONIC_CORE_EQUAL_HPP

#include "cljonic-core-identity.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Identical
* The \b Identical function returns true if all of its parameters return the same value when used in a call to the
* \b cljonic \b Identity function.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a1{Array<int, 10>{1, 2, 3, 4}};
    constexpr auto a2{Array<int, 10>{1, 2, 3, 4}};
    constexpr auto i0{Identical(a1)};     // true
    constexpr auto i1{Identical(a1, a1)}; // true
    constexpr auto i2{Identical(a1, a2)}; // false

    constexpr auto i3{Identical(Range<5>{}, Range<5>{})}; // false

    constexpr auto x{12};
    constexpr auto y{1};
    constexpr auto i4{Identical(x, x)}; // true
    constexpr auto i5{Identical(x, y)}; // false

    return 0;
}
~~~~~
*/
template <typename T, typename... Ts>
constexpr auto Identical(const T& t, const Ts&... ts) noexcept
{
    if constexpr (sizeof...(Ts) <= 0)
    {
        return true;
    }
    else
    {
        auto SameIdentity = [&](const auto& t1, const auto& t2) { return Identity(t1) == Identity(t2); };
        return (SameIdentity(t, ts) and ...);
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_EQUAL_HPP
