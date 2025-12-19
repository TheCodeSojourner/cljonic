#ifndef CLJONIC_CORE_FIRST_HPP
#define CLJONIC_CORE_FIRST_HPP

#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Second
* The \b Second function returns the second element in a \b cljonic \b collection.  If the \b cljonic \b collection has
* no second element its \b default \b element is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto a0{Second(a)};

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto rng0{Second(rng)};

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto rpt0{Second(rpt)};

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    constexpr auto set0{Second(set)};

    constexpr auto str{String{"Hello"}};
    constexpr auto str0{Second(str)};

    // Compiler Error: Second's parameter must be a cljonic collection
    // constexpr auto f{Second("Hello")};

return 0;
}
~~~~~
*/
template <typename C>
[[nodiscard]] constexpr auto Second(const C& coll) noexcept
{
    static_assert(IsCljonicCollection<C>, "Second's parameter must be a cljonic collection");

    return coll[1];
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_FIRST_HPP
