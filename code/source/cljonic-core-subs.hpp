#ifndef CLJONIC_CORE_subs_HPP
#define CLJONIC_CORE_subs_HPP

#include "cljonic-core-drop.hpp"
#include "cljonic-core-take.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Subs
* The \b Subs function returns a \b cljonic \b Array, which has the same \b MaximumCount as its \b cljonic \b collection
* first parameter, containing copies of the elements in its first parameter starting with index \b start, and ending
* with either index \b end-1, or the \b Count of the first parameter minus one. If the first parameter is empty, or
* the \b start and/or \b end parameters are out of range, the returned \b Array will be empty.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto subsA{Subs(a, 0, 3)}; // 11, 12, and 13

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto subsRng{Subs(rng, 1)}; // 2, 3, and 4

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto subsRpt{Subs(rpt, 0, 5)}; // 11, 11, and 11

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    constexpr auto subsSet{Subs(set, 4, 7)}; // empty Array

    constexpr auto str{String{"Hello"}};
    constexpr auto subsStr{Subs(str, 2, 4)}; // 'l' and 'l'

    // Compiler Error: Subs's first parameter must be a cljonic collection
    // const auto c{Subs("Hello, 3)};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Subs(const C& c, const SizeType start, const SizeType end) noexcept
{
    static_assert(IsCljonicCollection<C>, "Subs's first parameter must be a cljonic collection");

    return ((c.Count() == 0) or (start >= c.Count()) or (end <= start))
               ? Array<typename C::value_type, c.MaximumCount()>{}
               : Take((end - start), Drop(start, c));
}

template <typename C>
constexpr auto Subs(const C& c, const SizeType start) noexcept
{
    return Subs(c, start, c.Count());
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_subs_HPP
