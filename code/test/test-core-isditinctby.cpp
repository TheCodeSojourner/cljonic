#include <cstring>
#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-isdistinctby.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("IsDistinctBy", "[CljonicCoreIsDistinctBy]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto IDBF = [](const int a, const int b) { return a == b; };
    constexpr auto IDBFS = [](const char* a, const char* b) { return 0 == std::strcmp(a, b); };
    constexpr auto a{Array{1, 1, 1, 1}};
    constexpr auto a13{Array{1, 2, 3}};
    constexpr auto r14{Range<1, 4>{}};
    constexpr auto r04{Range<4>{}};
    constexpr auto r41{Repeat<4, int>{1}};
    constexpr auto s{Set{1, 3, 2}};
    constexpr auto s1{Set{3, 1, 2}};
    constexpr auto s2{Set{1, 4, 2}};
    constexpr auto str{String{"abc"}};
    constexpr auto str1{String{"def"}};
    CHECK_CLJONIC(IsDistinctBy(IDBF, 1));
    CHECK_CLJONIC_NOT(IsDistinctBy(IDBF, 1, 1));
    CHECK_CLJONIC(IsDistinctBy(IDBF, 1, 2));
    CHECK_CLJONIC_NOT(IsDistinctBy(IDBF, a));
    CHECK_CLJONIC_NOT(IsDistinctBy(IDBF, a, r41));
    CHECK_CLJONIC(IsDistinctBy(IDBF, a, a13));
    CHECK_CLJONIC_NOT(IsDistinctBy(IDBF, r14, a13));
    CHECK_CLJONIC(IsDistinctBy(IDBF, r04, a));
    CHECK_CLJONIC_NOT(IsDistinctBy(IDBF, s, s));
    CHECK_CLJONIC_NOT(IsDistinctBy(IDBF, s, s1));
    CHECK_CLJONIC(IsDistinctBy(IDBF, s, s2));
    CHECK_CLJONIC_NOT(IsDistinctBy(IDBF, str, str));
    CHECK_CLJONIC(IsDistinctBy(IDBF, str, str1));
    CHECK_CLJONIC_NOT(IsDistinctBy(IDBFS, "str", "str"));
    CHECK_CLJONIC(IsDistinctBy(IDBFS, "stra", "strb"));
    CHECK_CLJONIC(IsDistinctBy([]() { return true; }, 1, 2));

    DisableNoHeapMessagePrinting();
}
