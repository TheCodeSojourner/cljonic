#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-conj.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Conj", "[CljonicCoreConj]")
{
    constexpr auto a{Array{11, 12, 13, 14}};
    const auto cA{Conj(a, 15)};
    CHECK(5 == cA.Count());
    CHECK(11 == cA[0]);
    CHECK(12 == cA[1]);
    CHECK(13 == cA[2]);
    CHECK(14 == cA[3]);
    CHECK(15 == cA[4]);
    CHECK(0 == cA[5]);

    constexpr auto rng{Range<1, 5>{}};
    const auto cRng{Conj(rng, 15)};
    CHECK(5 == cRng.Count());
    CHECK(1 == cRng[0]);
    CHECK(2 == cRng[1]);
    CHECK(3 == cRng[2]);
    CHECK(4 == cRng[3]);
    CHECK(15 == cRng[4]);
    CHECK(0 == cRng[5]);

    constexpr auto rpt{Repeat<3, int>{11}};
    const auto cRpt{Conj(rpt, 15)};
    CHECK(4 == cRpt.Count());
    CHECK(11 == cRpt[0]);
    CHECK(11 == cRpt[1]);
    CHECK(11 == cRpt[2]);
    CHECK(15 == cRpt[3]);
    CHECK(0 == cRpt[4]);

    const auto set{Set{1, 2, 1, 3, 4}};
    const auto cSet{Conj(set, 15)};
    CHECK(5 == cSet.Count());
    CHECK(1 == cSet[0]);
    CHECK(2 == cSet[1]);
    CHECK(3 == cSet[2]);
    CHECK(4 == cSet[3]);
    CHECK(15 == cSet[4]);
    CHECK(0 == cSet[5]);

    constexpr auto str{String{"Hello"}};
    const auto cStr{Conj(str, '!')};
    CHECK(6 == cStr.Count());
    CHECK('H' == cStr[0]);
    CHECK('e' == cStr[1]);
    CHECK('l' == cStr[2]);
    CHECK('l' == cStr[3]);
    CHECK('o' == cStr[4]);
    CHECK('!' == cStr[5]);
    CHECK('\0' == cStr[6]);
}
