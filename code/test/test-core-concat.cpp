#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-concat.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Concat", "[CljonicCoreConcat]")
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto rng{Range<5>{}};
    constexpr auto rpt{Repeat<5, int>{10}};
    constexpr auto set{Set{100, 101}};
    constexpr auto str{String{"Hello"}};
    constexpr auto c{Concat(a, rng, rpt, set, str)};
    CHECK(21 == c.Count());
    CHECK(11 == c[0]);
    CHECK(12 == c[1]);
    CHECK(13 == c[2]);
    CHECK(14 == c[3]);
    CHECK(0 == c[4]);
    CHECK(1 == c[5]);
    CHECK(2 == c[6]);
    CHECK(3 == c[7]);
    CHECK(4 == c[8]);
    CHECK(10 == c[9]);
    CHECK(10 == c[10]);
    CHECK(10 == c[11]);
    CHECK(10 == c[12]);
    CHECK(10 == c[13]);
    CHECK(100 == c[14]);
    CHECK(101 == c[15]);
    CHECK(72 == c[16]);
    CHECK(101 == c[17]);
    CHECK(108 == c[18]);
    CHECK(108 == c[19]);
    CHECK(111 == c[20]);
    CHECK(0 == c[21]);
    CHECK(0 == Concat().Count());
}
