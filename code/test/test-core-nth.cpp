#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-nth.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Nth", "[CljonicCoreNth]")
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};

    CHECK(0 == Nth(a, 0));
    CHECK(4 == Nth(a, 4));
    CHECK(9 == Nth(a, 9));
    CHECK(0 == Nth(a, 10));
    CHECK(11 == Nth(a, 10, 11));
}
