#ifndef CLJONIC_CORE_MAP_HPP
#define CLJONIC_CORE_MAP_HPP

#include <concepts>
#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Map
* The \b Map function returns a \b cljonic \b Array of the values returned by calling its first parameter, which must
* be a function, with the first elements of the rest of its parameters, which must be one or more \b cljonic
* \b collections, followed by the second elements of the rest of its parameters, and so on.  The returned \b Array has
* the same \b MaximumCount as the \b smallest \b MaximumCount of all of the \b cljonic \b collection parameters.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto TwoTimes = [](const int i) { return 2 * i; };
    constexpr auto Add2 = [](const int i, const int j) { return i + j; };
    constexpr auto Add3 = [](const int i, const int j, const int k) { return i + j + k; };

    const auto m0{Map(TwoTimes, Array{1, 2, 3, 4})};                 // immutable, full, 2, 4, 6, and 8
    const auto m1{Map(TwoTimes, Array<int, 10>{1, 2, 3, 4})};        // immutable, sparse, 2, 4, 6, and 8
    const auto m2{Map(TwoTimes, Array<int, 10>{})};                  // immutable, sparse, empty
    const auto m3{Map(Add2, Array<int, 10>{1, 2, 3, 4}, Range{})};   // immutable, sparse, 1, 3, 5, and 7
    const auto m4{Map(Add2, Array<int, 10>{1, 2, 3, 4}, Repeat{1})}; // immutable, sparse, 2, 3, 4, and 5
    const auto m5{Map(Add3, Set{1, 2, 3, 4}, Range{}, Repeat{3})};   // immutable, full, 4, 6, 8, and 10

    // Compiler Error: Second parameter must be a cljonic collection
    // const auto m{Map(TwoTimes, 4)};
    // const auto m{Map(TwoTimes, Range{}, 4)};

    // Compiler Error: Function cannot be called with values from the specified cljonic collections
    // const auto m{Map([](const char* str) { return str[0]; }, Array{1, 2, 3, 4})};
    // const auto m{Map([](const char* str) { return str[0]; }, Array{1, 2, 3, 4}, Range{})};
    // const auto m{Map([](const char* str, const int i) { return str[0] + i; }, Array{1, 2, 3, 4}, Range{})};

return 0;
}
~~~~~
*/
template <typename F, typename C, typename... Cs>
constexpr auto Map(F&& f, const C& c, const Cs&... cs) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable.

    static_assert(AllCljonicCollections<C, Cs...>, "The second through last parameters must be cljonic collections");

    static_assert(std::invocable<F, typename C::value_type, typename Cs::value_type...>,
                  "Function cannot be called with values from the specified cljonic collections");

    using ResultType = decltype(f(std::declval<typename C::value_type>(), std::declval<typename Cs::value_type>()...));

    constexpr auto count{MinimumOfCljonicCollectionMaximumCounts<C, Cs...>()};
    auto result{Array<ResultType, count>{}};
    for (SizeType i{0}; i < c.Count(); ++i)
        result.MConj(f(c[i], cs[i]...));
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_MAP_HPP
