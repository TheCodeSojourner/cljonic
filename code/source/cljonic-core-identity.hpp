#ifndef CLJONIC_CORE_IDENTITY_HPP
#define CLJONIC_CORE_IDENTITY_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_Identity
* The \b Identity function returns a value that is the same value as its argument.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto identityA{Identity(a)};
    constexpr auto aEqual{Equal(a, identityA)}; // true

    const auto itr{Iterator{[](const int i) { return i + 1; }, 0}};
    const auto identityItr{Identity(itr)};
    const auto itrEqual{Equal(itr, identityItr)}; // true

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto identityRng{Identity(rng)};
    constexpr auto rngEqual{Equal(rng, identityRng)}; // true

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto identityRpt{Identity(rpt)};
    constexpr auto rptEqual{Equal(rpt, identityRpt)}; // true

    constexpr auto set{Set{1, 2, 3, 1}};
    constexpr auto identitySet{Identity(set)};
    constexpr auto setEqual{Equal(set, identitySet)}; // true

    constexpr auto str{String{"Hello"}};
    constexpr auto identityStr{Identity(str)};
    constexpr auto strEqual{Equal(str, identityStr)}; // true

    return 0;
}
~~~~~
*/
template <typename T>
constexpr auto Identity(const T& t) noexcept
{
    return t;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_IDENTITY_HPP
