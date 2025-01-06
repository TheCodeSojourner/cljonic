#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-second.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Second", "[CljonicCoreSecond]")
{
    constexpr auto a{Array{11, 12, 13, 14}};
    CHECK(12 == Second(a));

    constexpr auto rng{Range<1, 5>{}};
    CHECK(2 == Second(rng));

    constexpr auto rpt{Repeat<3, int>{11}};
    CHECK(11 == Second(rpt));

    const auto set{Set{1, 2, 1, 3, 4}};
    CHECK(2 == Second(set));

    constexpr auto str{String{"Hello"}};
    CHECK('e' == Second(str));

    constexpr auto strEmpty{String{""}};
    CHECK('\0' == Second(strEmpty));
}
