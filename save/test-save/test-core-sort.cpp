#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-sort.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Sort", "[CljonicCoreSort]")
{
    constexpr auto a{Array{11, 12, 13, 14}};
    CHECK(Equal(Array{11, 12, 13, 14}, Sort(a)));

    constexpr auto b{Array{11, 13, 12, 14}};
    CHECK(Equal(Array{11, 12, 13, 14}, Sort(b)));

    CHECK(Equal(Array<int, 0>{}, Sort(Range<0>{})));

    CHECK(Equal(Array{11, 11, 11, 11}, Sort(Repeat<4, int>{11})));

    CHECK(Equal(Array{11, 12, 13, 14}, Sort(Set{11, 12, 13, 14})));
    CHECK(Equal(Array{11, 12, 13, 14}, Sort(Set{11, 13, 12, 14})));

    CHECK(Equal(Array{'a', 'b', 'c', 'd'}, Sort(String{"abcd"})));
    CHECK(Equal(Array{'a', 'b', 'c', 'x', 'y', 'z'}, Sort(String{"axbycz"})));

    CHECK(Equal(Array{"four", "one", "three", "two"}, Sort(Array{"one", "two", "three", "four"})));
    CHECK(Equal(Array{"four", "one", "three", "two"}, Sort(Array{"one", "two", "three", "four"})));
}
