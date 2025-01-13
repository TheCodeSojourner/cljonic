#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-indexofby.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

SCENARIO("IndexOfBy", "[CljonicCoreIndexOfBy]")
{
    constexpr auto SameInts = [](const int i, const int j) { return i == j; };

    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK(2 == IndexOfBy(SameInts, a, 2));

    constexpr auto rng{Range<10>{}};
    CHECK(4 == IndexOfBy(SameInts, rng, 4));

    constexpr auto rpt{Repeat<10, int>{1}};
    CHECK(CLJONIC_INVALID_INDEX == IndexOfBy(SameInts, rpt, 9));

    constexpr auto s{Set<int, 4>{1, 2, 3, 4, 5, 6}};
    CHECK(3 == IndexOfBy(SameInts, s, 4));

    CHECK(2 == IndexOfBy(SameInts, String{"Hello"}, 'l'));
}
