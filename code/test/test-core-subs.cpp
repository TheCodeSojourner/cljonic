#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-subs.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Subs", "[CljonicCoreSubs]")
{
    constexpr auto a{Array<int, 10>{11, 12, 13, 14}};
    CHECK(Equal(Array<int, 0>{}, Subs(a, 2, 2)));
    CHECK(Equal(Array<int, 0>{}, Subs(a, 3, 0)));
    CHECK(Equal(Array{11, 12, 13}, Subs(a, 0, 3)));

    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    CHECK(Equal(Array{1, 2, 3}, Subs(itr, 0, 3)));

    constexpr auto rng{Range<1, 5>{}};
    CHECK(Equal(Array{2, 3, 4}, Subs(rng, 1)));

    constexpr auto rpt{Repeat<3, int>{11}};
    CHECK(Equal(Array{11, 11, 11}, Subs(rpt, 0, 5)));

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    CHECK(Equal(Array<int, 0>{}, Subs(set, 4, 7)));

    constexpr auto str{String{"Hello"}};
    CHECK(Equal(Array{'l', 'l'}, Subs(str, 2, 4)));
}
