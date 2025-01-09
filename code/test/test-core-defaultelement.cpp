#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-defaultelement.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("DefaultElement", "[CljonicCoreDefaultElement]")
{
    constexpr auto a{Array<int, 10>{11, 12, 13, 14}};
    CHECK(0 == a.DefaultElement());

    constexpr auto rng{Range<1, 5>{}};
    CHECK(0 == rng.DefaultElement());

    constexpr auto rpt{Repeat<3, int>{11}};
    CHECK(0 == rpt.DefaultElement());

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    CHECK(0 == set.DefaultElement());

    constexpr auto str{String{"Hello"}};
    CHECK('\0' == str.DefaultElement());
}
