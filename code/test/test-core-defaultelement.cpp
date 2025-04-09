#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-defaultelement.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("DefaultElement", "[CljonicCoreDefaultElement]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a{Array<int, 10>{11, 12, 13, 14}};
    CHECK_CLJONIC(0 == DefaultElement(a));

    const auto i{Iterator{[](const int i) { return i + 1; }, 0}};
    CHECK_CLJONIC(0 == DefaultElement(i));

    constexpr auto rng{Range<1, 5>{}};
    CHECK_CLJONIC(0 == DefaultElement(rng));

    constexpr auto rpt{Repeat<3, int>{11}};
    CHECK_CLJONIC(0 == DefaultElement(rpt));

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    CHECK_CLJONIC(0 == DefaultElement(set));

    constexpr auto str{String{"Hello"}};
    CHECK_CLJONIC('\0' == DefaultElement(str));

    DisableNoHeapMessagePrinting();
}
