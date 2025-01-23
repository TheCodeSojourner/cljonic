#include <cstring>
#include "catch.hpp"
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
    CHECK(IsDistinctBy(IDBF, 1));
    CHECK(not IsDistinctBy(IDBF, 1, 1));
    CHECK(IsDistinctBy(IDBF, 1, 2));
    CHECK(IsDistinctBy(IDBF, a));
    CHECK(not IsDistinctBy(IDBF, a, r41));
    CHECK(IsDistinctBy(IDBF, a, a13));
    CHECK(not IsDistinctBy(IDBF, r14, a13));
    CHECK(IsDistinctBy(IDBF, r04, a));
    CHECK(not IsDistinctBy(IDBF, s, s));
    CHECK(not IsDistinctBy(IDBF, s, s1));
    CHECK(IsDistinctBy(IDBF, s, s2));
    CHECK(not IsDistinctBy(IDBF, str, str));
    CHECK(IsDistinctBy(IDBF, str, str1));
    CHECK(not IsDistinctBy(IDBFS, "str", "str"));
    CHECK(IsDistinctBy(IDBFS, "stra", "strb"));
}
