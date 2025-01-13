#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-interleave.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Interleave", "[CljonicCoreInterleave]")
{
    constexpr auto a0{Array{1, 2, 3}};
    constexpr auto a1{Array{11, 12, 13}};
    constexpr auto a2{Array{111, 112}};

    CHECK(Equal(Array<int, 0>{}, Interleave()));
    CHECK(Equal(Array{1, 11, 2, 12, 3, 13}, Interleave(a0, a1)));
    CHECK(Equal(Array{1, 111, 2, 112}, Interleave(a0, a2)));
    CHECK(Equal(Array{1, 11, 111, 2, 12, 112}, Interleave(a0, a1, a2)));
    CHECK(Equal(Array{1, 0, 2, 1, 3, 2}, Interleave(a0, Range<>{})));
    CHECK(Equal(Array{1, 7, 2, 7, 3, 7}, Interleave(a0, Repeat{7})));
    CHECK(Equal(Array{1, 3, 2, 5, 3, 7}, Interleave(a0, Set{3, 5, 7, 9})));
    CHECK(Equal(Array{'H', '_', 'e', '_', 'l', '_', 'l', '_', 'o', '_'}, Interleave(String{"Hello"}, Repeat{'_'})));
}
