#ifndef CLJONIC_CORE_CONCAT_HPP
#define CLJONIC_CORE_CONCAT_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{
/** \anchor Core_Concat
* The \b Concat function returns a \b cljonic \b Array with all of the elements of its first parameter, which must be a
* \b cljonic \b collection, followed by all of the elements of its second parameter, which must also be a \b cljonic
* \b collection, etc. If \b Concat is called with no parameters it returns an empty \b cljonic \b Array of \b int.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto rng{Range<5>{}};
    constexpr auto rpt{Repeat<5, int>{10}};
    constexpr auto set{Set{100, 101}};
    constexpr auto str{String{"Hello"}};
    constexpr auto c{Concat(a, rng, rpt, set, str)};

    // Compiler Error: All Concat parameters must be cljonic collections
    // const auto c{Concat("Hello", 1)};

    // Compiler Error: All Concat cljonic collection value types must be interconvertible
    // const auto c{Concat(Array{"one", "two", "three"}, Repeat<5, int>{1})};

    return 0;
}
~~~~~
*/
template <typename C, typename... Cs>
constexpr auto Concat(const C& c, const Cs&... cs) noexcept
{
    // #lizard forgives -- The length of this function is acceptable

    static_assert(AllCljonicCollections<C, Cs...>, "All Concat parameters must be cljonic collections");

    static_assert(AllConvertibleValueTypes<C, Cs...>,
                  "All Concat cljonic collection value types must be interconvertible");

    using ResultType = FindCommonValueType<C, Cs...>;

    constexpr auto count{SumOfCljonicCollectionMaximumCounts<C, Cs...>()};
    auto result{Array<ResultType, count>{}};
    const auto MConjCollectionOnResult = [&](const auto& c)
    {
        for (const auto& v : c)
            MConj(result, static_cast<ResultType>(v));
    };
    (MConjCollectionOnResult(c), ..., MConjCollectionOnResult(cs));
    return result;
}

constexpr auto Concat() noexcept
{
    return Array<int, 0>{};
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_CONCAT_HPP
