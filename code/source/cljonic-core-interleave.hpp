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
* all be \b cljonic \b collections, followed by the second element in each of its parameters, etc. The \b MaximumCount
* of the return value is the number of parameters times the smallest \b MaximumCount of all the parameters. If no
* parameters are given an empty \b cljonic \b Array of \b int is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a0{Array{1, 2, 3}};
    constexpr auto a1{Array{11, 12, 13}};
    constexpr auto a2{Array{111, 112}};
    constexpr auto i{Iterate([](const int i) { return 1 + i; }, 11)};
    constexpr auto i0{Interleave()};                             // empty Array of int
    constexpr auto i1{Interleave(a0, a1)};                       // 1, 11, 2, 12, 3, 13
    constexpr auto i2{Interleave(a0, a2)};                       // 1, 111, 2, 112
    constexpr auto i3{Interleave(a0, a1, a2)};                   // 1, 11, 111, 2, 12, 112
    constexpr auto i4{Interleave(a0, Range<>{})};                // 1, 0, 2, 1, 3, 2
    constexpr auto i5{Interleave(a0, Repeat{7})};                // 1, 7, 2, 7, 3, 7
    constexpr auto i6{Interleave(a0, Set{3, 5, 7, 9})};          // 1, 3, 2, 5, 3, 7
    constexpr auto i7{Interleave(String{"Hello"}, Repeat{'_'})}; // 'H', '_', 'e', '_', 'l', '_', 'l', '_', 'o', '_'
    const auto i8{Interleave(a0, i, a2)};                        // 1, 11, 111, 2, 12, 112

    // Compiler Error: Interleave's parameters must all be cljonic collections
    // constexpr auto m{Interleave(4)};
    // constexpr auto m{Interleave(Array{1, 2, 3}, 4)};

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

    static_assert(AllCljonicCollections<C, Cs...>, "Interleave's parameters must all be cljonic collections");

    static_assert(AllConvertibleValueTypes<C, Cs...>,
                  "All Interleave cljonic collection value types must be interconvertible");

    using ResultType = FindCommonValueType<C, Cs...>;
    constexpr auto minimumCollectionMaximumCount{MinimumOfCljonicCollectionMaximumCounts<C, Cs...>()};
    constexpr auto collectionsCount{sizeof...(Cs) + 1};
    constexpr auto resultCount{minimumCollectionMaximumCount * collectionsCount};
    auto result{Array<ResultType, resultCount>{}};
    auto itrs{std::make_tuple(c.begin(), cs.begin()...)};
    for (SizeType i{0}; i < minimumCollectionMaximumCount; ++i)
        std::apply([&](auto&... itrs) { (MConj(result, *itrs++), ...); }, itrs);
    return result;
}

constexpr auto Interleave() noexcept
{
    return Array<int, 0>{};
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_Interleave_HPP
