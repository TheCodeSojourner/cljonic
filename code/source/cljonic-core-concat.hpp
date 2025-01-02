#ifndef CLJONIC_CORE_CONCAT_HPP
#define CLJONIC_CORE_CONCAT_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{
/** \anchor Core_Concat
* The \b Concat function returns a \b cljonic \b Array with all of the elements of its first parameter, which must be a
* \b cljonic \b collection, followed by all of the elements of its second parameter, which must also be a \b cljonic
* \b collection, etc.
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
    const auto c{Concat(a, rng, rpt, set, str)};

    // Compiler Error: All Concat parameters must be cljonic collections
    // const auto c{Concat("Hello", 1)};

    // Compiler Error: All Concat cljonic collections value types must be interconvertible
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
                  "All Concat cljonic collections value types must be interconvertible");

    using ResultType = FindCommonValueType<C, Cs...>;
    using SizeType = decltype(c.Count());

    constexpr auto CljonicCollectionMaximumElementCount{
        static_cast<SizeType>(CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT)};

    constexpr auto count{Min(SumOfCljonicCollectionMaximumCounts<C, Cs...>(), CljonicCollectionMaximumElementCount)};
    auto result{Array<ResultType, count>{}};
    const auto MConjCollectionOntoResult = [&](const auto& c)
    {
        for (SizeType i{0}; i < c.Count(); ++i)
            result.MConj(c[i]);
    };
    (MConjCollectionOntoResult(c), ..., MConjCollectionOntoResult(cs));
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_CONCAT_HPP
