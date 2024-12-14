#ifndef CLJONIC_CORE_EQUAL_HPP
#define CLJONIC_CORE_EQUAL_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Equal
* The \b Equal function returns true if the values of its arguments are equal, else false.
~~~~~{.cpp}
#include <iostream>
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    auto v1{Vector<int, 10>(1, 2, 3, 4)};
    auto v2{Vector<int, 10>(1, 2, 3, 4)};
    auto e0{Equal(v1)};                    // returns true whenever only one parameter is specified
    auto e1{Equal(v1, v2)};                // returns true
    auto e2{Equal(1, 2)};                  // returns false
    // auto e3{Equal(1.1, 2.1)};           // compilation error: Floating point types should not be compared for
equality
    // auto e{Equal()};                    // compilation error: Must specify at least one parameter
    // auto e{Equal(1.1, "Hello")};        // compilation error: Types are not comparable

    return 0;
}
~~~~~
*/
template <typename T, typename... Ts>
auto Equal(const T& t, const Ts&... ts)
{
    /* An invocation of Equal with only one argument, like Equal(1) or Equal(myVector), returns true because
     * when only one argument is provided to "and" the single argument is trivially equal to itself,
     * and the result is true.
     */
    if constexpr (AllCljonicCollections<T, Ts...>)
    {
        static_assert(AllSameCljonicCollectionType<T, Ts...>,
                      "Cljonic collection types are not all the same (e.g., Vector, Set, or String)");
        static_assert(not AnyFloatingPointValueTypes<T, Ts...>,
                      "Cljonic floating point collection value types should not be compared for equality");
        static_assert(AllEqualityComparableValueTypes<T, Ts...>,
                      "Cljonic collection value types are not all equality comparable");
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
