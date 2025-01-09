#ifndef CLJONIC_CORE_SORTBY_HPP
#define CLJONIC_CORE_SORTBY_HPP

#include <tuple>
#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"
#include "cljonic-core-drop.hpp"
#include "cljonic-core-takewhile.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_SplitBy
* The \b SplitBy function, called \b split-with in \b Clojure, returns a \b tuple of two \b cljonic \b Arrays, both with
* the same \b MaximumCount as its second parameter. The first returned \b cljonic \b collection contains the result of
* calling \b TakeWhile with the first parameter, which must be a \b unary \b predicate, and the second parameter. The
* second returned \b cljonic \b collection contains the rest of the second parameter's elements.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto IsLessThan13 = [](const int a) { return a < 13; };

    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto splitByA{SplitBy(IsLessThan13, a)}; // Array{11, 12} and Array{13, 14}

    constexpr auto b{Array{11, 13, 12, 14}};
    constexpr auto splitByB{SplitBy(IsLessThan13, b)}; // Array{11} and Array{13, 12, 14}

    constexpr auto splitByRng{SplitBy(IsLessThan13, Range<0>{})}; // two empty Arrays

    constexpr auto splitByRpt{SplitBy(IsLessThan13, Repeat<4, int>{11})}; // Array{11, 11, 11, 11} and an empty Array

    constexpr auto splitBySet0{SplitBy(IsLessThan13, Set{11, 12, 13, 14})}; // Array{11, 12} and Array{13, 14}
    constexpr auto splitBySet1{SplitBy(IsLessThan13, Set{11, 13, 12, 14})}; // Array{11} and Array{13, 12, 14}

    constexpr auto NotC = [](const char c) { return c != 'c'; };
    constexpr auto splitByStr0{SplitBy(NotC, String{"abcd"})};   // Array{'a', 'b'} and Array{'c', 'd'}
    constexpr auto splitByStr1{SplitBy(NotC, String{"axbycz"})}; // Array{'a', 'x', 'b', 'y'} and Array{'c', 'z'}

    // Compiler Error: SplitBy's second parameter must be a cljonic collection
    // constexpr auto sb{SplitBy(NotC, "Hello")};

    // Compiler Error: SplitBy's function is not a valid binary predicate for the collection value type
    // constexpr auto sb{SplitBy(IsLessThan13, Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
constexpr auto SplitBy(F&& f, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "SplitBy's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<std::decay_t<F>, typename C::value_type>,
                  "SplitBy's function is not a valid unary predicate for the collection value type");

    const auto firstArray{TakeWhile(f, c)};
    const auto secondArray{Drop(firstArray.Count(), c)};
    return std::make_tuple(std::move(firstArray), std::move(secondArray));
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_SORTBY_HPP
