#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-indexofby.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("IndexOfBy", "[CljonicCoreIndexOfBy]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto SameInts = [](const int i, const int j) { return i == j; };

    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 1, 2, 6, 7, 8, 9}};
    CHECK_CLJONIC(2 == IndexOfBy(SameInts, a, 2));
    CHECK_CLJONIC(5 == IndexOfBy(SameInts, a, 2, 3));
    CHECK_CLJONIC(CljonicInvalidIndex == IndexOfBy(SameInts, a, 2, 30));

    const auto itr{Iterator{[](const int i) { return i + 1; }, 10}};
    CHECK_CLJONIC(3 == IndexOfBy(SameInts, itr, 13));

    constexpr auto rng{Range<10>{}};
    CHECK_CLJONIC(4 == IndexOfBy(SameInts, rng, 4));

    constexpr auto rpt{Repeat<10, int>{1}};
    CHECK_CLJONIC(CljonicInvalidIndex == IndexOfBy(SameInts, rpt, 9));

    CHECK_CLJONIC(2 == IndexOfBy(SameInts, String{"Hello"}, 'l'));

    DisableNoHeapMessagePrinting();
}
