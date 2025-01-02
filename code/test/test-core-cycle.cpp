#include <limits>
#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-cycle.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Cycle", "[CljonicCoreCycle]")
{
    constexpr auto a{Array{11, 12, 13, 14}};
    const auto c{Cycle(a)};
    auto MAX_SIZE_T{CljonicCollectionMaximumElementCount};

    const auto cycleA0{Cycle(a)};
    CHECK(MAX_SIZE_T == cycleA0.Count());
    CHECK(11 == cycleA0[0]);
    CHECK(12 == cycleA0[1]);
    CHECK(13 == cycleA0[2]);
    CHECK(14 == cycleA0[3]);
    CHECK(11 == cycleA0[4]);
    CHECK(14 == cycleA0[MAX_SIZE_T - 1]);

    const auto cycleA1{Cycle(Array{11, 12, 13, 14})};
    CHECK(MAX_SIZE_T == cycleA1.Count());
    CHECK(11 == cycleA1[0]);
    CHECK(12 == cycleA1[1]);
    CHECK(13 == cycleA1[2]);
    CHECK(14 == cycleA1[3]);
    CHECK(11 == cycleA1[4]);
    CHECK(14 == cycleA1[MAX_SIZE_T - 1]);

    const auto cycleA2{Cycle(Array<int, 0>{})};
    CHECK(0 == cycleA2.Count());
    CHECK(0 == cycleA2[0]);
    CHECK(0 == cycleA2[1]);
    CHECK(0 == cycleA2[2]);
    CHECK(0 == cycleA2[3]);
    CHECK(0 == cycleA2[4]);
    CHECK(0 == cycleA2[MAX_SIZE_T - 1]);

    constexpr auto rng{Range<1, 5>{}};
    const auto cycleRng0{Cycle(rng)};
    CHECK(MAX_SIZE_T == cycleRng0.Count());
    CHECK(1 == cycleRng0[0]);
    CHECK(2 == cycleRng0[1]);
    CHECK(3 == cycleRng0[2]);
    CHECK(4 == cycleRng0[3]);
    CHECK(1 == cycleRng0[4]);
    CHECK(4 == cycleRng0[MAX_SIZE_T - 1]);

    const auto cycleRng1{Cycle(Range<1, 5>{})};
    CHECK(MAX_SIZE_T == cycleRng1.Count());
    CHECK(1 == cycleRng1[0]);
    CHECK(2 == cycleRng1[1]);
    CHECK(3 == cycleRng1[2]);
    CHECK(4 == cycleRng1[3]);
    CHECK(1 == cycleRng1[4]);
    CHECK(4 == cycleRng1[MAX_SIZE_T - 1]);

    const auto cycleRng2{Cycle(Range<0>{})};
    CHECK(0 == cycleRng2.Count());
    CHECK(0 == cycleRng2[0]);
    CHECK(0 == cycleRng2[1]);
    CHECK(0 == cycleRng2[2]);
    CHECK(0 == cycleRng2[3]);
    CHECK(0 == cycleRng2[4]);
    CHECK(0 == cycleRng2[MAX_SIZE_T - 1]);

    constexpr auto rpt{Repeat<3, int>{11}};
    const auto cycleRpt0{Cycle(rpt)};
    CHECK(MAX_SIZE_T == cycleRpt0.Count());
    CHECK(11 == cycleRpt0[0]);
    CHECK(11 == cycleRpt0[1]);
    CHECK(11 == cycleRpt0[2]);
    CHECK(11 == cycleRpt0[3]);
    CHECK(11 == cycleRpt0[4]);
    CHECK(11 == cycleRpt0[MAX_SIZE_T - 1]);

    const auto cycleRpt1{Cycle(Repeat<3, int>{11})};
    CHECK(MAX_SIZE_T == cycleRpt1.Count());
    CHECK(11 == cycleRpt1[0]);
    CHECK(11 == cycleRpt1[1]);
    CHECK(11 == cycleRpt1[2]);
    CHECK(11 == cycleRpt1[3]);
    CHECK(11 == cycleRpt1[4]);
    CHECK(11 == cycleRpt1[MAX_SIZE_T - 1]);

    const auto cycleRpt2{Cycle(Repeat<0, int>{11})};
    CHECK(0 == cycleRpt2.Count());
    CHECK(0 == cycleRpt2[0]);
    CHECK(0 == cycleRpt2[1]);
    CHECK(0 == cycleRpt2[2]);
    CHECK(0 == cycleRpt2[3]);
    CHECK(0 == cycleRpt2[4]);
    CHECK(0 == cycleRpt2[MAX_SIZE_T - 1]);

    const auto set{Set{1, 2, 1, 3, 4}};
    const auto cycleSet0{Cycle(set)};
    CHECK(MAX_SIZE_T == cycleSet0.Count());
    CHECK(1 == cycleSet0[0]);
    CHECK(2 == cycleSet0[1]);
    CHECK(3 == cycleSet0[2]);
    CHECK(4 == cycleSet0[3]);
    CHECK(1 == cycleSet0[4]);
    CHECK(4 == cycleSet0[MAX_SIZE_T - 1]);

    const auto cycleSet1{Cycle(Set{1, 2, 1, 3, 4})};
    CHECK(MAX_SIZE_T == cycleSet1.Count());
    CHECK(1 == cycleSet1[0]);
    CHECK(2 == cycleSet1[1]);
    CHECK(3 == cycleSet1[2]);
    CHECK(4 == cycleSet1[3]);
    CHECK(1 == cycleSet1[4]);
    CHECK(4 == cycleSet1[MAX_SIZE_T - 1]);

    const auto cycleSet2{Cycle(Set<int, 2>{})};
    CHECK(0 == cycleSet2.Count());
    CHECK(0 == cycleSet2[0]);
    CHECK(0 == cycleSet2[1]);
    CHECK(0 == cycleSet2[2]);
    CHECK(0 == cycleSet2[3]);
    CHECK(0 == cycleSet2[4]);
    CHECK(0 == cycleSet2[MAX_SIZE_T - 1]);

    constexpr auto str{String{"Hello"}};
    const auto cycleStr0{Cycle(str)};
    CHECK(MAX_SIZE_T == cycleStr0.Count());
    CHECK('H' == cycleStr0[0]);
    CHECK('e' == cycleStr0[1]);
    CHECK('l' == cycleStr0[2]);
    CHECK('l' == cycleStr0[3]);
    CHECK('o' == cycleStr0[4]);
    CHECK('H' == cycleStr0[5]);
    CHECK('o' == cycleStr0[MAX_SIZE_T - 1]);

    const auto cycleStr1{Cycle(String{"Hello"})};
    CHECK(MAX_SIZE_T == cycleStr1.Count());
    CHECK('H' == cycleStr1[0]);
    CHECK('e' == cycleStr1[1]);
    CHECK('l' == cycleStr1[2]);
    CHECK('l' == cycleStr1[3]);
    CHECK('o' == cycleStr1[4]);
    CHECK('H' == cycleStr1[5]);
    CHECK('o' == cycleStr1[MAX_SIZE_T - 1]);

    const auto cycleStr2{Cycle(String{""})};
    CHECK(0 == cycleStr2.Count());
    CHECK('\0' == cycleStr2[0]);
    CHECK('\0' == cycleStr2[1]);
    CHECK('\0' == cycleStr2[2]);
    CHECK('\0' == cycleStr2[3]);
    CHECK('\0' == cycleStr2[4]);
    CHECK('\0' == cycleStr2[5]);
    CHECK('\0' == cycleStr2[MAX_SIZE_T - 1]);
}
