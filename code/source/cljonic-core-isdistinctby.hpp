#ifndef CLJONIC_CORE_ISDISTINCTBY_HPP
#define CLJONIC_CORE_ISDISTINCTBY_HPP

#include <array>

namespace cljonic
{

namespace core
{

/** \anchor Core_IsDistinctBy
* The \b IsDistinctBy function returns true if it is called with three or more paremeters and none of its second through
* last parameters are equal, as defined by its first parameter that must be a \b binary \b predicate, else false. If it
* is called with only two parameters the function always returns true, and the first parameter is never called.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto EBF = [](const int a, const int b) { return a == b; };
    constexpr auto IDBFS = [](const char* a, const char* b) { return 0 == std::strcmp(a, b); };
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
    constexpr auto b0{IsDistinctBy(EBF, 1)};                 // true whenever there's only two parameters
    constexpr auto b1{IsDistinctBy(EBF, 1, 1)};              // false
    constexpr auto b2{IsDistinctBy(EBF, 1, 2)};              // true
    constexpr auto b3{IsDistinctBy(EBF, a)};                 // true whenever there's only two parameters
    constexpr auto b4{IsDistinctBy(EBF, a, r41)};            // false
    constexpr auto b5{IsDistinctBy(EBF, a, a13)};            // true
    constexpr auto b6{IsDistinctBy(EBF, r14, a13)};          // false
    constexpr auto b7{IsDistinctBy(EBF, r04, a)};            // true
    constexpr auto b8{IsDistinctBy(EBF, s, s)};              // false
    constexpr auto b9{IsDistinctBy(EBF, s, s1)};             // false
    constexpr auto b10{IsDistinctBy(EBF, s, s2)};            // true
    constexpr auto b11{IsDistinctBy(EBF, str, str)};         // false
    constexpr auto b12{IsDistinctBy(EBF, str, str1)};        // true
    constexpr auto b13{IsDistinctBy(IDBFS, "str", "str")};   // false
    constexpr auto b14{IsDistinctBy(IDBFS, "str1", "str2")}; // true

    // Compiler Error: no matching function for call
    // constexpr auto b{IsDistinctBy(EBF)}; // Compiler Error: Must specify at least two parameters

    // Compiler Error: IsDistinctBy cljonic collection types are not all the same, or all Array, Range or Repeat types
    // constexpr auto b{IsDistinctBy(EBF, a, Set{2, 3, 4})};

    // Compiler Error: IsDistinctBy function is not a valid binary predicate for all cljonic collection value types
    // constexpr auto b{IsDistinctBy([](){ return true; }, a, a)};

    // Compiler Error: IsDistinctBy function is not a valid binary predicate for all parameters
    // constexpr auto b{IsDistinctBy([]() { return true; }, 1, 2)};

    return 0;
}
~~~~~
*/
template <typename F, typename T, typename... Ts>
[[nodiscard]] constexpr auto IsDistinctBy(F&& f, const T& t, const Ts&... ts) noexcept
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
            static_assert(
                AllSameCljonicCollectionType<T, Ts...> or AllCljonicArrayRangeOrRepeat<T, Ts...>,
                "IsDistinctBy cljonic collection types are not all the same, or all Array, Range or Repeat types");

            static_assert(
                IsBinaryPredicateForAllCljonicCollections<std::decay_t<F>, T, Ts...>,
                "IsDistinctBy function is not a valid binary predicate for all cljonic collection value types");

            constexpr auto IndexInterfacesEqualBy = [](auto&& f, const auto& t, const auto& u) noexcept
            {
                if (&t == &u)
                    return true;
                if (t.Count() != u.Count())
                    return false;
                if constexpr (IsCljonicSet<T>)
                {
                    constexpr auto ContainsBy = [](auto&& f, const auto& setIndex, const auto& element) noexcept
                    {
                        auto result{false};
                        for (SizeType i{0}; ((not result) and (i < setIndex.Count())); ++i)
                            result = f(setIndex[i], element);
                        return result;
                    };
                    for (SizeType i{0}; i < t.Count(); ++i)
                        if (not ContainsBy(f, t, u[i]))
                            return false;
                    return true;
                }
                else
                {
                    for (SizeType i{0}; i < t.Count(); ++i)
                        if (not f(t[i], u[i]))
                            return false;
                    return true;
                }
            };
            using I = const IndexInterface<typename T::value_type>*;
            constexpr auto n{sizeof...(Ts) + 1};
            const auto i{std::array<I, n>{static_cast<I>(&t), static_cast<I>(&ts)...}};
            for (SizeType j{0}; j < n; ++j)
                for (SizeType k{j + 1}; k < n; ++k)
                    if (IndexInterfacesEqualBy(std::forward<F>(f), *i[j], *i[k]))
                        return false;
            return true;
        }
        else
        {
            static_assert(IsBinaryPredicateForAll<std::decay_t<F>, T, Ts...>,
                          "IsDistinctBy function is not a valid binary predicate for all parameters");

            return ((f(t, ts) == false) and ...) and IsDistinctBy(std::forward<F>(f), ts...);
        }
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_ISDISTINCTBY_HPP
