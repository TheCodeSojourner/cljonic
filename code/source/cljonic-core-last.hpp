#ifndef CLJONIC_CORE_LAST_HPP
#define CLJONIC_CORE_LAST_HPP

#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Last
* The \b Last function returns the last element in a \b cljonic \b collection.  If the \b cljonic \b collection has
* no elements its \b default \b element is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto a0{Last(a)};

    constexpr auto itr{Iterate([](const int i) { return 1 + i; }, 1)};
    const auto itr0{Last(itr)};

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto rng0{Last(rng)};

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto rpt0{Last(rpt)};

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    constexpr auto set0{Last(set)};

    constexpr auto str{String{"Hello"}};
    constexpr auto str0{Last(str)};

    // Compiler Error: Last's parameter must be a cljonic collection
    // constexpr auto f{Last("Hello")};

return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Last(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Last's parameter must be a cljonic collection");

    return *(c.begin() + (c.Count() - 1));
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_LAST_HPP
