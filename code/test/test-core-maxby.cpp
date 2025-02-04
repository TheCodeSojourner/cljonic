#include <tuple>
#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-maxby.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("MaxBy", "[CljonicCoreMaxBy]")
{
    constexpr auto IsALessThanB = [](const int a, const int b) { return a < b; };

    constexpr auto a{Array{11, 14, 13, 14}};
    CHECK(14 == MaxBy(IsALessThanB, a));

    constexpr auto b{Array{11}};
    CHECK(11 == MaxBy(IsALessThanB, b));

    const auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    CHECK(CljonicCollectionMaximumElementCount == MaxBy(IsALessThanB, itr));

    CHECK(0 == MaxBy(IsALessThanB, Range<0>{}));
    CHECK(11 == MaxBy(IsALessThanB, Repeat<4, int>{11}));
    CHECK(14 == MaxBy(IsALessThanB, Set{11, 14, 13, 14}));
    CHECK('d' == MaxBy([](const char i, const char j) { return i < j; }, String{"adcb"}));
    CHECK(14 == MaxBy(IsALessThanB, 1, 2, 14, 7, 9));
    CHECK('9' == MaxBy([](const char i, const char j) { return i < j; }, '1', '2', '9', '7', '9'));
}
