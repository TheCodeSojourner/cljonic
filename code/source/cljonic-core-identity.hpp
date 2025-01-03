#ifndef CLJONIC_CORE_IDENTITY_HPP
#define CLJONIC_CORE_IDENTITY_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_Identity
* The \b Identity function returns the unique identity (i.e., its address in memory) of its argument.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    const auto identityA{Identity(a)};

    constexpr auto rng{Range<1, 5>{}};
    const auto identityRng{Identity(rng)};

    const auto b0{identityA == identityA};   // true
    const auto b1{identityA == identityRng}; // false

    const auto b2{Identity(1) == Identity(2)}; // false
    const auto b3{identityA == Identity(2)};   // false

    return 0;
}
~~~~~
*/
template <typename T>
constexpr void* Identity(const T& t) noexcept
{
    return (void*)&t;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_IDENTITY_HPP
