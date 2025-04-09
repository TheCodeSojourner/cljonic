#ifndef CLJONIC_CORE_FILTER_HPP
#define CLJONIC_CORE_FILTER_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_IndexOf
* The \b IndexOf function compares each element of its first parameter, which must be a \b cljonic \b collection, with
* its second parameter, and if they are equal the index of the first parameter element is immediately returned. If none
* of the first parameter elements are equal to the second parameter then \b CljonicInvalidIndex is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto ioA{IndexOf(a, 2)}; // 2

    constexpr auto rng{Range<10>{}};
    constexpr auto ioRng{IndexOf(rng, 4)}; // 4

    constexpr auto rpt{Repeat<10, int>{1}};
    constexpr auto ioRpt{IndexOf(rpt, 9)}; // CljonicInvalidIndex

    constexpr auto s{Set<int, 4>{1, 2, 3, 4, 5, 6}};
    constexpr auto ioS{IndexOf(s, 4)}; // 3

    constexpr auto ioStr{IndexOf(String{"Hello"}, 'l')}; // 2

    constexpr auto ioStrs{IndexOf(Array{"one", "two", "three"}, "two")}; // 1

    // Compiler Error: IndexOf's first parameter must be a cljonic collection
    // constexpr auto io{IndexOf("Hello", 'l')};

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
constexpr auto IndexOf(const C& c, const T& t) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "IndexOf's first parameter must be a cljonic collection");

    static_assert((not std::floating_point<typename C::value_type>) and (not std::floating_point<T>),
                  "IndexOf should not compare floating point types for equality. Consider using IndexOfBy to override "
                  "this default.");

    static_assert(std::convertible_to<T, typename C::value_type>,
                  "IndexOf's second parameter must be convertible to the collection value type");

    auto result{CljonicInvalidIndex};
    for (SizeType i{0}; ((CljonicInvalidIndex == result) and (i < c.Count())); ++i)
        if (AreEqual(c[i], t))
            result = i;
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_FILTER_HPP
