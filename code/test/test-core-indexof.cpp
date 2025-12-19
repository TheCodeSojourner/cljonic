#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-indexof.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

SCENARIO("IndexOf", "[CljonicCoreIndexOf]")
{
    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK(2 == IndexOf(a, 2));

    constexpr auto rng{Range<10>{}};
    CHECK(4 == IndexOf(rng, 4));

    constexpr auto rpt{Repeat<10, int>{1}};
    CHECK(CLJONIC_INVALID_INDEX == IndexOf(rpt, 9));

    constexpr auto s{Set<int, 4>{1, 2, 3, 4}};
    CHECK(3 == IndexOf(s, 4));

    CHECK(2 == IndexOf(String{"Hello"}, 'l'));

    CHECK(1 == IndexOf(Array{"one", "two", "three"}, "two"));
}
