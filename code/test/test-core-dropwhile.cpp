#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-dropwhile.hpp"
#include "cljonic-core-iterate.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("DropWhile", "[CljonicCoreDropWhile]")
{
    constexpr auto Even = [](const int i) { return (0 == (i % 2)); };
    constexpr auto LessThan990 = [](const int i) { return (i < 990); };

    constexpr auto a{Array<int, 10>{0, 2, 4, 5, 6, 7, 8, 9}};
    CHECK(Equal(Array{5, 6, 7, 8, 9}, DropWhile(Even, a)));

    const auto itr{Iterate([](const int i) { return (i + 1); }, 1)};
    CHECK(Equal(Array{990, 991, 992, 993, 994, 995, 996, 997, 998, 999, 1000}, DropWhile(LessThan990, itr)));

    constexpr auto rng{Range<10>{}};
    CHECK(Equal(Array{1, 2, 3, 4, 5, 6, 7, 8, 9}, DropWhile(Even, rng)));

    constexpr auto rpt{Repeat<10, int>{2}};
    CHECK(Equal(Array<int, 0>{}, DropWhile(Even, rpt)));

    constexpr auto s{Set<int, 4>{2, 4, 5, 6}};
    CHECK(Equal(Array{5, 6}, DropWhile(Even, s)));

    CHECK(Equal(Array{'l', 'l', 'o'}, DropWhile([](const char c) { return ('l' != c); }, String{"Hello"})));
}
