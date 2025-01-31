#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-interpose.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-take.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Interpose", "[CljonicCoreInterpose]")
{
    constexpr auto i{Iterate([](const int i) { return 1 + i; }, 11)};
    CHECK(Equal(Array{11, 1, 12, 1, 13}, Interpose(1, Array{11, 12, 13})));
    CHECK(Equal(Array{11, 7, 12, 7, 13}, Interpose(7, Take<3>(i))));
    CHECK(Equal(Array{0, 7, 1, 7, 2, 7, 3, 7, 4}, Interpose(7, Range<5>{})));
    CHECK(Equal(Array{9, 7, 9, 7, 9, 7, 9}, Interpose(7, Repeat<4, int>{9})));
    CHECK(Equal(Array{"One", " ", "Two", " ", "Three"}, Interpose(" ", Set{"One", "Two", "Three"})));
    CHECK(Equal(Array{'H', '-', 'e', '-', 'l', '-', 'l', '-', 'o'}, Interpose('-', String{"Hello"})));
}
