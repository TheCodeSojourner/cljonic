#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-count.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Count", "[CljonicCoreCount]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a{Array<int, 10>{11, 12, 13, 14}};
    CHECK_CLJONIC(4 == Count(a));

    constexpr auto rng{Range<1, 5>{}};
    CHECK_CLJONIC(4 == Count(rng));

    constexpr auto rpt{Repeat<3, int>{11}};
    CHECK_CLJONIC(3 == Count(rpt));

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    CHECK_CLJONIC(4 == Count(set));

    constexpr auto str{String{"Hello"}};
    CHECK_CLJONIC(5 == Count(str));

    DisableNoHeapMessagePrinting();
}
