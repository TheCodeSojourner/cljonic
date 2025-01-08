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
    constexpr auto concat{Concat(a, a)};
    constexpr auto conj{Conj(a, 11)};
    constexpr auto count{Count(a)};
    const auto cycle{Cycle(a)};
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
    const auto identical{Identical(a)};
    const auto identity{Identity(a)};
    constexpr auto map{Map([](const int i) { return 1 == i; }, a)};
    constexpr auto partial{Partial([](const int i, const int j) { return i + j; }, 10)};
    constexpr auto reduce2{Reduce([](const int sum, const int i) { return sum + i; }, a)};
    constexpr auto reduce3{Reduce([](const int sum, const int i) { return sum + i; }, 30, a)};
    constexpr auto second{Second(a)};
    constexpr auto seq{Seq(a)};
    constexpr auto size{Size(a)};
    constexpr auto some{Some([](const int i) { return 20 == i; }, a)};
    constexpr auto sort{Sort(a)};
    constexpr auto sortby{SortBy([](const int i, const int j) { return i < j; }, a)};
    constexpr auto splitby{SplitBy([](const int i) { return true; }, a)};
    constexpr auto subs_1{Subs(a, 0, 2)};
    constexpr auto subs_2{Subs(a, 0)};
    constexpr auto take{Take(2, a)};
    constexpr auto takelast{TakeLast(2, a)};
    constexpr auto takenth{TakeNth(2, a)};
    constexpr auto takewhile{TakeWhile([](const int i) { return true; }, a)};

    return 0;
}
