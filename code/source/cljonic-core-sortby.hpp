#ifndef CLJONIC_CORE_SORTBY_HPP
#define CLJONIC_CORE_SORTBY_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-core-seq.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_SortBy
* The \b SortBy function uses its first parameter, which must be a \b binary \b predicate, in an \b Insertion \b Sort
* algorithm to sort its second parameter, which must be a \b cljonic \b collection, into its result, which is a
* \b cljonic \b Array with the same \b MaximumCount as its second parameter.  The first parameter \b binary \b predicate
* must reutrn \b true if its first parameter is less than its second parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto IsALessThanB = [](const int a, const int b) { return a < b; };

    constexpr auto a{Array{11, 12, 13, 14}};
    const auto sortByA{SortBy(IsALessThanB, a)};

    constexpr auto b{Array{11, 13, 12, 14}};
    const auto sortByB{SortBy(IsALessThanB, a)};

    const auto sortByRng{SortBy(IsALessThanB, Range<0>{})};

    const auto sortByRpt{SortBy(IsALessThanB, Repeat<4, int>{11})};

    const auto sortBySet0{SortBy(IsALessThanB, Set{11, 12, 13, 14})};
    const auto sortBySet1{SortBy(IsALessThanB, Set{11, 13, 12, 14})};

    const auto sortByStr0{SortBy([](const char i, const char j) { return i < j; }, String{"abcd"})};
    const auto sortByStr1{SortBy([](const char i, const char j) { return i < j; }, String{"axbycz"})};

    // Compiler Error: SortBy's second parameter must be a cljonic collection
    // const auto sb{SortBy(IsALessThanB, "Hello")};

    // Compiler Error: SortBy's function is not a valid unary predicate for the collection value type
    // const auto sb{SortBy(IsALessThanB, Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
constexpr auto SortBy(F&& f, const C& c) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "SortBy's second parameter must be a cljonic collection");

    static_assert(IsBinaryPredicate<std::decay_t<F>, typename C::value_type, typename C::value_type>,
                  "SortBy's function is not a valid binary predicate for the collection value type");

    // Insertion sort algorithm
    auto result{Seq(c)};
    for (SizeType i{1}; i < result.Count(); ++i)
    {
        auto key = result[i];
        SizeType j = i;
        while ((j > 0) and f(key, result[j - 1]))
        {
            result.MSet(j, result[j - 1]);
            --j;
        }
        result.MSet(j, key);
    }
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_SORTBY_HPP
