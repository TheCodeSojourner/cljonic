#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-dedupeby.hpp"
#include "cljonic-core-equal.hpp"

using namespace cljonic;
using namespace cljonic::core;

constexpr auto EqualDoubleCollections(const auto& c1, const auto& c2)
{
    auto result{c1.Count() == c2.Count()};
    for (SizeType i{0}; (result and (i < c1.Count())); ++i)
        result = (c1[i] == c2[i]);
    return result;
};

SCENARIO("DedupeBy", "[CljonicCoreDedupeBy]")
{
    constexpr auto EqualInts = [](const int i, const int j) { return i == j; };
    constexpr auto EqualDoubles = [](const double i, const double j) { return i == j; };

    constexpr auto a0{Array<int, 5>{}};
    CHECK(Equal(Array<int, 0>{}, DedupeBy(EqualInts, a0)));

    constexpr auto a1{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, DedupeBy(EqualInts, a1)));

    constexpr auto a2{Array{0, 0, 0, 0}};
    CHECK(Equal(Array{0}, DedupeBy(EqualInts, a2)));

    constexpr auto a3{Array{0, 0, 0, 0, 1, 2, 3, 4, 5}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5}, DedupeBy(EqualInts, a3)));

    constexpr auto a4{Array{0, 1, 2, 3, 4, 5, 5, 5, 5}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5}, DedupeBy(EqualInts, a4)));

    constexpr auto a5{Array{0, 1, 2, 3, 3, 3, 3, 4, 5}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5}, DedupeBy(EqualInts, a5)));

    constexpr auto a6{Array{0, 1, 1, 2, 3, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, DedupeBy(EqualInts, a6)));

    constexpr auto rng0{Range<1, 10>{}};
    CHECK(Equal(Array{1, 2, 3, 4, 5, 6, 7, 8, 9}, DedupeBy(EqualInts, rng0)));

    constexpr auto rpt0{Repeat<5, int>{11}};
    CHECK(Equal(Array{11}, DedupeBy(EqualInts, rpt0)));

    constexpr auto s0{Set<int, 5>{}};
    CHECK(Equal(Array<int, 0>{}, DedupeBy(EqualInts, s0)));

    constexpr auto s1{Set{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, DedupeBy(EqualInts, s1)));

    constexpr auto s2{Set{0, 0, 0, 0}};
    CHECK(Equal(Array{0}, DedupeBy(EqualInts, s2)));

    constexpr auto s3{Set{0, 0, 0, 0, 1, 2, 3, 4, 5}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5}, DedupeBy(EqualInts, s3)));

    constexpr auto s4{Set{0, 1, 2, 3, 4, 5, 5, 5, 5}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5}, DedupeBy(EqualInts, s4)));

    constexpr auto s5{Set{0, 1, 2, 3, 3, 3, 3, 4, 5}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5}, DedupeBy(EqualInts, s5)));

    constexpr auto s6{Set{0, 1, 1, 2, 3, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9}};
    CHECK(Equal(Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, DedupeBy(EqualInts, s6)));

    constexpr auto str0{String{""}};
    CHECK(Equal(Array<char, 0>{}, DedupeBy(EqualInts, str0)));

    constexpr auto str1{String{"Ouch"}};
    CHECK(Equal(Array{'O', 'u', 'c', 'h'}, DedupeBy(EqualInts, str1)));

    constexpr auto str2{String{"aaaaa"}};
    CHECK(Equal(Array{'a'}, DedupeBy(EqualInts, str2)));

    constexpr auto str3{String{"aaaa12345"}};
    CHECK(Equal(Array{'a', '1', '2', '3', '4', '5'}, DedupeBy(EqualInts, str3)));

    constexpr auto str4{String{"12345aaaa"}};
    CHECK(Equal(Array{'1', '2', '3', '4', '5', 'a'}, DedupeBy(EqualInts, str4)));

    constexpr auto str5{String{"123aaaa45"}};
    CHECK(Equal(Array{'1', '2', '3', 'a', '4', '5'}, DedupeBy(EqualInts, str5)));

    constexpr auto str6{String{"011233345567789"}};
    CHECK(Equal(Array{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}, DedupeBy(EqualInts, str6)));

    constexpr auto d0{Array<double, 5>{}};
    CHECK(EqualDoubleCollections(Array<double, 0>{}, DedupeBy(EqualDoubles, d0)));

    constexpr auto d1{Array{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0}};
    CHECK(EqualDoubleCollections(Array{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0}, DedupeBy(EqualDoubles, d1)));

    constexpr auto d2{Array{0.0, 0.0, 0.0, 0.0}};
    CHECK(EqualDoubleCollections(Array{0.0}, DedupeBy(EqualDoubles, d2)));

    constexpr auto d3{Array{0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0}};
    CHECK(EqualDoubleCollections(Array{0.0, 1.0, 2.0, 3.0, 4.0, 5.0}, DedupeBy(EqualDoubles, d3)));

    constexpr auto d4{Array{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 5.0, 5.0, 5.0}};
    CHECK(EqualDoubleCollections(Array{0.0, 1.0, 2.0, 3.0, 4.0, 5.0}, DedupeBy(EqualDoubles, d4)));

    constexpr auto d5{Array{0.0, 1.0, 2.0, 3.0, 3.0, 3.0, 3.0, 4.0, 5.0}};
    CHECK(EqualDoubleCollections(Array{0.0, 1.0, 2.0, 3.0, 4.0, 5.0}, DedupeBy(EqualDoubles, d5)));

    constexpr auto d6{Array{0.0, 1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 5.0, 5.0, 6.0, 7.0, 7.0, 8.0, 9.0}};
    CHECK(EqualDoubleCollections(Array{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0}, DedupeBy(EqualDoubles, d6)));
}
