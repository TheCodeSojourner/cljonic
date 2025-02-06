#ifndef CLJONIC_CORE_SORTBY_HPP
#define CLJONIC_CORE_SORTBY_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-core-seq.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_SortBy
* The \b SortBy function uses its first parameter, which must be a \b binary \b predicate that returns \b true if its
* first parameter is less than its second parameter, in an \b Insertion \b Sort algorithm to sort its second parameter,
* which must be a \b cljonic \b collection, into its result, which is a \b cljonic \b Array with the same
* \b MaximumCount as its second parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto IsALessThanB = [](const int a, const int b) { return a < b; };

    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto sortByA{SortBy(IsALessThanB, a)};

    constexpr auto b{Array{11, 13, 12, 14}};
    constexpr auto sortByB{SortBy(IsALessThanB, b)};

    constexpr auto itr{Iterate([](const int i) { return i - 1; }, 10000)};
    const auto sortByItr{SortBy(IsALessThanB, itr)};

    constexpr auto sortByRng{SortBy(IsALessThanB, Range<0>{})};

    constexpr auto sortByRpt{SortBy(IsALessThanB, Repeat<4, int>{11})};

    constexpr auto sortBySet0{SortBy(IsALessThanB, Set{11, 12, 13, 14})};
    constexpr auto sortBySet1{SortBy(IsALessThanB, Set{11, 13, 12, 14})};

    constexpr auto sortByStr0{SortBy([](const char i, const char j) { return i < j; }, String{"abcd"})};
    constexpr auto sortByStr1{SortBy([](const char i, const char j) { return i < j; }, String{"axbycz"})};

    // Compiler Error: SortBy's second parameter must be a cljonic collection
    // constexpr auto sb{SortBy(IsALessThanB, "Hello")};

    // Compiler Error: SortBy's function is not a valid binary predicate for the collection value type
    // constexpr auto sb{SortBy(IsALessThanB, Array<const char*, 5>{})};

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

    using ResultType = typename C::value_type;
    auto result{Array<ResultType, C::MaximumCount()>{}};
    for (const ResultType& v : c)
        result.MAppend(v);
    result.MSortBy(std::forward<F>(f));
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_SORTBY_HPP
