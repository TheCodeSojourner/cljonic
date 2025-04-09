#ifndef CLJONIC_CORE_ISDISTINCTBY_HPP
#define CLJONIC_CORE_ISDISTINCTBY_HPP

#include <utility>
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

template <typename F, typename T, typename... Ts>
constexpr auto InnerIsDistinctBy(F&& f, const T& t, const Ts&... ts) noexcept
{
    if constexpr (sizeof...(Ts) == 0)
    {
        return true;
    }
    else
    {
        return ((not AreEqualValuesBy(std::forward<F>(f), t, ts)) and ... and
                (InnerIsDistinctBy(std::forward<F>(f), ts...)));
    }
}

/** \anchor Core_IsDistinctBy
* The \b IsDistinctBy function's first parameter, let's call it \b BPRED, must be a \b binary \b predicate that returns
* true if its two parameters are equal. If \b IsDistinctBy is called with three or more paremeters, and no two of the
* second through last parameters are equal (as defined by calling \b BPRED), the function returns true, else false. If
* \b IsDistinctBy is called with only two parameters, and the second parameter is a \b cljonic \b collection, the
* function returns true if no two elements of the \b cljonic \b collection are equal (as defined by calling \b BPRED),
* else false. If \b IsDistinctBy is called with only two parameters, and the second parameter is not a \b cljonic
* \b collection, the function always returns true, and \b BPRED is never called. If an attempt is made to call \b BPRED,
* and it is determined not to be a valid binary predicate for its two parameters, a default return value of \b false is
* returned, indicating that the two parameters are \b not \b equal.
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
    constexpr auto b0{IsDistinctBy(EBF, 1)};                 // true
    constexpr auto b1{IsDistinctBy(EBF, 1, 1)};              // false
    constexpr auto b2{IsDistinctBy(EBF, 1, 2)};              // true
    constexpr auto b3{IsDistinctBy(EBF, a)};                 // false
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

    // No Compiler Error: IsDistinctBy function is not a valid binary predicate for cljonic collection value type
    // constexpr auto b{IsDistinctBy([]() { return true; }, a)};

    return 0;
}
~~~~~
*/
template <typename F, typename T, typename... Ts>
constexpr auto IsDistinctBy(F&& f, const T& t, const Ts&... ts) noexcept
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
            static_assert(IsBinaryPredicate<std::decay_t<F>, typename T::value_type, typename T::value_type>,
                          "IsDistinctBy function is not a valid binary predicate for cljonic collection value type");

            return AreUniqueValuesBy<F, decltype(t.begin()), T::MaximumCount()>(std::forward<F>(f), t.begin(), t.end());
        }
    }
    else
    {
        return InnerIsDistinctBy(std::forward<F>(f), t, ts...);
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_ISDISTINCTBY_HPP
