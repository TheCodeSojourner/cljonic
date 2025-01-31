#ifndef CLJONIC_CORE_FILTER_HPP
#define CLJONIC_CORE_FILTER_HPP

#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_IndexOfBy
* The \b IndexOfBy function calls its first parameter, which must be a \b binary \b predicate, with each element of its
* second parameter, which must be a \b cljonic \b collection, and its third parameter, and if the result is true the
* index of the second parameter element is immediately returned. If the first parameter never returns true then
* \b CljonicInvalidIndex is returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto SameInts = [](const int i, const int j) { return i == j; };

    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto iobA{IndexOfBy(SameInts, a, 2)}; // 2

    constexpr auto itr{Iterate([](const int i) { return 1 + i; }, 0)};
    const auto iobItr{IndexOfBy(SameInts, itr, 4)}; // 4

    constexpr auto rng{Range<10>{}};
    constexpr auto iobRng{IndexOfBy(SameInts, rng, 4)}; // 4

    constexpr auto rpt{Repeat<10, int>{1}};
    constexpr auto iobRpt{IndexOfBy(SameInts, rpt, 9)}; // CljonicInvalidIndex

    constexpr auto s{Set<int, 4>{1, 2, 3, 4, 5, 6}};
    constexpr auto iobS{IndexOfBy(SameInts, s, 4)}; // 3

    constexpr auto iobStr{IndexOfBy(SameInts, String{"Hello"}, 'l')}; // 2

    // Compiler Error: IndexOfBy's function is not a valid binary predicate for the collection value type
    // constexpr auto iob{IndexOfBy(SameInts, Array<const char*, 5>{}, "Hello")};

    // Compiler Error: IndexOfBy's second parameter must be a cljonic collection
    // constexpr auto iob{IndexOfBy(SameInts, "Hello", 'l')};

    // Compiler Error: IndexOfBy's third parameter must be convertible to the collection value type
    // constexpr auto iob{IndexOfBy(SameInts, Range<7>{}, "Hello")};

    return 0;
}
~~~~~
*/
template <typename F, typename C, typename T>
constexpr auto IndexOfBy(F&& f, const C& c, const T& t) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "IndexOfBy's second parameter must be a cljonic collection");

    static_assert(IsBinaryPredicate<std::decay_t<F>, typename C::value_type, typename C::value_type>,
                  "IndexOfBy's function is not a valid binary predicate for the collection value type");

    static_assert(std::convertible_to<T, typename C::value_type>,
                  "IndexOfBy's third parameter must be convertible to the collection value type");

    auto result{CljonicInvalidIndex};
    auto it{c.begin()};
    auto cEnd{c.end()};
    for (SizeType i{0}; ((CljonicInvalidIndex == result) and (it != cEnd)); ++it, ++i)
        if (f(*it, t))
            result = i;
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_FILTER_HPP
