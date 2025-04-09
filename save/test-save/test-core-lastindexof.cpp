#include <tuple>
#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-lastindexof.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("LastIndexOf", "[CljonicCoreLastIndexOf]")
{
    constexpr auto a{Array{11, 14, 13, 14}};
    CHECK(3 == LastIndexOf(a, 14));

    constexpr auto b{Array{11}};
    CHECK(0 == LastIndexOf(b, 11));

    CHECK(CljonicInvalidIndex == LastIndexOf(Range<0>{}, 3));
    CHECK(3 == LastIndexOf(Repeat<4, int>{11}, 11));
    CHECK(2 == LastIndexOf(Set{11, 14, 13, 14}, 13));
    CHECK(2 == LastIndexOf(String{"adcb"}, 'c'));
    CHECK(1 == LastIndexOf(Array{"zero", "one", "two"}, "one"));
}
