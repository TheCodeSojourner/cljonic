#ifndef CLJONIC_CORE_EQUALBY_HPP
#define CLJONIC_CORE_EQUALBY_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_EqualBy
* The \b EqualBy function returns true if it is called with three or more arguments and the second through last
* arguments are equal as defined by the first argument, which must be a binary predicate, else false. If it is called
* with only two arguments the function always returns true, and the first argument is never called.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto EBF = [](const int a, const int b){ return a == b; };
    const auto a{Array{1, 1, 1, 1}};
    const auto a13{Array{1, 2, 3}};
    const auto r14{Range(1, 4)};
    const auto r04{Range(4)};
    const auto r{Repeat(4, 1)};
    const auto b0{EqualBy(EBF, 1.1)};      // true whenever one parameter is specified, even if it's a floating point
    const auto b1{EqualBy(EBF, 1, 1)};     // true
    const auto b2{EqualBy(EBF, 1, 2)};     // false
    const auto b3{EqualBy(EBF, a)};        // true whenever one parameter is specified
    const auto b4{EqualBy(EBF, a, r)};     // true
    const auto b5{EqualBy(EBF, a, a13)};   // false
    const auto b6{EqualBy(EBF, a, a13)};   // false
    const auto b7{EqualBy(EBF, r14, a13)}; // true
    const auto b8{EqualBy(EBF, r04, a)};   // false

    return 0;
}
~~~~~
*/
template <typename F, typename T, typename... Ts>
constexpr auto EqualBy(const F& f, const T& t, const Ts&... ts) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    /* An invocation of EqualBy with only one argument, like EqualBy(1) or EqualBy(myArray), returns true because
     * when only one argument is provided to "and" the single argument is trivially equal to itself,
     * and the result is true.
     */
    if constexpr (sizeof...(Ts) <= 0)
    {
        if constexpr (IsCljonicCollection<T>)
            static_assert((not std::floating_point<typename T::value_type>),
                          "Equal(By) should not compare cljonic floating point collection value types for equality");
        else
            static_assert((not std::floating_point<T>),
                          "Equal(By) should not compare floating point types for equality");
        return true;
    }
    else if constexpr (AllCljonicCollections<T, Ts...>)
    {
        static_assert(AllSameCljonicCollectionType<T, Ts...> or AllCljonicArrayRangeOrRepeat<T, Ts...>,
                      "Equal(By) cljonic collection types are not all the same, or all Array, Range or Repeat types");

        static_assert(not AnyFloatingPointValueTypes<T, Ts...>,
                      "Equal(By) should not compare cljonic floating point collection value types for equality");

        static_assert(IsBinaryPredicateForAllCljonicCollections<F, T, Ts...>,
                      "Equal(By) function is not a valid binary predicate for all cljonic collection value types");

        if constexpr (sizeof...(Ts) <= 0)
        {
            return true;
        }
        else if constexpr (AllCljonicSets<T, Ts...>)
        {
            auto EqualSets = [&](const auto& c1, const auto& c2)
            {
                auto result{c1.Count() == c2.Count()};
                for (SizeType i{0}; (result and (i < c1.Count())); ++i)
                    result = c2.ContainsBy(f, c1[i]);
                return result;
            };
            return (EqualSets(t, ts) and ...);
        }
        else
        {
            auto EqualCollections = [&](const auto& c1, const auto& c2)
            {
                auto result{c1.Count() == c2.Count()};
                for (SizeType i{0}; (result and (i < c1.Count())); ++i)
                    result = AreEqualBy(f, c1[i], c2[i]);
                return result;
            };
            return (EqualCollections(t, ts) and ...);
        }
    }
    else
    {
        static_assert(not AnyFloatingPointTypes<T, Ts...>,
                      "Equal(By) should not compare floating point types for equality");

        static_assert(AllEqualityComparableTypes<T, Ts...>, "Not all Equal(By) types are equality comparable");

        static_assert(IsBinaryPredicateForAll<F, T, Ts...>,
                      "Equal(By) function is not a valid binary predicate for all parameters");

        auto EqualParameters = [&](const auto& p1, const auto& p2) { return AreEqualBy(f, p1, p2); };
        return (EqualParameters(t, ts) and ...);
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_EQUALBY_HPP
