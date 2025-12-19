#ifndef CLJONIC_CORE_size_HPP
#define CLJONIC_CORE_size_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_Size
* The \b Size function returns the maximum number of elements a \b cljonic \b collection can contain.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto sizeA{Size(a)}; // 4

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto sizeRng{Size(rng)}; // 4

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto sizeRpt{Size(rpt)}; // 3

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    constexpr auto sizeSet{Size(set)}; // 5

    constexpr auto str{String{"Hello"}};
    constexpr auto sizeStr{Size(str)}; // 5

    // Compiler Error: Size's parameter must be a cljonic collection
    // constexpr auto c{Size("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
[[nodiscard]] constexpr auto Size(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Size's parameter must be a cljonic collection");

    return c.MaximumCount();
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_size_HPP
