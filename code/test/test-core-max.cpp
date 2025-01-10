#include <tuple>
#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-max.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Max", "[CljonicCoreMax]")
{
    constexpr auto a{Array{11, 14, 13, 14}};
    CHECK(14 == Max(a));

    constexpr auto b{Array{11}};
    CHECK(11 == Max(b));

    CHECK(0 == Max(Range<0>{}));
    CHECK(11 == Max(Repeat<4, int>{11}));
    CHECK(14 == Max(Set{11, 14, 13, 14}));
    CHECK('d' == Max(String{"adcb"}));
    CHECK(14 == Max(1, 2, 14, 7, 9));
    CHECK('9' == Max('1', '2', '9', '7', '9'));
}
