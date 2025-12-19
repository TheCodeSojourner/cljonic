#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-remove.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

SCENARIO("Remove", "[CljonicCoreRemove]")
{
    constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK(Equal(Array{1, 3, 5, 7, 9}, Remove(Even, a)));

    constexpr auto rng{Range<10>{}};
    CHECK(Equal(Array{1, 3, 5, 7, 9}, Remove(Even, rng)));

    constexpr auto rpt{Repeat<10, int>{1}};
    CHECK(Equal(Array{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, Remove(Even, rpt)));

    constexpr auto s{Set<int, 4>{1, 2, 3, 4}};
    CHECK(Equal(Array{1, 3}, Remove(Even, s)));

    CHECK(Equal(Array{'H', 'e', 'o'}, Remove([](const char c) { return ('l' == c); }, String{"Hello"})));
}
