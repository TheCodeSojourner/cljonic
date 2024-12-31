#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-first.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("First", "[CljonicCoreFirst]")
{
    constexpr auto a{Array{11, 12, 13, 14}};
    CHECK(11 == First(a));

    constexpr auto rng{Range<1, 5>{}};
    CHECK(1 == First(rng));

    constexpr auto rpt{Repeat<3, int>{11}};
    CHECK(11 == First(rpt));

    const auto set{Set{1, 2, 1, 3, 4}};
    CHECK(1 == First(set));

    constexpr auto str{String{"Hello"}};
    CHECK('H' == First(str));

    constexpr auto strEmpty{String{""}};
    CHECK('\0' == First(strEmpty));
}
