#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-sortby.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("SortBy", "[CljonicCoreSortBy]")
{
    constexpr auto IsALessThanB = [](const int a, const int b) { return a < b; };

    constexpr auto a{Array{11, 12, 13, 14}};
    CHECK(Equal(Array{11, 12, 13, 14}, SortBy(IsALessThanB, a)));

    constexpr auto b{Array{11, 13, 12, 14}};
    CHECK(Equal(Array{11, 12, 13, 14}, SortBy(IsALessThanB, b)));

    constexpr auto itr{Iterate([](const int i) { return i - 1; }, 10000)};
    const auto sortByItr{SortBy(IsALessThanB, itr)};
    CHECK(9001 == *sortByItr.begin());
    CHECK(10000 == *(sortByItr.end() - 1));

    CHECK(Equal(Array<int, 0>{}, SortBy(IsALessThanB, Range<0>{})));

    CHECK(Equal(Array{11, 11, 11, 11}, SortBy(IsALessThanB, Repeat<4, int>{11})));

    CHECK(Equal(Array{11, 12, 13, 14}, SortBy(IsALessThanB, Set{11, 12, 13, 14})));
    CHECK(Equal(Array{11, 12, 13, 14}, SortBy(IsALessThanB, Set{11, 13, 12, 14})));

    CHECK(Equal(Array{'a', 'b', 'c', 'd'}, SortBy([](const char i, const char j) { return i < j; }, String{"abcd"})));
    CHECK(Equal(Array{'a', 'b', 'c', 'x', 'y', 'z'},
                SortBy([](const char i, const char j) { return i < j; }, String{"axbycz"})));
}
