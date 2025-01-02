#ifndef CLJONIC_CORE_FIRST_HPP
#define CLJONIC_CORE_FIRST_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_First
* The \b First function returns the first element in a \b cljonic \b collection.  If the \b cljonic \b collection has
* no elements its default element is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    const auto a0{First(a)};

    constexpr auto rng{Range<1, 5>{}};
    const auto rng0{First(rng)};

    constexpr auto rpt{Repeat<3, int>{11}};
    const auto rpt0{First(rpt)};

    const auto set{Set{1, 2, 1, 3, 4}};
    const auto set0{First(set)};

    constexpr auto str{String{"Hello"}};
    const auto str0{First(str)};

    // Compiler Error: First parameter must be a cljonic collection
    // const auto f{First("Hello")};

return 0;
}
~~~~~
*/
template <typename C>
constexpr auto First(const C& coll) noexcept
{
    static_assert(IsCljonicCollection<C>, "First's parameter must be a cljonic collection");

    return coll[0];
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_FIRST_HPP
