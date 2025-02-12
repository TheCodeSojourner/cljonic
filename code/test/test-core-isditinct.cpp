#include <cstring>
#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-isdistinct.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("IsDistinct", "[CljonicCoreIsDistinct]")
{
    constexpr auto a{Array{1, 1, 1, 1}};
    constexpr auto a13{Array{1, 2, 3}};
    constexpr auto r14{Range<1, 4>{}};
    constexpr auto r15{Range<1, 5>{}};
    constexpr auto r04{Range<4>{}};
    constexpr auto r41{Repeat<4, int>{1}};
    constexpr auto r42{Repeat<4, int>{2}};
    constexpr auto s{Set{1, 3, 2}};
    constexpr auto s1{Set{3, 1, 2}};
    constexpr auto s2{Set{1, 4, 2}};
    constexpr auto str{String{"abc"}};
    constexpr auto str1{String{"def"}};
    CHECK(IsDistinct(1));
    CHECK(not IsDistinct(1, 1));
    CHECK(IsDistinct(1, 2));
    CHECK(not IsDistinct(a));
    // TODO: CHECK(not IsDistinct(a, r41));
    // TODO: CHECK(IsDistinct(a, a13));
    // TODO: CHECK(not IsDistinct(r14, a13));
    // TODO: CHECK(IsDistinct(r04, a));
    // TODO: CHECK(IsDistinct(r04, r15));
    // TODO: CHECK(IsDistinct(r41, r42));
    CHECK(not IsDistinct(s, s));
    CHECK(not IsDistinct(s, s1));
    CHECK(IsDistinct(s, s2));
    CHECK(not IsDistinct(str, str));
    CHECK(IsDistinct(str, str1));
    // TODO: CHECK(not IsDistinct("str", "str"));
    // TODO: CHECK(IsDistinct("stra", "strb"));
}
