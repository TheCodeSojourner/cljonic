#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-notany.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto Less10 = [](const int i) { return i < 10; };
constexpr auto LessP = [](const char c) { return c < 'p'; };

SCENARIO("NotAny", "[CljonicCoreNotAny]")
{
    CHECK(true == NotAny(Less10, Array<int, 10>{}));
    CHECK(true == NotAny(Less10, Array{10}));
    CHECK(false == NotAny(Less10, Array{1, 2, 3}));

    CHECK(true == NotAny(Less10, Range<0>{}));
    CHECK(false == NotAny(Less10, Range<11>{}));
    CHECK(false == NotAny(Less10, Range<10>{}));

    CHECK(true == NotAny(Less10, Repeat<0, int>{10}));
    CHECK(true == NotAny(Less10, Repeat<10, int>{10}));
    CHECK(false == NotAny(Less10, Repeat<3, int>{1}));

    CHECK(true == NotAny(Less10, Set<int, 5>{}));
    CHECK(true == NotAny(Less10, Set{10, 11, 12}));
    CHECK(false == NotAny(Less10, Set{3, 5, 7, 9}));

    CHECK(true == NotAny(LessP, String{""}));
    CHECK(true == NotAny(LessP, String{"xyz"}));
    CHECK(false == NotAny(LessP, String{"hello"}));
}
