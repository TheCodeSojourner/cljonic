#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-iterate.hpp"
#include "cljonic-core-takelast.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("TakeLast", "[CljonicCoreTakeLast]")
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    CHECK(Equal(Array<int, 0>{}, TakeLast(0, a)));
    CHECK(Equal(Array{9}, TakeLast(1, a)));
    CHECK(Equal(Array{5, 6, 7, 8, 9}, TakeLast(5, a)));
    CHECK(Equal(Array{996, 997, 998, 999, 1000}, TakeLast(5, itr)));
    CHECK(Equal(Array<int, 0>{}, TakeLast(50, Range<0>{})));
    CHECK(Equal(Array{"11", "11", "11", "11", "11", "11", "11"}, TakeLast(50, Repeat<7, const char*>{"11"})));
    CHECK(Equal(Array{'l', 'l', 'o'}, TakeLast(3, String{"Hello"})));
}
