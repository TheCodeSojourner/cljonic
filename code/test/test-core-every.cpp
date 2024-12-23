#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-every.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto Less10 = [](const int i) { return i < 10; };
constexpr auto LessP = [](const char c) { return c < 'p'; };

SCENARIO("Every", "[CljonicCoreEvery]")
{
    CHECK(true == Every(Less10, Array<int, 10>{}));
    CHECK(false == Every(Less10, Array{10}));
    CHECK(true == Every(Less10, Array{1, 2, 3}));

    CHECK(true == Every(Less10, Range<0>{}));
    CHECK(false == Every(Less10, Range<11>{}));
    CHECK(true == Every(Less10, Range<10>{}));

    CHECK(true == Every(Less10, Repeat(0, 10)));
    CHECK(false == Every(Less10, Repeat(10, 10)));
    CHECK(true == Every(Less10, Repeat(4, 1)));

    CHECK(true == Every(Less10, Set<int, 5>{}));
    CHECK(false == Every(Less10, Set{10, 11, 12}));
    CHECK(true == Every(Less10, Set{3, 5, 7, 9}));

    CHECK(true == Every(LessP, String{""}));
    CHECK(false == Every(LessP, String{"xyz"}));
    CHECK(true == Every(LessP, String{"hello"}));
}
