#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-concat.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Concat", "[CljonicCoreConcat]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a{Array{11, 12, 13, 14}};
    constexpr auto rng{Range<5>{}};
    constexpr auto rpt{Repeat<5, int>{10}};
    constexpr auto set{Set{100, 101}};
    constexpr auto str{String{"Hello"}};
    constexpr auto c{Concat(a, rng, rpt, set, str)};
    CHECK_CLJONIC(21 == c.Count());
    auto cIt{c.begin()};
    CHECK_CLJONIC(11 == *cIt);
    ++cIt;
    CHECK_CLJONIC(12 == *cIt);
    ++cIt;
    CHECK_CLJONIC(13 == *cIt);
    ++cIt;
    CHECK_CLJONIC(14 == *cIt);
    ++cIt;
    CHECK_CLJONIC(0 == *cIt);
    ++cIt;
    CHECK_CLJONIC(1 == *cIt);
    ++cIt;
    CHECK_CLJONIC(2 == *cIt);
    ++cIt;
    CHECK_CLJONIC(3 == *cIt);
    ++cIt;
    CHECK_CLJONIC(4 == *cIt);
    ++cIt;
    CHECK_CLJONIC(10 == *cIt);
    ++cIt;
    CHECK_CLJONIC(10 == *cIt);
    ++cIt;
    CHECK_CLJONIC(10 == *cIt);
    ++cIt;
    CHECK_CLJONIC(10 == *cIt);
    ++cIt;
    CHECK_CLJONIC(10 == *cIt);
    ++cIt;
    CHECK_CLJONIC(100 == *cIt);
    ++cIt;
    CHECK_CLJONIC(101 == *cIt);
    ++cIt;
    CHECK_CLJONIC(72 == *cIt);
    ++cIt;
    CHECK_CLJONIC(101 == *cIt);
    ++cIt;
    CHECK_CLJONIC(108 == *cIt);
    ++cIt;
    CHECK_CLJONIC(108 == *cIt);
    ++cIt;
    CHECK_CLJONIC(111 == *cIt);
    ++cIt;
    CHECK_CLJONIC(0 == Concat().Count());

    DisableNoHeapMessagePrinting();
}
