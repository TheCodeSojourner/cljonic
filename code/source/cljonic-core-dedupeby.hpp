#ifndef CLJONIC_CORE_DEDUPEBY_HPP
#define CLJONIC_CORE_DEDUPEBY_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_DedupeBy
* The \b DedupeBy function returns a \b cljonic \b Array of elements from a \b cljonic \b collection such that
* consecutive duplicates, as defined by its \b binary \b predicate first argument, are removed.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto EqualInts = [](const int i, const int j) { return i == j; };

    constexpr auto a0{Array<int, 5>{}};
    constexpr auto d0{DedupeBy(EqualInts, a0)};

    constexpr auto a1{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto d1{DedupeBy(EqualInts, a1)};

    constexpr auto a2{Array{0, 0, 0, 0}};
    constexpr auto d2{DedupeBy(EqualInts, a2)};

    constexpr auto a3{Array{0, 0, 0, 0, 1, 2, 3, 4, 5}};
    constexpr auto d3{DedupeBy(EqualInts, a3)};

    constexpr auto a4{Array{0, 1, 2, 3, 4, 5, 5, 5, 5}};
    constexpr auto d4{DedupeBy(EqualInts, a4)};

    constexpr auto a5{Array{0, 1, 2, 3, 3, 3, 3, 4, 5}};
    constexpr auto d5{DedupeBy(EqualInts, a5)};

    constexpr auto a6{Array{0, 1, 1, 2, 3, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9}};
    constexpr auto d6{DedupeBy(EqualInts, a6)};

    // Compiler Error: DedupeBy's second parameter must be a cljonic collection
    // constexpr auto d{DedupeBy(EqualInts, 1)};

    // Compiler Error: DedupBy's function is not a valid binary predicate for the collection value type
    // constexpr auto d{DedupeBy([](const char* s1, const char* s2) { return true; }, Array{1.1, 1.1})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
constexpr auto DedupeBy(F&& f, const C& c) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "DedupeBy's second parameter must be a cljonic collection");

    static_assert(IsBinaryPredicate<std::decay_t<F>, typename C::value_type, typename C::value_type>,
                  "DedupeBy's function is not a valid binary predicate for the collection value type");

    using ValueType = typename C::value_type;

    constexpr auto IndexOfNextElementNotEqualToCurrentElement =
        [](const F& fn, const C& collection, const SizeType currentIndex)
    {
        auto currentElement{collection[currentIndex]};
        auto i{currentIndex + 1};
        while ((i < collection.Count()) and fn(collection[i], currentElement))
            ++i;
        return i;
    };

    auto result{Array<ValueType, c.MaximumCount()>{}};
    for (SizeType i{0}; i < c.Count();)
    {
        result.MConj(c[i]);
        i = IndexOfNextElementNotEqualToCurrentElement(f, c, i);
    }
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_DEDUPEBY_HPP
