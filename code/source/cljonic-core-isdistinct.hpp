#ifndef CLJONIC_CORE_ISDISTINCT_HPP
#define CLJONIC_CORE_ISDISTINCT_HPP

#include <array>
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_IsDistinct
* The \b IsDistinct function returns true if it is called with two or more paremeters and none of them are equal, else
* false. If it is called with only one parameter the function always returns true.
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
    constexpr auto b3{IsDistinct(a)};               // true whenever there's only two parameters
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

    // Compiler Error: IsDistinct cljonic collection types are not all the same, or all Array, Range or Repeat types
    // constexpr auto b{IsDistinct(a, Set{2, 3, 4})};

    // Compiler Error: IsDistinct should not compare floating point types for equality. Consider using IsDistinctBy to
    //                 override this default.
    // constexpr auto b{IsDistinct(1.1, 1)};

    // Compiler Error: Not all IsDistinct types are equality comparable
    // constexpr auto b{IsDistinct("Hello", 1)};

    return 0;
}
~~~~~
*/
template <typename T, typename... Ts>
[[nodiscard]] constexpr auto IsDistinct(const T& t, const Ts&... ts) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    if constexpr (sizeof...(Ts) == 0)
    {
        return true;
    }
    else
    {
        if constexpr (AllCljonicCollections<T, Ts...>)
        {
            static_assert(not AnyFloatingPointValueTypes<T, Ts...>,
                          "IsDistinct should not compare cljonic floating point collection value types for "
                          "equality. Consider using IsDistinctBy to override this default.");

            static_assert(
                AllSameCljonicCollectionType<T, Ts...> or AllCljonicArrayRangeOrRepeat<T, Ts...>,
                "IsDistinct cljonic collection types are not all the same, or all Array, Range or Repeat types");

            constexpr auto IndexInterfacesEqual = [](const auto& t, const auto& u) noexcept
            {
                if (&t == &u) // if t and u point to the same IndexInterface
                    return true;
                if (t.Count() != u.Count())
                    return false;
                for (SizeType i{0}; i < t.Count(); ++i)
                    if (not t.ElementAtIndexIsEqualToElement(i, u[i]))
                        return false;
                return true;
            };
            using I = const IndexInterface<typename T::value_type>*;
            constexpr auto n{sizeof...(Ts) + 1};
            const auto i{std::array<I, n>{static_cast<I>(&t), static_cast<I>(&ts)...}};
            for (SizeType j{0}; j < n; ++j)
                for (SizeType k{j + 1}; k < n; ++k)
                    if (IndexInterfacesEqual(*i[j], *i[k]))
                        return false;
            return true;
        }
        else
        {
            static_assert(not AnyFloatingPointTypes<T, Ts...>,
                          "IsDistinct should not compare floating point types for equality. Consider using "
                          "IsDistinctBy to override this default.");

            static_assert(AllEqualityComparableTypes<T, Ts...>, "Not all IsDistinct types are equality comparable");

            return (not AreEqual(t, ts) and ...);
        }
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_ISDISTINCT_HPP
