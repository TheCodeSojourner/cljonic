#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-splitwith.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("SplitWith", "[CljonicCoreSplitWith]")
{
    constexpr auto IsLessThan13 = [](const int a) { return a < 13; };
    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};

    constexpr auto a{Array{11, 12, 13, 14}};
    CHECK(Equal(Array{Array<int, 4>{11, 12}, Array<int, 4>{13, 14}}, SplitWith(IsLessThan13, a)));

    constexpr auto b{Array{11, 13, 12, 14}};
    CHECK(Equal(Array{Array<int, 4>{11}, Array<int, 4>{13, 12, 14}}, SplitWith(IsLessThan13, b)));

    const auto splitByItr{SplitWith(IsLessThan13, itr)}; // Array{1, ..., 12} and Array{13, ..., 1000}
    const auto splitByItr0{*splitByItr.begin()};
    const auto splitByItr1{*(splitByItr.begin() + 1)};
    CHECK(Equal(Array{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, splitByItr0));
    CHECK(13 == *splitByItr1.begin());
    CHECK(1000 == *(splitByItr1.end() - 1));

    CHECK(Equal(Array{Array<int, 0>{}, Array<int, 0>{}}, SplitWith(IsLessThan13, Range<0>{})));

    CHECK(Equal(Array{Array<int, 4>{11, 11, 11, 11}, Array<int, 4>{}}, SplitWith(IsLessThan13, Repeat<4, int>{11})));

    CHECK(Equal(Array{Array<int, 4>{11, 12}, Array<int, 4>{13, 14}}, SplitWith(IsLessThan13, Set{11, 12, 13, 14})));

    CHECK(Equal(Array{Array<int, 4>{11}, Array<int, 4>{13, 12, 14}}, SplitWith(IsLessThan13, Set{11, 13, 12, 14})));

    constexpr auto NotC = [](const char c) { return c != 'c'; };
    CHECK(Equal(Array{Array<char, 4>{'a', 'b'}, Array<char, 4>{'c', 'd'}}, SplitWith(NotC, String{"abcd"})));
    CHECK(
        Equal(Array{Array<char, 4>{'a', 'x', 'b', 'y'}, Array<char, 4>{'c', 'z'}}, SplitWith(NotC, String{"axbycz"})));
}
