#ifndef CLJONIC_CORE_DEFAULTELEMENT_HPP
#define CLJONIC_CORE_DEFAULTELEMENT_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_DefaultElement
* The \b DefaultElement function returns the default element of a \b cljonic \b collection.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto deA{DefaultElement(a)};

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto deRng{DefaultElement(rng)};

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto deRpt{DefaultElement(rpt)};

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    constexpr auto deSet{DefaultElement(set)};

    constexpr auto str{String{"Hello"}};
    constexpr auto deStr{DefaultElement(str)};

    // Compiler Error: DefaultElement's parameter must be a cljonic collection
    // const auto de{DefaultElement("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
[[nodiscard]] constexpr auto DefaultElement(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "DefaultElement's parameter must be a cljonic collection");

    return c.DefaultElement();
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_DEFAULTELEMENT_HPP
