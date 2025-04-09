#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-iterator.hpp"

using namespace cljonic;

int TimesTenLI(const int x) noexcept
{
    return x * 10;
}

SCENARIO("Iterator", "[CljonicIterator]")
{
    EnableNoHeapMessagePrinting();
    {
        auto i{Iterator{[](const int i) { return 1 + i; }, 0}};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == i.Count());
        auto j{0};
        for (const auto& v : i)
            CHECK_CLJONIC(j++ == v);
    }
    {
        auto i{Iterator{[](const int i) { return 1 + i; }, 11}};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == i.Count());
        auto j{11};
        for (const auto& v : i)
            CHECK_CLJONIC(j++ == v);
    }

    {
        auto i{Iterator{TimesTenLI, 1}};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == i.Count());
        auto j{1};
        for (const auto& v : i)
        {
            CHECK_CLJONIC(j == v);
            j *= 10;
        }
    }
    DisableNoHeapMessagePrinting();
}
