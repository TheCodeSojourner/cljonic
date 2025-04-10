#ifndef CLJONIC_CORE_Interleave_HPP
#define CLJONIC_CORE_Interleave_HPP

#include <concepts>
#include <tuple>
#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Interleave
* The \b Interleave function returns a \b cljonic \b Array of the first element in each of its parameters, which must
* all be \b cljonic \b collections other than \b Set, followed by the second element in each of its parameters, etc. The
* \b MaximumCount of the return value is the number of parameters times the smallest \b MaximumCount of all the
* parameters. If only one collection is provided then its values will be returned. If no parameters are given an empty
* \b cljonic \b Array of \b int is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a0{Array{1, 2, 3}};
    constexpr auto a1{Array{11, 12, 13}};
    constexpr auto a2{Array{111, 112}};
    constexpr auto i0{Interleave()};                                               // empty Array of int
    constexpr auto i1{Interleave(a0)};                                             // 1, 2, 3
    constexpr auto i2{Interleave(a0, a1)};                                         // 1, 11, 2, 12, 3, 13
    constexpr auto i3{Interleave(a0, a2)};                                         // 1, 111, 2, 112
    constexpr auto i4{Interleave(a0, a1, a2)};                                     // 1, 11, 111, 2, 12, 112
    const auto i6{Interleave(a0, Iterate([](const int i) { return i + 1; }, 10))}; // 1, 10, 2, 11, 3, 12
    constexpr auto i5{Interleave(a0, Range<>{})};                                  // 1, 0, 2, 1, 3, 2
    constexpr auto i7{Interleave(a0, Repeat{7})};                                  // 1, 7, 2, 7, 3, 7
    constexpr auto i8{Interleave(String{"Hi"}, Repeat{'_'})};                      // 'H', '_', 'i', '_'

    // Compiler Error: Interleave's parameters must all be non-Set cljonic collections
    // constexpr auto m{Interleave(4)};
    // constexpr auto m{Interleave(Array{1, 2, 3}, 4)};
    // constexpr auto m{Interleave(a0, Set{3, 5, 7, 9})};

    // Compiler Error: All Interleave cljonic collection value types must be interconvertible
    // constexpr auto m{Interleave(Array{1, 2, 3, 4}, Array{"one", "two", "three"})};

    return 0;
}
~~~~~
*/
template <typename C, typename... Cs>
constexpr auto Interleave(const C& c, const Cs&... cs) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(AllCljonicNonSet<C, Cs...>, "Interleave's parameters must all be non-Set cljonic collections");

    static_assert(AllConvertibleValueTypes<C, Cs...>,
                  "All Interleave cljonic collection value types must be interconvertible");

    if constexpr (sizeof...(Cs) == 0)
    {
        using ResultType = typename C::value_type;

        constexpr auto count{C::MaximumCount()};
        auto result{Array<ResultType, count>{}};
        for (const auto& e : c)
            MConj(result, e);
        return result;
    }
    else
    {
        constexpr auto NoCollectionsAreAtEnd{
            [](const auto& collectionTuples) noexcept
            {
                return std::apply([](const auto&... tuples)
                                  { return (... and (std::get<1>(tuples) != std::get<2>(tuples))); },
                                  collectionTuples);
            }};
        constexpr auto numberOfCollections{SizeType{sizeof...(Cs) + 1}};
        constexpr auto minimumCollectionMaximumCount{MinimumOfCljonicCollectionMaximumCounts<C, Cs...>()};
        constexpr auto resultCount{minimumCollectionMaximumCount * numberOfCollections};
        auto result{Array<CommonValueType<C, Cs...>, resultCount>{}};
        const auto MConjAndAdvance = [&](auto&... tuples)
        { (..., (MConj(result, *std::get<1>(tuples)), ++std::get<1>(tuples))); };
        auto collectionTuples{
            std::tuple{std::make_tuple(&c, c.begin(), c.end()), std::make_tuple(&cs, cs.begin(), cs.end())...}};
        while (NoCollectionsAreAtEnd(collectionTuples))
            std::apply(MConjAndAdvance, collectionTuples);
        return result;
    }
}

constexpr auto Interleave() noexcept
{
    return Array<int, 0>{};
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_Interleave_HPP
