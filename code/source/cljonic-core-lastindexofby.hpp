#ifndef CLJONIC_CORE_LASTINDEXOFBY_HPP
#define CLJONIC_CORE_LASTINDEXOFBY_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_LastIndexOfBy
* The \b LastIndexOfBy function returns the index of the last value in its second parameter, which must be a \b cljonic
* \b collection, for which its first parameter, which must be a \b binary \b predicate, returns true when called with
* an element of its first parameter and its third parameter. If its first parameter is never called because its second
* parameter is empty, or its first parameter never returns true, \b LastIndexOfBy returns the value
* \b CLJONIC_INVALID_INDEX.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto EBF = [](const int a, const int b) { return a == b; };

    constexpr auto a{Array{11, 14, 13, 14}};
    constexpr auto lastIndexOfByA{LastIndexOfBy(EBF, a, 14)}; // 3

    constexpr auto b{Array{11}};
    constexpr auto lastIndexOfByB{LastIndexOfBy(EBF, b, 11)}; // 0

    constexpr auto lastIndexOfByRng{LastIndexOfBy(EBF, Range<0>{}, 3)};           // CLJONIC_INVALID_INDEX
    constexpr auto lastIndexOfByRpt{LastIndexOfBy(EBF, Repeat<4, int>{11}, 11)};  // 3
    constexpr auto lastIndexOfBySet{LastIndexOfBy(EBF, Set{11, 14, 13, 14}, 13)}; // 2
    constexpr auto lastIndexOfByStr{LastIndexOfBy(EBF, String{"adcb"}, 'c')};     // 2

    constexpr auto lastIndexOfByStrArray{LastIndexOfBy(
        [](const char* a, const char* b) { return std::strcmp(a, b) == 0; }, Array{"zero", "one", "two"}, "one")}; // 1

    // Compiler Error: LastIndexOfBy's second parameter must be a cljonic collection
    // constexpr auto sb{LastIndexOfBy(EBF, "Hello", 3)};

    // Compiler Error: LastIndexOfBy's third parameter is not equality comparable with the cljonic collection values
    // constexpr auto sb{LastIndexOfBy(EBF, a, "Hello")};

    // Compiler Error: LastIndexOfBy's first parameter is not a valid binary predicate for the collection value type
    // constexpr auto sb{LastIndexOfBy(EBF, Array{"one", "two", "three"}, "four")};

    return 0;
}
~~~~~
*/
template <typename F, typename C, typename T>
[[nodiscard]] constexpr auto LastIndexOfBy(F&& f, const C& c, const T& t) noexcept
{
    // #lizard forgives -- The complexity and length of this function is acceptable

    static_assert(IsCljonicCollection<C>, "LastIndexOfBy's second parameter must be a cljonic collection");

    static_assert(std::equality_comparable_with<typename C::value_type, T>,
                  "LastIndexOfBy's third parameter is not equality comparable with the cljonic collection values");

    static_assert(IsBinaryPredicate<F, typename C::value_type, T>,
                  "LastIndexOfBy's first parameter is not a valid binary predicate for the collection value type");

    auto result{CLJONIC_INVALID_INDEX};
    for (SizeType nextIndex{c.Count()}; ((CLJONIC_INVALID_INDEX == result) and (nextIndex > 0)); --nextIndex)
        if (f(c[nextIndex - 1], t))
            result = nextIndex - 1;
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_LASTINDEXOFBY_HPP
