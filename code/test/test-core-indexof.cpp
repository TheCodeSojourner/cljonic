#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-indexof.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("IndexOf", "[CljonicCoreIndexOf]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK_CLJONIC(2 == IndexOf(a, 2));

    const auto itr{Iterator{[](const int i) { return i + 1; }, 0}};
    CHECK_CLJONIC(2 == IndexOf(itr, 2));

    constexpr auto rng{Range<10>{}};
    CHECK_CLJONIC(4 == IndexOf(rng, 4));

    constexpr auto rpt{Repeat<10, int>{1}};
    CHECK_CLJONIC(0 == IndexOf(rpt, 1));
    CHECK_CLJONIC(CljonicInvalidIndex == IndexOf(rpt, 9));

    CHECK_CLJONIC(2 == IndexOf(String{"Hello"}, 'l'));

    CHECK_CLJONIC(1 == IndexOf(Array{"one", "two", "three"}, "two"));

    DisableNoHeapMessagePrinting();
}
