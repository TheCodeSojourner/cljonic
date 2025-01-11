#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-shared.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"
#include "cljonic-core-replace.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Replace", "[CljonicCoreReplace]")
{
    constexpr auto a{Array{1, 2, 3, 4}};
    CHECK(Equal(Array{2, 3, 4, 4}, Replace(a, Array{1, 2, 3, 4})));
    CHECK(Equal(Array{2, 3, 4, 14}, Replace(a, Array{1, 2, 3, 14})));
    CHECK(Equal(Array{-2, 3, 4, 14}, Replace(a, Array{-2, 2, 3, 14})));
    CHECK(Equal(Array{11, 12, 13, 14}, Replace(a, Array{11, 12, 13, 14})));
    CHECK(Equal(Array{4}, Replace(a, Array{3})));
    CHECK(Equal(Array<int, 0>{}, Replace(a, Range<0>{})));
    CHECK(Equal(Array{1, 2, 3, 4, 4, 5, 6}, Replace(a, Range<7>{})));
    CHECK(Equal(Array{4, 4, 4, 4}, Replace(a, Repeat<4, int>{3})));
    CHECK(Equal(Array{33, 33, 33, 33}, Replace(a, Repeat<4, int>{33})));
    CHECK(Equal(Array{2, 3, 4, 4}, Replace(a, Set{1, 2, 3, 4})));
    CHECK(Equal(Array{49, 50, 51, 52}, Replace(a, String{'1', '2', '3', '4'})));

    constexpr auto ebf{[](const auto a, const auto b) { return AreEqual(a, b); }};
    constexpr auto f{Array{1.1, 2.2, 3.3, 4.4}};
    CHECK(EqualBy(ebf, Array{2.2, 3.3, 4.4, 4.0}, Replace(f, Array{1, 2, 3, 4})));
    CHECK(EqualBy(ebf, Array{2.2, 3.3, 4.4, 14.0}, Replace(f, Array{1, 2, 3, 14})));
    CHECK(EqualBy(ebf, Array{-2.0, 3.3, 4.4, 14.0}, Replace(f, Array{-2, 2, 3, 14})));
    CHECK(EqualBy(ebf, Array{11.0, 12.0, 13.0, 14.0}, Replace(f, Array{11, 12, 13, 14})));
    CHECK(EqualBy(ebf, Array{4.4}, Replace(f, Array{3})));
    CHECK(EqualBy(ebf, Array<int, 0>{}, Replace(f, Range<0>{})));
    CHECK(EqualBy(ebf, Array{1.1, 2.2, 3.3, 4.4, 4.0, 5.0, 6.0}, Replace(f, Range<7>{})));
    CHECK(EqualBy(ebf, Array{4.4, 4.4, 4.4, 4.4}, Replace(f, Repeat<4, int>{3})));
    CHECK(EqualBy(ebf, Array{33.0, 33.0, 33.0, 33.0}, Replace(f, Repeat<4, int>{33})));
    CHECK(EqualBy(ebf, Array{2.2, 3.3, 4.4, 4.0}, Replace(f, Set{1, 2, 3, 4})));
    CHECK(EqualBy(ebf, Array{49.0, 50.0, 51.0, 52.0}, Replace(f, String{'1', '2', '3', '4'})));
}
