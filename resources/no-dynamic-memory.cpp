#include "../cljonic/resources/no-dynamic-memory.hpp"
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array<int, 3>{1, 2, 3}};
    constexpr auto rng{Range<1, 5>{}};
    constexpr auto rpt{Repeat{1}};
    constexpr auto set{Set{11, 12, 13}};
    constexpr auto str{String{"Hello"}};

    constexpr auto compose{Compose(a, a)};
    constexpr auto concat0{Concat()};
    constexpr auto concat1{Concat(a, a)};
    constexpr auto conj{Conj(a, 11)};
    constexpr auto count{Count(a)};
    constexpr auto cycle{Cycle(a)};
    constexpr auto dedupe{Dedupe(a)};
    constexpr auto dedupeby{DedupeBy([](const int i, const int j) { return i == j; }, a)};
    constexpr auto defaultelement{DefaultElement(a)};
    constexpr auto drop{Drop(1, a)};
    constexpr auto droplast{DropLast(1, a)};
    constexpr auto dropwhile{DropWhile([](const int i) { return true; }, a)};
    constexpr auto equal{Equal(a, a)};
    constexpr auto equalby{EqualBy([](const int i, const int j) { return i == j; }, 1, 1, 1, 1)};
    constexpr auto every{Every([](const int i) { return 1 == i; }, a)};
    constexpr auto filter{Filter([](const int i) { return 1 == i; }, a)};
    constexpr auto first{First(a)};
    constexpr auto identical{Identical(a)};
    const auto identity{Identity(a)};
    constexpr auto inc{Inc(1)};
    constexpr auto indexof{IndexOf(Array{1, 2, 3}, 1)};
    constexpr auto indexofby{IndexOfBy([](const int i, const int j) { return i == j; }, Array{1, 2, 3}, 1)};
    constexpr auto interleave0{Interleave()};
    constexpr auto interleave1{Interleave(a)};
    constexpr auto interpose{Interpose(1, Array{1, 2, 3})};
    constexpr auto isdistinct0{IsDistinct(a)};
    constexpr auto isdistinct1{IsDistinct(1, 1, 1)};
    constexpr auto isdistinctby0{IsDistinctBy([](const int i, const int j) { return i == j; }, a)};
    constexpr auto isdistinctby1{IsDistinctBy([](const int i, const int j) { return i == j; }, 1, 1, 1)};
    constexpr auto isempty{IsEmpty(a)};
    constexpr auto isfull{IsFull(a)};
    constexpr auto iterate{Iterate([](const int i) { return 1 + i; }, 1)};
    constexpr auto last{Last(a)};
    constexpr auto lastindexof{LastIndexOf(a, 5)};
    constexpr auto lastindexofby{LastIndexOfBy([](const int i, const int j) { return j == i; }, a, 5)};
    constexpr auto map{Map([](const int i) { return 1 == i; }, a)};
    constexpr auto max1{Max(a)};
    constexpr auto max2{Max(4, 1, 5, 3)};
    constexpr auto maxby1{MaxBy([](const int i, const int j) { return i < j; }, a)};
    constexpr auto maxby2{MaxBy([](const int i, const int j) { return i < j; }, 4, 1, 5, 3)};
    constexpr auto min1{Min(a)};
    constexpr auto min2{Min(4, 1, 5, 3)};
    constexpr auto minby1{MinBy([](const int i, const int j) { return i < j; }, a)};
    constexpr auto minby2{MinBy([](const int i, const int j) { return i < j; }, 4, 1, 5, 3)};
    constexpr auto notany{NotAny([](const int i) { return 1 == i; }, a)};
    constexpr auto notevery{NotEvery([](const int i) { return 1 == i; }, a)};
    constexpr auto nth1{Nth(a, 1)};
    constexpr auto nth2{Nth(a, 1, 11)};
    constexpr auto partial{Partial([](const int i, const int j) { return i + j; }, 10)};
    constexpr auto reduce2{Reduce([](const int sum, const int i) { return sum + i; }, a)};
    constexpr auto reduce3{Reduce([](const int sum, const int i) { return sum + i; }, 30, a)};
    constexpr auto remove{Remove([](const int i) { return 1 == i; }, a)};
    constexpr auto replace{Replace(a, Array{1, 2, 3, 20})};
    constexpr auto reverse{Reverse(a)};
    constexpr auto second{Second(a)};
    constexpr auto seq{Seq(a)};
    constexpr auto size{Size(a)};
    constexpr auto some{Some([](const int i) { return 20 == i; }, a)};
    constexpr auto sort{Sort(a)};
    constexpr auto sortby{SortBy([](const int i, const int j) { return i < j; }, a)};
    constexpr auto splitat{SplitAt(2, a)};
    constexpr auto splitwith{SplitWith([](const int i) { return true; }, a)};
    constexpr auto subs_1{Subs(a, 0, 2)};
    constexpr auto subs_2{Subs(a, 0)};
    constexpr auto take{Take(2, a)};
    constexpr auto takelast{TakeLast(2, a)};
    constexpr auto takenth{TakeNth(2, a)};
    constexpr auto takewhile{TakeWhile([](const int i) { return true; }, a)};

    return 0;
}
