#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-dedupe.hpp"
#include "cljonic-core-equal.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Dedupe", "[CljonicCoreDedupe]")
{
    EnableNoHeapMessagePrinting();

    constexpr auto a0{Array<int, 5>{}};
    CHECK_CLJONIC(Equal(Array<int, 0>{}, Dedupe(a0)));

    constexpr auto a1{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, Dedupe(a1)));

    constexpr auto a2{Array{0, 0, 0, 0}};
    CHECK_CLJONIC(Equal(Array{0}, Dedupe(a2)));

    constexpr auto a3{Array{0, 0, 0, 0, 1, 2, 3, 4, 5}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5}, Dedupe(a3)));

    constexpr auto a4{Array{0, 1, 2, 3, 4, 5, 5, 5, 5}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5}, Dedupe(a4)));

    constexpr auto a5{Array{0, 1, 2, 3, 3, 3, 3, 4, 5}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5}, Dedupe(a5)));

    constexpr auto a6{Array{0, 1, 1, 2, 3, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, Dedupe(a6)));

    constexpr auto rng0{Range<1, 10>{}};
    CHECK_CLJONIC(Equal(Array{1, 2, 3, 4, 5, 6, 7, 8, 9}, Dedupe(rng0)));

    constexpr auto rpt0{Repeat<5, int>{11}};
    CHECK_CLJONIC(Equal(Array{11}, Dedupe(rpt0)));

    constexpr auto s0{Set<int, 5>{}};
    CHECK_CLJONIC(Equal(Array<int, 0>{}, Dedupe(s0)));

    constexpr auto s1{Set{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, Dedupe(s1)));

    constexpr auto s2{Set{0, 0, 0, 0}};
    CHECK_CLJONIC(Equal(Array{0}, Dedupe(s2)));

    constexpr auto s3{Set{0, 0, 0, 0, 1, 2, 3, 4, 5}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5}, Dedupe(s3)));

    constexpr auto s4{Set{0, 1, 2, 3, 4, 5, 5, 5, 5}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5}, Dedupe(s4)));

    constexpr auto s5{Set{0, 1, 2, 3, 3, 3, 3, 4, 5}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5}, Dedupe(s5)));

    constexpr auto s6{Set{0, 1, 1, 2, 3, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9}};
    CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, Dedupe(s6)));

    constexpr auto str0{String{""}};
    CHECK_CLJONIC(Equal(Array<char, 0>{}, Dedupe(str0)));

    constexpr auto str1{String{"Ouch"}};
    CHECK_CLJONIC(Equal(Array{'O', 'u', 'c', 'h'}, Dedupe(str1)));

    constexpr auto str2{String{"aaaaa"}};
    CHECK_CLJONIC(Equal(Array{'a'}, Dedupe(str2)));

    constexpr auto str3{String{"aaaa12345"}};
    CHECK_CLJONIC(Equal(Array{'a', '1', '2', '3', '4', '5'}, Dedupe(str3)));

    constexpr auto str4{String{"12345aaaa"}};
    CHECK_CLJONIC(Equal(Array{'1', '2', '3', '4', '5', 'a'}, Dedupe(str4)));

    constexpr auto str5{String{"123aaaa45"}};
    CHECK_CLJONIC(Equal(Array{'1', '2', '3', 'a', '4', '5'}, Dedupe(str5)));

    constexpr auto str6{String{"011233345567789"}};
    CHECK_CLJONIC(Equal(Array{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}, Dedupe(str6)));

    DisableNoHeapMessagePrinting();
}
