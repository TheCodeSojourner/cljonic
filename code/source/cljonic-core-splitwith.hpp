#ifndef CLJONIC_CORE_SPLITWITH_HPP
#define CLJONIC_CORE_SPLITWITH_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"
#include "cljonic-core-drop.hpp"
#include "cljonic-core-takewhile.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_SplitWith
* The \b SplitWith function returns a \b cljonic \b Array of two \b cljonic \b Arrays, both with the same
* \b MaximumCount as its second parameter. The first \b cljonic \b Array contains the result of calling \b TakeWhile
* with the first parameter, which must be a \b unary \b predicate, and the second parameter. The second \b cljonic
* \b Array contains the remaining second parameter elements not in the first \b cljonic \b Array.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto IsLessThan13 = [](const int a) { return a < 13; };
    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};

    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto splitByA{SplitWith(IsLessThan13, a)}; // Array{11, 12} and Array{13, 14}

    constexpr auto b{Array{11, 13, 12, 14}};
    constexpr auto splitByB{SplitWith(IsLessThan13, b)}; // Array{11} and Array{13, 12, 14}

    const auto splitByItr{SplitWith(IsLessThan13, itr)}; // Array{1, ..., 12} and Array{13, ..., 1000}

    constexpr auto splitByRng{SplitWith(IsLessThan13, Range<0>{})}; // two empty Arrays

    constexpr auto splitByRpt{SplitWith(IsLessThan13, Repeat<4, int>{11})}; // Array{11, 11, 11, 11} and an empty Array

    constexpr auto splitBySet0{SplitWith(IsLessThan13, Set{11, 12, 13, 14})}; // Array{11, 12} and Array{13, 14}
    constexpr auto splitBySet1{SplitWith(IsLessThan13, Set{11, 13, 12, 14})}; // Array{11} and Array{13, 12, 14}

    constexpr auto NotC = [](const char c) { return c != 'c'; };
    constexpr auto splitByStr0{SplitWith(NotC, String{"abcd"})};   // Array{'a', 'b'} and Array{'c', 'd'}
    constexpr auto splitByStr1{SplitWith(NotC, String{"axbycz"})}; // Array{'a', 'x', 'b', 'y'} and Array{'c', 'z'}

    // Compiler Error: SplitWith's second parameter must be a cljonic collection
    // constexpr auto sb{SplitWith(NotC, "Hello")};

    // Compiler Error: SplitWith's function is not a valid binary predicate for the collection value type
    // constexpr auto sb{SplitWith(IsLessThan13, Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
constexpr auto SplitWith(F&& f, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "SplitWith's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<std::decay_t<F>, typename C::value_type>,
                  "SplitWith's function is not a valid unary predicate for the collection value type");

    const auto firstArray{TakeWhile(std::forward<F>(f), c)};
    const auto secondArray{Drop(firstArray.Count(), c)};
    return Array{firstArray, secondArray};
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_SPLITWITH_HPP
