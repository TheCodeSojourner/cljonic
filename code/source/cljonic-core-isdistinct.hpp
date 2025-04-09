#ifndef CLJONIC_CORE_ISDISTINCT_HPP
#define CLJONIC_CORE_ISDISTINCT_HPP

#include <array>
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

template <typename T, typename... Ts>
constexpr auto InnerIsDistinct(const T& t, const Ts&... ts) noexcept
{
    if constexpr (sizeof...(Ts) == 0)
    {
        return true;
    }
    else
    {
        return ((not AreEqualValues(t, ts)) and ... and (InnerIsDistinct(ts...)));
    }
}

/** \anchor Core_IsDistinct
* The \b IsDistinct function returns true if it is called with two or more paremeters and no two of them are equal, else
* false. If it is called with one \b cljonic \b collection parameter the function returns true if no two elements of the
* \b cljonic \b collection are equal. If it is called with one parameter that is not a \b cljonic \b collection the
* function returns true.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{1, 1, 1, 1}};
    constexpr auto a13{Array{1, 2, 3}};
    constexpr auto r14{Range<1, 4>{}};
    constexpr auto r04{Range<4>{}};
    constexpr auto r41{Repeat<4, int>{1}};
    constexpr auto s{Set{1, 3, 2}};
    constexpr auto s1{Set{3, 1, 2}};
    constexpr auto s2{Set{1, 4, 2}};
    constexpr auto str{String{"abc"}};
    constexpr auto str1{String{"def"}};
    constexpr auto b0{IsDistinct(1)};               // true whenever there's only two parameters
    constexpr auto b1{IsDistinct(1, 1)};            // false
    constexpr auto b2{IsDistinct(1, 2)};            // true
    constexpr auto b3{IsDistinct(a)};               // false
    constexpr auto b4{IsDistinct(a, r41)};          // false
    constexpr auto b5{IsDistinct(a, a13)};          // true
    constexpr auto b6{IsDistinct(r14, a13)};        // false
    constexpr auto b7{IsDistinct(r04, a)};          // true
    constexpr auto b8{IsDistinct(s, s)};            // false
    constexpr auto b9{IsDistinct(s, s1)};           // false
    constexpr auto b10{IsDistinct(s, s2)};          // true
    constexpr auto b11{IsDistinct(str, str)};       // false
    constexpr auto b12{IsDistinct(str, str1)};      // true
    constexpr auto b13{IsDistinct("str", "str")};   // false
    constexpr auto b14{IsDistinct("str1", "str2")}; // true

    // Compiler Error: IsDistinct should not compare cljonic floating point collection value types for equality.
    //                 Consider using IsDistinctBy to override this default.
    // constexpr auto b{IsDistinct(Array{1.1, 1.2}, a)};

    // Compiler Error: IsDistinct should not compare floating point types for equality. Consider using IsDistinctBy to
    //                 override this default.
    // constexpr auto b{IsDistinct(1.1, 1)};

    return 0;
}
~~~~~
*/
template <typename T, typename... Ts>
constexpr auto IsDistinct(const T& t, const Ts&... ts) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    if constexpr (sizeof...(Ts) == 0)
    {
        if constexpr (IsNotCljonicCollection<T> or IsCljonicSet<T>)
        {
            return true;
        }
        else
        {
            static_assert(not std::floating_point<typename T::value_type>,
                          "IsDistinct should not compare cljonic floating point collection value types for equality. "
                          "Consider using IsDistinctBy to override this default.");

            return AreUniqueValues<decltype(t.begin()), T::MaximumCount()>(t.begin(), t.end());
        }
    }
    else
    {
        static_assert(not AnyFloatingPointOrFloatingPointValueType<T, Ts...>,
                      "IsDistinct should not compare floating point types for equality. Consider using IsDistinctBy to "
                      "override this default.");

        return InnerIsDistinct(t, ts...);
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_ISDISTINCT_HPP
