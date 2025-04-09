#include <type_traits>
#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-repeat.hpp"

using namespace cljonic;

SCENARIO("Repeat", "[CljonicRepeat]")
{
    EnableNoHeapMessagePrinting();
    {
        constexpr auto r{Repeat{1}};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == r.Count());
        auto lastElement{1234};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(1 == element);
            lastElement = element;
        }
        CHECK_CLJONIC(1 == lastElement);
    }

    {
        constexpr auto r{Repeat<5, int>{1}};
        CHECK_CLJONIC(5 == r.Count());
        auto lastElement{1234};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(1 == element);
            lastElement = element;
        }
        CHECK_CLJONIC(1 == lastElement);
    }
    DisableNoHeapMessagePrinting();
}
