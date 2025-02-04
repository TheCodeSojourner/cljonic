#ifndef CLJONIC_CORE_MAXBY_HPP
#define CLJONIC_CORE_MAXBY_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_MaxBy
* The \b MaxBy function uses its first parameter, which must be a \b binary \b predicate that returns \b true if its
* first parameter is less than its second parameter, to return the maximum value of either the values in its \b cljonic
* \b collection second parameter or its second through last parameters. If its \b cljonic \b collection second parameter
* is empty, then \b MaxBy returns the default value of its \b cljonic \b collection second parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto IsALessThanB = [](const int a, const int b) { return a < b; };

    constexpr auto a{Array{11, 14, 13, 14}};
    constexpr auto maxByA{MaxBy(IsALessThanB, a)}; // 14

    constexpr auto b{Array{11}};
    constexpr auto maxByB{MaxBy(IsALessThanB, b)}; // 11

    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    const auto maxByItr{MaxBy(IsALessThanB, itr)};

    constexpr auto maxByRng{MaxBy(IsALessThanB, Range<0>{})};          // 0, the default value of Range<0>
    constexpr auto maxByRpt{MaxBy(IsALessThanB, Repeat<4, int>{11})};  // 11
    constexpr auto maxBySet{MaxBy(IsALessThanB, Set{11, 14, 13, 14})}; // 14
    constexpr auto maxByStr{MaxBy([](const char i, const char j) { return i < j; }, String{"adcb"})}; // 'd'
    constexpr auto maxBy{MaxBy(IsALessThanB, 1, 2, 14, 7, 9)}; // 14

    // Compiler Error: MaxBy's second parameter must be a cljonic collection
    // constexpr auto sb{MaxBy(IsALessThanB, "Hello")};

    // Compiler Error: MaxBy function is not a valid binary predicate for the collection value type
    // constexpr auto sb{MaxBy(IsALessThanB, Array<const char*, 5>{})};

    // Compiler Error: None of MaxBy's second through last parameters can be cljonic collections
    // constexpr auto sb{MaxBy(IsALessThanB, 1, "Hello", Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename T, typename... Ts>
constexpr auto MaxBy(F&& f, const T& t, const Ts&... ts) noexcept
{
    // #lizard forgives -- The complexity and length of this function is acceptable

    if constexpr (sizeof...(Ts) == 0)
    {
        static_assert(IsCljonicCollection<T>, "MaxBy's second parameter must be a cljonic collection");

        static_assert(IsBinaryPredicate<F, typename T::value_type, typename T::value_type>,
                      "MaxBy function is not a valid binary predicate for the collection value type");

        auto result{t.DefaultElement()};
        if (t.Count() > 0)
        {
            auto tBegin{t.begin()};
            auto tEnd{t.end()};
            result = *tBegin++;
            for (auto it{tBegin}; it != tEnd; ++it)
                if (f(result, *it))
                    result = *it;
        }
        return result;
    }
    else
    {
        static_assert(AllNotCljonicCollections<T, Ts...>,
                      "None of MaxBy's second through last parameters can be cljonic collections");

        static_assert(IsBinaryPredicate<F, T, T>,
                      "MaxBy function is not a valid binary predicate for the collection value type");

        // ***** CAUTION: C++ Voodoo Ahead *****
        // This code initializes "result" to the value "t" then applies the predicate "f" to "result" and each element
        // in the parameter pack "ts", and updates "result" if the predicate returns true. The "(void)" cast ensures
        // that the expression is evaluated for its "side effects" only, and the comma operator ensures that this is
        // done for each element in the parameter pack "ts".
        auto result{t};
        (..., (void)(f(result, ts) ? result = ts : result));
        return result;
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_MAXBY_HPP
