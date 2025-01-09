#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-splitby.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("SplitBy", "[CljonicCoreSplitBy]")
{
    constexpr auto IsLessThan13 = [](const int a) { return a < 13; };

    constexpr auto a{Array{11, 12, 13, 14}};
    CHECK(Equal(Array{Array<int, 4>{11, 12}, Array<int, 4>{13, 14}}, SplitBy(IsLessThan13, a)));

    constexpr auto b{Array{11, 13, 12, 14}};
    CHECK(Equal(Array{Array<int, 4>{11}, Array<int, 4>{13, 12, 14}}, SplitBy(IsLessThan13, b)));

    CHECK(Equal(Array{Array<int, 0>{}, Array<int, 0>{}}, SplitBy(IsLessThan13, Range<0>{})));

    CHECK(Equal(Array{Array<int, 4>{11, 11, 11, 11}, Array<int, 4>{}}, SplitBy(IsLessThan13, Repeat<4, int>{11})));

    CHECK(Equal(Array{Array<int, 4>{11, 12}, Array<int, 4>{13, 14}}, SplitBy(IsLessThan13, Set{11, 12, 13, 14})));

    CHECK(Equal(Array{Array<int, 4>{11}, Array<int, 4>{13, 12, 14}}, SplitBy(IsLessThan13, Set{11, 13, 12, 14})));

    constexpr auto NotC = [](const char c) { return c != 'c'; };
    CHECK(Equal(Array{Array<char, 4>{'a', 'b'}, Array<char, 4>{'c', 'd'}}, SplitBy(NotC, String{"abcd"})));
    CHECK(Equal(Array{Array<char, 4>{'a', 'x', 'b', 'y'}, Array<char, 4>{'c', 'z'}}, SplitBy(NotC, String{"axbycz"})));
}
