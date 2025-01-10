#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-last.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Last", "[CljonicCoreLast]")
{
    constexpr auto a{Array{11, 12, 13, 14}};
    CHECK(14 == Last(a));

    constexpr auto rng{Range<1, 5>{}};
    CHECK(4 == Last(rng));

    constexpr auto rpt{Repeat<3, int>{11}};
    CHECK(11 == Last(rpt));

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    CHECK(4 == Last(set));

    constexpr auto str{String{"Hello"}};
    CHECK('o' == Last(str));

    constexpr auto strEmpty{String{""}};
    CHECK('\0' == Last(strEmpty));
}
