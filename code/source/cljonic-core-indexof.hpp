#ifndef CLJONIC_CORE_FILTER_HPP
#define CLJONIC_CORE_FILTER_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_IndexOf
* Starting at \b fromIndex, default zero, the \b IndexOf function equality compares each element of its first
* parameter, which must be a \b cljonic \b collection other than a \b Set, with its second parameter, and
* returns the index of the first matching element. If none of the first parameter elements compared are equal to the
* second parameter then \b CljonicInvalidIndex is returned. If \b fromIndex is greater than the maximum valid index
* of the first parameter then \b CljonicInvalidIndex is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 1, 2, 6, 7, 8, 9}};
    constexpr auto ioA1{IndexOf(a, 2)};     // 2
    constexpr auto ioA2{IndexOf(a, 2, 3)};  // 5
    constexpr auto ioA3{IndexOf(a, 2, 30)}; // CljonicInvalidIndex

    const auto itr{Iterate([](const int i) { return i + 1; }, 10)};
    const auto ioItr{IndexOf(itr, 13)}; // 3

    constexpr auto rng{Range<10>{}};
    constexpr auto ioRng{IndexOf(rng, 4)}; // 4

    constexpr auto rpt{Repeat<10, int>{1}};
    constexpr auto ioRpt{IndexOf(rpt, 9)}; // CljonicInvalidIndex

    constexpr auto ioStr{IndexOf(String{"Hello"}, 'l')}; // 2

    constexpr auto ioStrs{IndexOf(Array{"one", "two", "three"}, "two")}; // 1

    // Compiler Error: IndexOf's first parameter must be a cljonic collection
    // constexpr auto io{IndexOf("Hello", 'l')};

    // Compiler Error: IndexOf's first parameter must not be a cljonic set
    // constexpr auto io{IndexOf(Set<int, 4>{1, 2, 3, 4, 5, 6}, 4)};

    // Compiler Error: IndexOf should not compare floating point types for equality. Consider using IndexOfBy to
    //                 override this default.
    // constexpr auto io{IndexOf(Array{1.1, 2.2}, 3.3)};
    // constexpr auto io{IndexOf(Array{1.1, 2.2}, 3)};
    // constexpr auto io{IndexOf(Array{1, 2}, 3.3)};

    // Compiler Error: IndexOf's second parameter must be convertible to the collection value type
    // constexpr auto io{IndexOf(Range<7>{}, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C, typename T>
constexpr auto IndexOf(const C& c, const T& t, const SizeType fromIndex) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "IndexOf's first parameter must be a cljonic collection");

    static_assert(IsCljonicNonSet<C>, "IndexOf's first parameter must not be a cljonic set");

    static_assert((not std::floating_point<typename C::value_type>) and (not std::floating_point<T>),
                  "IndexOf should not compare floating point types for equality. Consider using IndexOfBy to override "
                  "this default.");

    static_assert(std::convertible_to<T, typename C::value_type>,
                  "IndexOf's second parameter must be convertible to the collection value type");

    auto i{SizeType{0}};
    auto cEnd{c.end()};
    for (auto it{c.begin()}; it != cEnd; ++it)
        if ((i >= fromIndex) and (AreEqualValues(*it, t)))
            return i;
        else
            ++i;
    return CljonicInvalidIndex; // LCOV_EXCL_LINE - This line of code may only execute at compile-time
}

template <typename C, typename T>
constexpr auto IndexOf(const C& c, const T& t) noexcept
{
    return IndexOf(c, t, 0);
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_FILTER_HPP
