#ifndef CLJONIC_CORE_INDEX_OF_BY_HPP
#define CLJONIC_CORE_INDEX_OF_BY_HPP

#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_IndexOfBy
* Starting at \b fromIndex, default zero, the \b IndexOfBy function uses its first parameter, which must be a \b binary
* \b predicate, to equality compare each element of its second parameter, which must be a \b cljonic \b collection other
* than a \b Set, with its third parameter, and returns the index of the first matching element. If none of the second
* parameter elements compared are equal to the third parameter then \b CljonicInvalidIndex is returned. If \b fromIndex
* is greater than the maximum valid index of the second parameter then \b CljonicInvalidIndex is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto SameInts = [](const int i, const int j) { return i == j; };

    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 1, 2, 6, 7, 8, 9}};
    constexpr auto ioA1{IndexOfBy(SameInts, a, 2)};     // 2
    constexpr auto ioA2{IndexOfBy(SameInts, a, 2, 3)};  // 5
    constexpr auto ioA3{IndexOfBy(SameInts, a, 2, 30)}; // CljonicInvalidIndex

    const auto itr{Iterate([](const int i) { return i + 1; }, 10)};
    const auto ioItr{IndexOfBy(SameInts, itr, 13)}; // 3

    constexpr auto rng{Range<10>{}};
    constexpr auto ioRng{IndexOfBy(SameInts, rng, 4)}; // 4

    constexpr auto rpt{Repeat<10, int>{1}};
    constexpr auto ioRpt{IndexOfBy(SameInts, rpt, 9)}; // CljonicInvalidIndex

    constexpr auto ioStr{IndexOfBy(SameInts, String{"Hello"}, 'l')}; // 2

    // Compiler Error: IndexOfBy's first parameter must be a cljonic collection
    // constexpr auto io{IndexOfBy("Hello", 'l')};

    // Compiler Error: IndexOfBy's first parameter must not be a cljonic set
    // constexpr auto io{IndexOfBy(SameInts, Set<int, 4>{1, 2, 3, 4, 5, 6}, 4)};

    // Compiler Error: IndexOfBy's first parameter must not be a cljonic set
    // constexpr auto io{IndexOfBy([](const char* s1, const char* s2) { return true; }, Array{}, 4)};

    // Compiler Error: IndexOfBy's second parameter must be convertible to the collection value type
    // constexpr auto io{IndexOfBy(SameInts, Range<7>{}, "Hello")};

    return 0;
}
~~~~~
*/
template <typename F, typename C, typename T>
constexpr auto IndexOfBy(F&& f, const C& c, const T& t, const SizeType fromIndex) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "IndexOfBy's second parameter must be a cljonic collection");

    static_assert(IsCljonicNonSet<C>, "IndexOfBy's first parameter must not be a cljonic set");

    static_assert(IsBinaryPredicate<std::decay_t<F>, typename C::value_type, typename C::value_type>,
                  "IndexOfBy's function is not a valid binary predicate for the collection value type");

    static_assert(std::convertible_to<T, typename C::value_type>,
                  "IndexOfBy's third parameter must be convertible to the collection value type");

    auto i{SizeType{0}};
    auto cEnd{c.end()};
    for (auto it{c.begin()}; it != cEnd; ++it)
        if ((i >= fromIndex) and (f(*it, t)))
            return i;
        else
            ++i;
    return CljonicInvalidIndex; // LCOV_EXCL_LINE - This line of code may only execute at compile-time
}

template <typename F, typename C, typename T>
constexpr auto IndexOfBy(F&& f, const C& c, const T& t) noexcept
{
    return IndexOfBy(std::forward<F>(f), c, t, 0);
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_INDEX_OF_BY_HPP
