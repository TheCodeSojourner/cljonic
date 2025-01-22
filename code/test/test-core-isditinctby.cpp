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
    constexpr auto EBF = [](const int a, const int b) { return a == b; };
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
    CHECK(IsDistinctBy(EBF, 1));
    CHECK(not IsDistinctBy(EBF, 1, 1));
    CHECK(IsDistinctBy(EBF, 1, 2));
    CHECK(IsDistinctBy(EBF, a));
    CHECK(not IsDistinctBy(EBF, a, r41));
    CHECK(IsDistinctBy(EBF, a, a13));
    CHECK(not IsDistinctBy(EBF, r14, a13));
    CHECK(IsDistinctBy(EBF, r04, a));
    CHECK(not IsDistinctBy(EBF, s, s));
    CHECK(not IsDistinctBy(EBF, s, s1));
    CHECK(IsDistinctBy(EBF, s, s2));
    CHECK(not IsDistinctBy(EBF, str, str));
    CHECK(IsDistinctBy(EBF, str, str1));
}
