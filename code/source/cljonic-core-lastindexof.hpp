#ifndef CLJONIC_CORE_LASTINDEXOF_HPP
#define CLJONIC_CORE_LASTINDEXOF_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_LastIndexOf
* The \b LastIndexOf function returns the index of the last value in its first parameter, which must be a \b cljonic
* \b collection, that equals the value of its second parameter. If the first parameter has no value equal to the second
* parameter then \b LastIndexOf returns the value \b CljonicInvalidIndex.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 14, 13, 14}};
    constexpr auto lastIndexOfA{LastIndexOf(a, 14)}; // 3

    constexpr auto b{Array{11}};
    constexpr auto lastIndexOfB{LastIndexOf(b, 11)}; // 0

    constexpr auto lastIndexOfRng{LastIndexOf(Range<0>{}, 3)};                           // CljonicInvalidIndex
    constexpr auto lastIndexOfRpt{LastIndexOf(Repeat<4, int>{11}, 11)};                  // 3
    constexpr auto lastIndexOfSet{LastIndexOf(Set{11, 14, 13, 14}, 13)};                 // 2
    constexpr auto lastIndexOfStr{LastIndexOf(String{"adcb"}, 'c')};                     // 2
    constexpr auto lastIndexOfStrArray{LastIndexOf(Array{"zero", "one", "two"}, "one")}; // 1

    // Compiler Error: LastIndexOf's first parameter must be a cljonic collection
    // constexpr auto sb{LastIndexOf("Hello", 'e')};

    // Compiler Error: LastIndexOf's second parameter is not equality comparable with the cljonic collection values
    // constexpr auto sb{LastIndexOf(Array<const char*, 5>{}, 3)};

    // Compiler Error: LastIndexOf should not compare floating point types for equality
    // constexpr auto sb{LastIndexOf(Array{1.1, 2.2, 3.3}, 3.3)};
    // constexpr auto sb{LastIndexOf(Array{1.1, 2.2, 3.3}, 3)};
    // constexpr auto sb{LastIndexOf(Array{1, 2, 3}, 3.3)};

    return 0;
}
~~~~~
*/
template <typename C, typename T>
constexpr auto LastIndexOf(const C& c, const T& t) noexcept
{
    // #lizard forgives -- The complexity and length of this function is acceptable

    static_assert(IsCljonicCollection<C>, "LastIndexOf's first parameter must be a cljonic collection");

    static_assert(std::equality_comparable_with<typename C::value_type, T>,
                  "LastIndexOf's second parameter is not equality comparable with the cljonic collection values");

    static_assert(not std::floating_point<typename C::value_type>,
                  "LastIndexOf should not compare floating point types for equality. Consider using LastIndexOfBy to "
                  "override this default.");

    static_assert(not std::floating_point<T>,
                  "LastIndexOf should not compare floating point types for equality. Consider using LastIndexOfBy to "
                  "override this default.");

    auto result{CljonicInvalidIndex};
    for (SizeType nextIndex{c.Count()}; ((CljonicInvalidIndex == result) and (nextIndex > 0)); --nextIndex)
        if (AreEqual(c[nextIndex - 1], t))
            result = nextIndex - 1;
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_LASTINDEXOF_HPP
