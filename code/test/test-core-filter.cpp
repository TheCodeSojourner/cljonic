#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-filter.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

SCENARIO("Filter", "[CljonicCoreFilter]")
{

    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK(Equal(Array{0, 2, 4, 6, 8}, Filter(Even, a)));

    constexpr auto rng{Range<10>{}};
    CHECK(Equal(Array{0, 2, 4, 6, 8}, Filter(Even, rng)));

    constexpr auto rpt{Repeat<10, int>{1}};
    CHECK(Equal(Array<int, 0>{}, Filter(Even, rpt)));

    constexpr auto s{Set<int, 4>{1, 2, 3, 4, 5, 6}};
    CHECK(Equal(Array{2, 4}, Filter(Even, s)));

    CHECK(Equal(Array{'l', 'l'}, Filter([](const char c) { return ('l' == c); }, String{"Hello"})));
}
