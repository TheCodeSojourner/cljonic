#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-map.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto TwoTimes = [](const int i) { return 2 * i; };
constexpr auto Add2 = [](const int i, const int j) { return i + j; };
constexpr auto Add3 = [](const int i, const int j, const int k) { return i + j + k; };

SCENARIO("Map", "[CljonicCoreMap]")
{
    constexpr auto itr{Iterate([](const int i) { return 10 * i; }, 1)};
    CHECK(Equal(Array{2, 12, 103, 1004}, Map(Add2, Set{1, 2, 3, 4}, itr)));
    CHECK(Equal(Array{2, 4, 6, 8}, Map(TwoTimes, Array{1, 2, 3, 4})));
    CHECK(Equal(Array{2, 4, 6, 8}, Map(TwoTimes, Array<int, 10>{1, 2, 3, 4})));
    CHECK(Equal(Array<int, 10>{}, Map(TwoTimes, Array<int, 10>{})));
    CHECK(Equal(Array{1, 3, 5, 7}, Map(Add2, Array<int, 10>{1, 2, 3, 4}, Range{})));
    CHECK(Equal(Array{2, 3, 4, 5}, Map(Add2, Array<int, 10>{1, 2, 3, 4}, Repeat{1})));
    CHECK(Equal(Array{2, 4, 6, 8}, Map(TwoTimes, Set{1, 2, 3, 4})));
    CHECK(Equal(Array{1, 3, 5, 7}, Map(Add2, Set{1, 2, 3, 4}, Range{})));
    CHECK(Equal(Array{2, 3, 4, 5}, Map(Add2, Set{1, 2, 3, 4}, Repeat{1})));
    CHECK(Equal(Array{4, 6, 8, 10}, Map(Add3, Set{1, 2, 3, 4}, Range{}, Repeat{3})));
}
