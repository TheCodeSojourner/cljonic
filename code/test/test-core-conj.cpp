#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-conj.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-take.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Conj", "[CljonicCoreConj]")
{
    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto cA{Conj(a, 15, 16)};
    CHECK(6 == cA.Count());
    CHECK(11 == cA[0]);
    CHECK(12 == cA[1]);
    CHECK(13 == cA[2]);
    CHECK(14 == cA[3]);
    CHECK(15 == cA[4]);
    CHECK(16 == cA[5]);
    CHECK(0 == cA[6]);

    const auto i{Iterate([](const int i) { return 1 + i; }, 1)};
    const auto i3{Take<3>(i)};
    const auto cItr{Conj(i3, 15)};
    CHECK(4 == cItr.Count());
    CHECK(1 == cItr[0]);
    CHECK(2 == cItr[1]);
    CHECK(3 == cItr[2]);
    CHECK(15 == cItr[3]);
    CHECK(0 == cItr[4]);

    constexpr auto rng{Range<1, 5>{}};
    constexpr auto cRng{Conj(rng, 15)};
    CHECK(5 == cRng.Count());
    CHECK(1 == cRng[0]);
    CHECK(2 == cRng[1]);
    CHECK(3 == cRng[2]);
    CHECK(4 == cRng[3]);
    CHECK(15 == cRng[4]);
    CHECK(0 == cRng[5]);

    constexpr auto rpt{Repeat<3, int>{11}};
    constexpr auto cRpt{Conj(rpt, 15)};
    CHECK(4 == cRpt.Count());
    CHECK(11 == cRpt[0]);
    CHECK(11 == cRpt[1]);
    CHECK(11 == cRpt[2]);
    CHECK(15 == cRpt[3]);
    CHECK(0 == cRpt[4]);

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    constexpr auto cSet{Conj(set, 15)};
    CHECK(5 == cSet.Count());
    CHECK(1 == cSet[0]);
    CHECK(2 == cSet[1]);
    CHECK(3 == cSet[2]);
    CHECK(4 == cSet[3]);
    CHECK(15 == cSet[4]);
    CHECK(0 == cSet[5]);

    constexpr auto str{String{"Hello"}};

    constexpr auto cStr{Conj(str)};
    CHECK(5 == cStr.Count());
    CHECK('H' == cStr[0]);
    CHECK('e' == cStr[1]);
    CHECK('l' == cStr[2]);
    CHECK('l' == cStr[3]);
    CHECK('o' == cStr[4]);
    CHECK('\0' == cStr[5]);

    constexpr auto cStrBang{Conj(str, '!', '!', '!')};
    CHECK(8 == cStrBang.Count());
    CHECK('H' == cStrBang[0]);
    CHECK('e' == cStrBang[1]);
    CHECK('l' == cStrBang[2]);
    CHECK('l' == cStrBang[3]);
    CHECK('o' == cStrBang[4]);
    CHECK('!' == cStrBang[5]);
    CHECK('!' == cStrBang[6]);
    CHECK('!' == cStrBang[7]);
    CHECK('\0' == cStrBang[8]);
}
