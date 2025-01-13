#ifndef CLJONIC_CORE_EQUAL_HPP
#define CLJONIC_CORE_EQUAL_HPP

#include "cljonic-core-equalby.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Equal
* The \b Equal function returns true if it is called with two or more equal arguments, else false. If it is called with
* only one argument the function always returns true.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a1{Array<int, 10>{1, 2, 3, 4}};
    constexpr auto a2{Array<int, 10>{1, 2, 3, 4}};
    constexpr auto a3{Array{0, 1, 2, 3, 4}};
    constexpr auto a4{Array{0}};
    constexpr auto ra1{Range<1>{}};
    constexpr auto re1{Repeat<1, int>{0}};
    constexpr auto s1{Set{1, 2, 3}};
    constexpr auto str1{String{"abc"}};
    constexpr auto str2{String<3>{'a', 'b', 'c'}};
    constexpr auto e0{Equal(a1, a2)};          // true
    constexpr auto e1{Equal(1, 2)};            // false
    constexpr auto e2{Equal(Range{})};         // true
    constexpr auto e3{Equal(Range<5>{}, a3)};  // true
    constexpr auto e4{Equal(Range<10>{}, a3)}; // false
    constexpr auto e5{Equal(s1, s1)};          // true
    constexpr auto e6{Equal(str1, str2)};      // true
    constexpr auto e7{Equal(a4, ra1, re1)};    // true

    // Compiler Error: Equal should not compare floating point types for equality. Consider using EqualBy to override
    //                 this default.
    // constexpr auto e{Equal(1.1)};

    // Compiler Error: Equal should not compare floating point types for equality. Consider using EqualBy to override
    //                 this default.
    // constexpr auto e{Equal(1.1, 2.1)};

    // Compiler Error: no matching function for call to ‘Equal()’
    // constexpr auto e{Equal()}; // Compiler Error: Must specify at least one parameter

    // Compiler Error: Not all Equal types are equality comparable
    // constexpr auto e{Equal(1, "Hello")};

    return 0;
}
~~~~~
*/
template <typename T, typename... Ts>
constexpr auto Equal(const T& t, const Ts&... ts) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    if constexpr (sizeof...(Ts) == 0)
    {
        if constexpr (IsCljonicCollection<T>)
            static_assert((not std::floating_point<typename T::value_type>),
                          "Equal should not compare cljonic floating point collection value types for equality. "
                          "Consider using EqualBy to override this default.");
        else
            static_assert((not std::floating_point<T>),
                          "Equal should not compare floating point types for equality. Consider using EqualBy to "
                          "override this default.");
        return true;
    }
    else if constexpr (AllCljonicCollections<T, Ts...>)
    {
        static_assert(not AnyFloatingPointValueTypes<T, Ts...>,
                      "Equal should not compare cljonic floating point collection value types for equality. Consider "
                      "using EqualBy to override this default.");

        static_assert(AllSameCljonicCollectionType<T, Ts...> or AllCljonicArrayRangeOrRepeat<T, Ts...>,
                      "Equal cljonic collection types are not all the same, or all Array, Range or Repeat types");

        return (AreEqual(t, ts) and ...);
    }
    else
    {
        static_assert(not AnyFloatingPointTypes<T, Ts...>,
                      "Equal should not compare floating point types for equality. Consider using EqualBy to override "
                      "this default.");

        static_assert(AllEqualityComparableTypes<T, Ts...>, "Not all Equal types are equality comparable");

        return (AreEqual(t, ts) and ...);
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_EQUAL_HPP
