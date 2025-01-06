#ifndef CLJONIC_CORE_SORT_HPP
#define CLJONIC_CORE_SORT_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-core-seq.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Sort
* The \b Sort function uses an \b Insertion \b Sort algorithm to sort its parameter, which must be a \b cljonic
* \b collection, into its result, which is a \b cljonic \b Array with the same \b MaximumCount as its parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    const auto sortA{Sort(a)};

    constexpr auto b{Array{11, 13, 12, 14}};
    const auto sortB{Sort(a)};

    const auto sortRng{Sort(Range<0>{})};

    const auto sortRpt{Sort(Repeat<4, int>{11})};

    const auto sortSet0{Sort(Set{11, 12, 13, 14})};
    const auto sortSet1{Sort(Set{11, 13, 12, 14})};

    const auto sortStr0{Sort(String{"abcd"})};
    const auto sortStr1{Sort(String{"axbycz"})};

    const auto sortCStr0{Sort(Array{"four", "one", "three", "two"})};
    const auto sortCStr1{Sort(Array{"one", "two", "three", "four"})};

    // Compiler Error: Sort's parameter must be a cljonic collection
    // const auto sb{Sort("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Sort(const C& c) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "Sort's parameter must be a cljonic collection");

    // Insertion sort algorithm
    auto result{Seq(c)};
    for (SizeType i{1}; i < result.Count(); ++i)
    {
        auto key = result[i];
        SizeType j = i;
        while ((j > 0) and FirstLessThanSecond(key, result[j - 1]))
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

#endif // CLJONIC_CORE_SORT_HPP
