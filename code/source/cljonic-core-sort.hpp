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
    constexpr auto sortA{Sort(a)};

    constexpr auto b{Array{11, 13, 12, 14}};
    constexpr auto sortB{Sort(b)};

    constexpr auto itr{Iterate([](const int i) { return 10000 - i; }, 1)};
    const auto sortItr{Sort(itr)};

    constexpr auto sortRng{Sort(Range<0>{})};

    constexpr auto sortRpt{Sort(Repeat<4, int>{11})};

    constexpr auto sortSet0{Sort(Set{11, 12, 13, 14})};
    constexpr auto sortSet1{Sort(Set{11, 13, 12, 14})};

    constexpr auto sortStr0{Sort(String{"abcd"})};
    constexpr auto sortStr1{Sort(String{"axbycz"})};

    constexpr auto sortCStr0{Sort(Array{"four", "one", "three", "two"})};
    constexpr auto sortCStr1{Sort(Array{"one", "two", "three", "four"})};

    // Compiler Error: Sort's parameter must be a cljonic collection
    // constexpr auto sb{Sort("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Sort(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Sort's parameter must be a cljonic collection");

    using ResultType = typename C::value_type;
    auto result{Array<ResultType, C::MaximumCount()>{}};
    for (const ResultType& v : c)
        result.MAppend(v);
    result.MSort();
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_SORT_HPP
