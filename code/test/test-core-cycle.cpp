#include <limits>
#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-cycle.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Cycle", "[CljonicCoreCycle]")
{
    EnableNoHeapMessagePrinting();
    {
        const auto a{Array<int, 4>{11, 12, 13, 14}};
        const auto c{Cycle(a)};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(12 == *it);
        ++it;
        CHECK_CLJONIC(13 == *it);
        ++it;
        CHECK_CLJONIC(14 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
    }

    {
        const auto c{Cycle(Array<int, 4>{11, 12, 13, 14})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(12 == *it);
        ++it;
        CHECK_CLJONIC(13 == *it);
        ++it;
        CHECK_CLJONIC(14 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
    }

    {
        const auto c{Cycle(Array<int, 0>{})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
    }
    {
        const auto i{Iterator{[](const int i) { return i + 1; }, 1}};
        const auto c{Cycle(i)};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(1 == *it);
        ++it;
        CHECK_CLJONIC(2 == *it);
        ++it;
        CHECK_CLJONIC(3 == *it);
        ++it;
        CHECK_CLJONIC(4 == *it);
        ++it;
        CHECK_CLJONIC(5 == *it);
    }

    {
        const auto c{Cycle(Iterator{[](const int i) { return i + 1; }, 1})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(1 == *it);
        ++it;
        CHECK_CLJONIC(2 == *it);
        ++it;
        CHECK_CLJONIC(3 == *it);
        ++it;
        CHECK_CLJONIC(4 == *it);
        ++it;
        CHECK_CLJONIC(5 == *it);
    }

    {
        constexpr auto r{Range<1, 5>{}};
        const auto c{Cycle(r)};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(1 == *it);
        ++it;
        CHECK_CLJONIC(2 == *it);
        ++it;
        CHECK_CLJONIC(3 == *it);
        ++it;
        CHECK_CLJONIC(4 == *it);
        ++it;
        CHECK_CLJONIC(1 == *it);
    }

    {
        const auto c{Cycle(Range<1, 5>{})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(1 == *it);
        ++it;
        CHECK_CLJONIC(2 == *it);
        ++it;
        CHECK_CLJONIC(3 == *it);
        ++it;
        CHECK_CLJONIC(4 == *it);
        ++it;
        CHECK_CLJONIC(1 == *it);
    }

    {
        const auto c{Cycle(Range<0>{})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
    }

    {
        constexpr auto r{Repeat<3, int>{11}};
        const auto c{Cycle(r)};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
    }

    {
        const auto c{Cycle(Repeat<3, int>{11})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
        ++it;
        CHECK_CLJONIC(11 == *it);
    }

    {
        const auto c{Cycle(Repeat<0, int>{11})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
    }

    {
        constexpr auto s{Set{1, 2, 1, 3, 4}};
        const auto c{Cycle(s)};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(1 == *it);
        ++it;
        CHECK_CLJONIC(2 == *it);
        ++it;
        CHECK_CLJONIC(3 == *it);
        ++it;
        CHECK_CLJONIC(4 == *it);
        ++it;
        CHECK_CLJONIC(1 == *it);
    }

    {
        const auto c{Cycle(Set{1, 2, 1, 3, 4})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(1 == *it);
        ++it;
        CHECK_CLJONIC(2 == *it);
        ++it;
        CHECK_CLJONIC(3 == *it);
        ++it;
        CHECK_CLJONIC(4 == *it);
        ++it;
        CHECK_CLJONIC(1 == *it);
    }

    {
        const auto c{Cycle(Set<int, 2>{})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
        ++it;
        CHECK_CLJONIC(0 == *it);
    }

    {
        constexpr auto s{String{"Hello"}};
        const auto c{Cycle(s)};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC('H' == *it);
        ++it;
        CHECK_CLJONIC('e' == *it);
        ++it;
        CHECK_CLJONIC('l' == *it);
        ++it;
        CHECK_CLJONIC('l' == *it);
        ++it;
        CHECK_CLJONIC('o' == *it);
        ++it;
        CHECK_CLJONIC('H' == *it);
        ++it;
        CHECK_CLJONIC('e' == *it);
    }

    {
        const auto c{Cycle(String{"Hello"})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC('H' == *it);
        ++it;
        CHECK_CLJONIC('e' == *it);
        ++it;
        CHECK_CLJONIC('l' == *it);
        ++it;
        CHECK_CLJONIC('l' == *it);
        ++it;
        CHECK_CLJONIC('o' == *it);
        ++it;
        CHECK_CLJONIC('H' == *it);
        ++it;
        CHECK_CLJONIC('e' == *it);
    }

    {
        const auto c{Cycle(String{""})};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == c.Count());
        auto it{c.begin()};
        CHECK_CLJONIC('\0' == *it);
        ++it;
        CHECK_CLJONIC('\0' == *it);
        ++it;
        CHECK_CLJONIC('\0' == *it);
        ++it;
        CHECK_CLJONIC('\0' == *it);
        ++it;
        CHECK_CLJONIC('\0' == *it);
        ++it;
        CHECK_CLJONIC('\0' == *it);
        ++it;
        CHECK_CLJONIC('\0' == *it);
    }
    DisableNoHeapMessagePrinting();
}
