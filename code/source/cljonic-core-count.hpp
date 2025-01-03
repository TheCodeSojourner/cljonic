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
    const auto countA{Count(a)};

    constexpr auto rng{Range<1, 5>{}};
    const auto countRng{Count(rng)};

    constexpr auto rpt{Repeat<3, int>{11}};
    const auto countRpt{Count(rpt)};

    const auto set{Set{1, 2, 1, 3, 4}};
    const auto countSet{Count(set)};

    constexpr auto str{String{"Hello"}};
    const auto countStr{Count(str)};;

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
