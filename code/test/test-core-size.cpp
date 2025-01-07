#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-size.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Size", "[CljonicCoreSize]")
{
    const auto a{Array<int, 10>{11, 12, 13, 14}};
    CHECK(10 == Size(a));

    constexpr auto rng{Range<1, 5>{}};
    CHECK(4 == Size(rng));

    constexpr auto rpt{Repeat<3, int>{11}};
    CHECK(3 == Size(rpt));

    const auto set{Set{1, 2, 1, 3, 4}};
    CHECK(5 == Size(set));

    constexpr auto str{String{"Hello"}};
    CHECK(5 == Size(str));
}
