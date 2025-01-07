#ifndef CLJONIC_CORE_COUNT_HPP
#define CLJONIC_CORE_COUNT_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_Count
* The \b Count function returns the number of elements currently contained in a \b cljonic \b collection.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto countA{Count(a)}; // 4

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto countRng{Count(rng)}; // 4

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto countRpt{Count(rpt)}; // 3

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    constexpr auto countSet{Count(set)}; // 4

    constexpr auto str{String{"Hello"}};
    constexpr auto countStr{Count(str)}; // 5

    // Compiler Error: Count's parameter must be a cljonic collection
    // const auto c{Count("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Count(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Count's parameter must be a cljonic collection");

    return c.Count();
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_COUNT_HPP
