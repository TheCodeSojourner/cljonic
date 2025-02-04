#include <tuple>
#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-minby.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("MinBy", "[CljonicCoreMinBy]")
{
    constexpr auto IsALessThanB = [](const int a, const int b) { return a < b; };

    constexpr auto a{Array{11, 14, 13, 14}};
    CHECK(11 == MinBy(IsALessThanB, a));

    constexpr auto b{Array{11}};
    CHECK(11 == MinBy(IsALessThanB, b));

    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    CHECK(1 == MinBy(IsALessThanB, itr));

    CHECK(0 == MinBy(IsALessThanB, Range<0>{}));
    CHECK(11 == MinBy(IsALessThanB, Repeat<4, int>{11}));
    CHECK(11 == MinBy(IsALessThanB, Set{11, 14, 13, 14}));
    CHECK('a' == MinBy([](const char i, const char j) { return i < j; }, String{"adcb"}));
    CHECK(1 == MinBy(IsALessThanB, 1, 2, 14, 7, 9));
    CHECK('1' == MinBy([](const char i, const char j) { return i < j; }, '1', '2', '9', '7', '9'));
}
