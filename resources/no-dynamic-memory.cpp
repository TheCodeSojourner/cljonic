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
    constexpr auto equal{Equal(a, a)};
    constexpr auto equalby{EqualBy([](const int i, const int j) { return i == j; }, 1, 1, 1, 1)};
    constexpr auto every{Every([](const int i) { return 1 == i; }, a)};
    constexpr auto filter{Filter([](const int i) { return 1 == i; }, a)};
    constexpr auto first{First(a)};
    constexpr auto map{Map([](const int i) { return 1 == i; }, a)};
    constexpr auto partial{Partial([](const int i, const int j) { return i + j; }, 10)};
    constexpr auto reduce2{Reduce([](const int sum, const int i) { return sum + i; }, a)};
    constexpr auto reduce3{Reduce([](const int sum, const int i) { return sum + i; }, 30, a)};
    constexpr auto seq{Seq(a)};
    constexpr auto take{Take(2, a)};
    constexpr auto takelast{TakeLast(2, a)};

    return 0;
}
