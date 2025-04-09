#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-every.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto Less10 = [](const int i)
{ return i < 10; };
constexpr auto LessP = [](const char c)
{ return c < 'p'; };

SCENARIO("Every", "[CljonicCoreEvery]")
{
    EnableNoHeapMessagePrinting();

    CHECK_CLJONIC(true == Every(Less10, Array<int, 10>{}));
    CHECK_CLJONIC(false == Every(Less10, Array{10}));
    CHECK_CLJONIC(true == Every(Less10, Array{1, 2, 3}));

    CHECK_CLJONIC(true == Every(Less10, Range<0>{}));
    CHECK_CLJONIC(false == Every(Less10, Range<11>{}));
    CHECK_CLJONIC(true == Every(Less10, Range<10>{}));

    CHECK_CLJONIC(true == Every(Less10, Repeat<0, int>{10}));
    CHECK_CLJONIC(false == Every(Less10, Repeat<10, int>{10}));
    CHECK_CLJONIC(true == Every(Less10, Repeat<3, int>{1}));

    CHECK_CLJONIC(true == Every(Less10, Set<int, 5>{}));
    CHECK_CLJONIC(false == Every(Less10, Set{10, 11, 12}));
    CHECK_CLJONIC(true == Every(Less10, Set{3, 5, 7, 9}));

    CHECK_CLJONIC(true == Every(LessP, String{""}));
    CHECK_CLJONIC(false == Every(LessP, String{"xyz"}));
    CHECK_CLJONIC(true == Every(LessP, String{"hello"}));

    DisableNoHeapMessagePrinting();
}
