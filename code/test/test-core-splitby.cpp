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
    const auto [splitByA1, splitByA2]{SplitBy(IsLessThan13, a)};
    CHECK(Equal(splitByA1, Array{11, 12}));
    CHECK(Equal(splitByA2, Array{13, 14}));

    constexpr auto b{Array{11, 13, 12, 14}};
    const auto [splitByB1, splitByB2]{SplitBy(IsLessThan13, b)};
    CHECK(Equal(splitByB1, Array{11}));
    CHECK(Equal(splitByB2, Array{13, 12, 14}));

    const auto [splitByRng1, splitByRng2]{SplitBy(IsLessThan13, Range<0>{})};
    CHECK(Equal(splitByRng1, Array<int, 0>{}));
    CHECK(Equal(splitByRng2, Array<int, 0>{}));

    const auto [splitByRpt1, splitByRpt2]{SplitBy(IsLessThan13, Repeat<4, int>{11})};
    CHECK(Equal(splitByRpt1, Array{11, 11, 11, 11}));
    CHECK(Equal(splitByRpt2, Array<int, 0>{}));

    const auto [splitBySet01, splitBySet02]{SplitBy(IsLessThan13, Set{11, 12, 13, 14})};
    CHECK(Equal(splitBySet01, Array{11, 12}));
    CHECK(Equal(splitBySet02, Array{13, 14}));

    const auto [splitBySet11, splitBySet12]{SplitBy(IsLessThan13, Set{11, 13, 12, 14})};
    CHECK(Equal(splitBySet11, Array{11}));
    CHECK(Equal(splitBySet12, Array{13, 12, 14}));

    constexpr auto NotC = [](const char c) { return c != 'c'; };

    const auto [splitByStr01, splitByStr02]{SplitBy(NotC, String{"abcd"})};
    CHECK(Equal(splitByStr01, Array{'a', 'b'}));
    CHECK(Equal(splitByStr02, Array{'c', 'd'}));

    const auto [splitByStr11, splitByStr12]{SplitBy(NotC, String{"axbycz"})};
    CHECK(Equal(splitByStr11, Array{'a', 'x', 'b', 'y'}));
    CHECK(Equal(splitByStr12, Array{'c', 'z'}));
}
