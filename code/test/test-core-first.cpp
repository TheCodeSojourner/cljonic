#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-first.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("First", "[CljonicCoreFirst]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a{Array{11, 12, 13, 14}};
    CHECK_CLJONIC(11 == First(a));

    CHECK_CLJONIC(0 == First(Iterator{[](const int i) { return i + 1; }, 0}));

    constexpr auto rng{Range<1, 5>{}};
    CHECK_CLJONIC(1 == First(rng));

    constexpr auto rpt{Repeat<3, int>{11}};
    CHECK_CLJONIC(11 == First(rpt));

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    CHECK_CLJONIC(1 == First(set));

    constexpr auto str{String{"Hello"}};
    CHECK_CLJONIC('H' == First(str));

    constexpr auto strEmpty{String{""}};
    CHECK_CLJONIC('\0' == First(strEmpty));

    DisableNoHeapMessagePrinting();
}
