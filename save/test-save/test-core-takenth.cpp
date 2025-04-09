#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-takenth.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("TakeNth", "[CljonicCoreTakeNth]")
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK(Equal(Array{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, TakeNth(0, a)));
    CHECK(Equal(Array{0, 2, 4, 6, 8}, TakeNth(2, a)));
    CHECK(Equal(Array{0, 3, 6, 9}, TakeNth(3, a)));
    CHECK(Equal(Array{0, 4, 8}, TakeNth(4, a)));
    CHECK(Equal(Array{0, 5}, TakeNth(5, a)));
    CHECK(Equal(Array{0, 6}, TakeNth(6, a)));
    CHECK(Equal(Array{0, 7}, TakeNth(7, a)));
    CHECK(Equal(Array{0, 8}, TakeNth(8, a)));
    CHECK(Equal(Array{0, 9}, TakeNth(9, a)));
    CHECK(Equal(Array{0}, TakeNth(10, a)));
    CHECK(Equal(Array{0}, TakeNth(50, a)));
    CHECK(Equal(Array<int, 1>{}, TakeNth(50, Range<0>{})));
    CHECK(Equal(Array{"11"}, TakeNth(50, Repeat<7, const char*>{"11"})));
    CHECK(Equal(Array{'a'}, TakeNth(5, Set{'a', 'b'})));
    CHECK(Equal(Array{'a', 'd'}, TakeNth(3, String{"abcde"})));
}
