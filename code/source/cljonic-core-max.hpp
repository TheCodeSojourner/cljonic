#ifndef CLJONIC_CORE_MAX_HPP
#define CLJONIC_CORE_MAX_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Max
* The \b Max function returns the maximum value of either the values in its only \b cljonic \b collection parameter or
* all of its parameters. If its only \b cljonic \b collection parameter is empty, then \b Max returns the default value
* of its only \b cljonic \b collection parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 14, 13, 14}};
    constexpr auto maxA{Max(a)}; // 14

    constexpr auto b{Array{11}};
    constexpr auto maxB{Max(b)}; // 11

    constexpr auto maxRng{Max(Range<0>{})};          // 0, the default value of Range<0>
    constexpr auto maxRpt{Max(Repeat<4, int>{11})};  // 11
    constexpr auto maxSet{Max(Set{11, 14, 13, 14})}; // 14
    constexpr auto maxStr{Max(String{"adcb"})}; // 'd'
    constexpr auto max{Max(1, 2, 14, 7, 9)}; // 14

    // Compiler Error: Max's parameter must be a cljonic collection
    // constexpr auto sb{Max("Hello")};

    // Compiler Error: None of Max's parameters can be cljonic collections
    // constexpr auto sb{Max(1, "Hello", Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename T, typename... Ts>
constexpr auto Max(const T& t, const Ts... ts) noexcept
{
    // #lizard forgives -- The complexity and length of this function is acceptable

    if constexpr (sizeof...(Ts) == 0)
    {
        static_assert(IsCljonicCollection<T>, "Max's parameter must be a cljonic collection");

        auto result{t.DefaultElement()};
        if (t.Count() > 0)
        {
            result = t[0];
            for (SizeType i{1}; i < t.Count(); ++i)
                if (result < t[i])
                    result = t[i];
        }
        return result;
    }
    else
    {
        static_assert(AllNotCljonicCollections<T, Ts...>, "None of Max's parameters can be cljonic collections");

        // ***** CAUTION: C++ Voodoo Ahead *****
        // This code initializes "result" to the value "t" then tests "result" to see if it is less than each element
        // in the parameter pack "ts", and updates "result" if is less than the element. The "(void)" cast ensures
        // that the expression is evaluated for its "side effects" only, and the comma operator ensures that this is
        // done for each element in the parameter pack "ts".
        auto result{t};
        (..., (void)((result < ts) ? result = ts : result));
        return result;
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_MAX_HPP
