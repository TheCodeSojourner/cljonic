#ifndef CLJONIC_CORE_EQUAL_HPP
#define CLJONIC_CORE_EQUAL_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Equal
* The \b Equal function returns true if there is only one argument, or the values of its arguments are equal, else
false.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    const auto a1{Array<int, 10>{1, 2, 3, 4}};
    const auto a2{Array<int, 10>{1, 2, 3, 4}};
    const auto a3{Array{0, 1, 2, 3, 4}};
    const auto a4{Array{0}};
    const auto ra1{Range(1)};
    const auto re1{Repeat(1, 0)};
    const auto s1{Set{1, 2, 3}};
    const auto str1{String{"abc"}};
    const auto str2{String<3>{'a', 'b', 'c'}};
    const auto e0{Equal(1.1)};           // true whenever one parameter is specified, even if it's a floating point
    const auto e1{Equal(a1, a2)};        // true
    const auto e2{Equal(1, 2)};          // false
    const auto e3{Equal(Range())};       // true
    const auto e4{Equal(Range(5), a3)};  // true
    const auto e5{Equal(Range(10), a3)}; // false
    const auto e6{Equal(s1, s1)};        // true
    const auto e7{Equal(str1, str2)};    // true
    const auto e8{Equal(a4, ra1, re1)};  // true
    // const auto e{Equal(1.1, 2.1)};    // error: Floating point types should not be compared for equality
    // const auto e{Equal()};            // error: Must specify at least one parameter
    // const auto e{Equal(1, "Hello")};  // error: Types are not comparable

    return 0;
}
~~~~~
*/
template <typename T, typename... Ts>
auto Equal(const T& t, const Ts&... ts) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    /* An invocation of Equal with only one argument, like Equal(1) or Equal(myArray), returns true because
     * when only one argument is provided to "and" the single argument is trivially equal to itself,
     * and the result is true.
     */
    if constexpr (sizeof...(Ts) <= 0)
    {
        if constexpr (IsCljonicCollection<T>)
            static_assert((not std::floating_point<typename T::value_type>),
                          "cljonic floating point collection value types should not be compared for equality");
        else
            static_assert((not std::floating_point<T>), "Floating point types should not be compared for equality");
        return true;
    }
    else if constexpr (AllCljonicCollections<T, Ts...>)
    {
        static_assert(AllSameCljonicCollectionType<T, Ts...> or AllCljonicArrayRangeOrRepeat<T, Ts...>,
                      "cljonic collection types are not all the same, or all Array, Range or Repeat types");
        static_assert(not AnyFloatingPointValueTypes<T, Ts...>,
                      "cljonic floating point collection value types should not be compared for equality");
        static_assert(AllEqualityComparableValueTypes<T, Ts...>,
                      "cljonic collection value types are not all equality comparable");
        if constexpr (AllCljonicSets<T, Ts...>)
        {
            constexpr auto EqualSets = [&](const auto& c1, const auto& c2)
            {
                using CountType = decltype(c1.Count());
                auto result{c1.Count() == c2.Count()};
                for (CountType i = 0; (result and (i < c1.Count())); ++i)
                    result = c2.Contains(c1[i]);
                return result;
            };
            return (EqualSets(t, ts) and ...);
        }
        else
        {
            constexpr auto EqualCollections = [&](const auto& c1, const auto& c2)
            {
                using CountType = decltype(c1.Count());
                auto result{c1.Count() == c2.Count()};
                for (CountType i = 0; (result and (i < c1.Count())); ++i)
                    result = AreEqual(c1[i], c2[i]);
                return result;
            };
            return (EqualCollections(t, ts) and ...);
        }
    }
    else
    {
        static_assert(not AnyFloatingPointTypes<T, Ts...>, "Floating point types should not be compared for equality");
        static_assert(AllEqualityComparableTypes<T, Ts...>, "Not all types are equality comparable");
        return (AreEqual(t, ts) and ...);
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_EQUAL_HPP
