#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-notevery.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto Less10 = [](const int i) { return i < 10; };
constexpr auto LessP = [](const char c) { return c < 'p'; };

SCENARIO("NotEvery", "[CljonicCoreNotEvery]")
{
    CHECK(false == NotEvery(Less10, Array<int, 10>{}));
    CHECK(true == NotEvery(Less10, Array{10}));
    CHECK(false == NotEvery(Less10, Array{1, 2, 3}));

    CHECK(false == NotEvery(Less10, Range<0>{}));
    CHECK(true == NotEvery(Less10, Range<11>{}));
    CHECK(false == NotEvery(Less10, Range<10>{}));

    CHECK(false == NotEvery(Less10, Repeat<0, int>{10}));
    CHECK(true == NotEvery(Less10, Repeat<10, int>{10}));
    CHECK(false == NotEvery(Less10, Repeat<3, int>{1}));

    CHECK(false == NotEvery(Less10, Set<int, 5>{}));
    CHECK(true == NotEvery(Less10, Set{10, 11, 12}));
    CHECK(false == NotEvery(Less10, Set{3, 5, 7, 9}));

    CHECK(false == NotEvery(LessP, String{""}));
    CHECK(true == NotEvery(LessP, String{"xyz"}));
    CHECK(false == NotEvery(LessP, String{"hello"}));
}
