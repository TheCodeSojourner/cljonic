#ifndef CLJONIC_CORE_SPLITAT_HPP
#define CLJONIC_CORE_SPLITAT_HPP

#include <tuple>
#include "cljonic-concepts.hpp"
#include "cljonic-core-drop.hpp"
#include "cljonic-core-take.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_SplitAt
* The \b SplitAt function returns a \b cljonic \b Array of two \b cljonic \b Arrays, both with the same \b MaximumCount
* as its second parameter. The first \b cljonic \b Array contains the value returned by executing the \b Take function
* with the first parameter, and the second parameter. The second \b cljonic \b Array contains the value returned by
* executing the \b Drop function with the first parameter, and the second parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto sa0{SplitAt(0, a)};                                // Empty Array and Array with copy of a
    constexpr auto sa1{SplitAt(1, a)};                                // Array{0} and Array of a, without 0
    constexpr auto saRng50{SplitAt(50, Range<0>{})};                  // Empty Array and empty Array
    constexpr auto saRpt7{SplitAt(50, Repeat<7, const char*>{"11"})}; // Array of "11"s and empty Array
    constexpr auto saSet5{SplitAt(5, Set{'a', 'b'})};                 // Array of 'a' and 'b', and empty Array
    constexpr auto saStr3{SplitAt(3, String{"Hello"})}; // Array of 'H', 'e' and 'l', and Array of 'l', 'o'

    // Compiler Error: SplitAt's second parameter must be a cljonic collection
    // constexpr auto sa{SplitAt(10, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
[[nodiscard]] constexpr auto SplitAt(const SizeType count, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "SplitAt's second parameter must be a cljonic collection");

    return Array{Take(count, c), Drop(count, c)};
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_SPLITAT_HPP
