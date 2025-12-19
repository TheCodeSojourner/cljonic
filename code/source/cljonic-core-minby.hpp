#ifndef CLJONIC_CORE_MINBY_HPP
#define CLJONIC_CORE_MINBY_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_MinBy
* The \b MinBy function uses its first parameter, which must be a \b binary \b predicate that returns \b true if its
* first parameter is less than its second parameter, to return the minimum value of either the values in its \b cljonic
* \b collection second parameter or its second through last parameters. If its \b cljonic \b collection second parameter
* is empty, then \b MinBy returns the default value of its \b cljonic \b collection second parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto IsALessThanB = [](const int a, const int b) { return a < b; };

    constexpr auto a{Array{11, 14, 13, 14}};
    constexpr auto minByA{MinBy(IsALessThanB, a)}; // 11

    constexpr auto b{Array{11}};
    constexpr auto minByB{MinBy(IsALessThanB, b)}; // 11

    constexpr auto minByRng{MinBy(IsALessThanB, Range<0>{})};          // 0, the default value of Range<0>
    constexpr auto minByRpt{MinBy(IsALessThanB, Repeat<4, int>{11})};  // 11
    constexpr auto minBySet{MinBy(IsALessThanB, Set{11, 14, 13, 14})}; // 11
    constexpr auto minByStr{MinBy([](const char i, const char j) { return i < j; }, String{"adcb"})}; // 'a'
    constexpr auto minBy{MinBy(IsALessThanB, 1, 2, 14, 7, 9)}; // 1

    // Compiler Error: MinBy's second parameter must be a cljonic collection
    // constexpr auto sb{MinBy(IsALessThanB, "Hello")};

    // Compiler Error: MinBy function is not a valid binary predicate for the collection value type
    // constexpr auto sb{MinBy(IsALessThanB, Array<const char*, 5>{})};

    // Compiler Error: None of MinBy's second through last parameters can be cljonic collections
    // constexpr auto sb{MinBy(IsALessThanB, 1, "Hello", Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename T, typename... Ts>
[[nodiscard]] constexpr auto MinBy(F&& f, const T& t, const Ts&... ts) noexcept
{
    // #lizard forgives -- The complexity and length of this function is acceptable

    if constexpr (sizeof...(Ts) == 0)
    {
        static_assert(IsCljonicCollection<T>, "MinBy's second parameter must be a cljonic collection");

        static_assert(IsBinaryPredicate<F, typename T::value_type, typename T::value_type>,
                      "MinBy function is not a valid binary predicate for the collection value type");

        auto result{t.DefaultElement()};
        if (t.Count() > 0)
        {
            result = t[0];
            for (SizeType i{1}; i < t.Count(); ++i)
                if (not f(result, t[i]))
                    result = t[i];
        }
        return result;
    }
    else
    {
        static_assert(AllNotCljonicCollections<T, Ts...>,
                      "None of MinBy's second through last parameters can be cljonic collections");

        static_assert(IsBinaryPredicate<F, T, T>,
                      "MinBy function is not a valid binary predicate for the collection value type");

        // ***** CAUTION: C++ Voodoo Ahead *****
        // This code initializes "result" to the value "t" then applies the predicate "f" to "result" and each element
        // in the parameter pack "ts", and updates "result" if the predicate returns false. The "(void)" cast ensures
        // that the expression is evaluated for its "side effects" only, and the comma operator ensures that this is
        // done for each element in the parameter pack "ts".
        auto result{t};
        (..., (void)((not f(result, ts)) ? result = ts : result));
        return result;
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_MINBY_HPP
