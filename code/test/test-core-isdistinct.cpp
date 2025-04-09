#include <cstring>
#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-isdistinct.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("IsDistinct", "[CljonicCoreIsDistinct]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a{Array{1, 1, 1, 1}};
    constexpr auto a13{Array{1, 2, 3}};
    const auto itr1{Iterator([](const int i) { return i + 1; }, 1)};
    const auto itr2{Iterator([](const int i) { return (1 == i) ? 2 : 1; }, 1)};
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
    CHECK_CLJONIC(IsDistinct(1));
    CHECK_CLJONIC_NOT(IsDistinct(1, 1));
    CHECK_CLJONIC(IsDistinct(1, 2));
    CHECK_CLJONIC_NOT(IsDistinct(a));
    CHECK_CLJONIC(IsDistinct(a, a13));
    CHECK_CLJONIC(IsDistinct(a, itr1));
    CHECK_CLJONIC_NOT(IsDistinct(a13, itr1));
    CHECK_CLJONIC_NOT(IsDistinct(a, r41));
    CHECK_CLJONIC(IsDistinct(itr2, itr1));
    CHECK_CLJONIC_NOT(IsDistinct(itr2));
    CHECK_CLJONIC(IsDistinct(itr1));
    CHECK_CLJONIC_NOT(IsDistinct(r14, a13));
    CHECK_CLJONIC(IsDistinct(r04, a));
    CHECK_CLJONIC(IsDistinct(r04, r15));
    CHECK_CLJONIC(IsDistinct(r41, r42));
    CHECK_CLJONIC_NOT(IsDistinct(s, s));
    CHECK_CLJONIC_NOT(IsDistinct(s, s1));
    CHECK_CLJONIC(IsDistinct(s, s2));
    CHECK_CLJONIC_NOT(IsDistinct(str, str));
    CHECK_CLJONIC(IsDistinct(str, str1));
    CHECK_CLJONIC_NOT(IsDistinct("str", "str"));
    CHECK_CLJONIC(IsDistinct("stra", "strb"));
    CHECK_CLJONIC(IsDistinct(a, 1, itr1, '1', r04, "Hello", r42, s, str));
    CHECK_CLJONIC_NOT(IsDistinct(a, 1, itr1, '1', r04, "Hello", r42, s, 1, str));

    DisableNoHeapMessagePrinting();
}
