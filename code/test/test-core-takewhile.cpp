#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-takewhile.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("TakeWhile", "[CljonicCoreTakeWhile]")
{
    constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

    const auto a{Array<int, 10>{0, 2, 4, 5, 6, 7, 8, 9}};
    CHECK(Equal(Array{0, 2, 4}, TakeWhile(Even, a)));

    const auto rng{Range<10>{}};
    CHECK(Equal(Array{0}, TakeWhile(Even, rng)));

    const auto rpt{Repeat<10, int>{2}};
    CHECK(Equal(Array{2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, TakeWhile(Even, rpt)));

    const auto s{Set<int, 4>{2, 4, 5, 6}};
    CHECK(Equal(Array{2, 4}, TakeWhile(Even, s)));

    CHECK(Equal(Array{'H', 'e'}, TakeWhile([](const char c) { return ('l' != c); }, String{"Hello"})));
}
