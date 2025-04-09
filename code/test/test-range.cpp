#include <type_traits>
#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-range.hpp"

using namespace cljonic;

SCENARIO("Range", "[CljonicRange]")
{
    EnableNoHeapMessagePrinting();
    {
        constexpr auto r{Range{}};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{0}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 1;
            lastElement = element;
        }
        CHECK_CLJONIC((CljonicCollectionMaximumElementCount - 1) == lastElement);
    }

    {
        constexpr auto r{Range<0>{}};
        CHECK_CLJONIC(0 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
    }

    {
        constexpr auto r{Range<-10>{}};
        CHECK_CLJONIC(0 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
    }

    {
        constexpr auto r{Range<10>{}};
        CHECK_CLJONIC(10 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{0}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 1;
            lastElement = element;
        }
        CHECK_CLJONIC(9 == lastElement);
    }

    {
        constexpr auto r{Range<-3, -10>{}};
        CHECK_CLJONIC(0 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
    }

    {
        constexpr auto r{Range<-3, -3>{}};
        CHECK_CLJONIC(0 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
    }

    {
        constexpr auto r{Range<-3, 10>{}};
        CHECK_CLJONIC(13 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{-3}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 1;
            lastElement = element;
        }
        CHECK_CLJONIC(9 == lastElement);
    }

    {
        constexpr auto r{Range<10, 10, 0>{}};
        CHECK_CLJONIC(0 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
    }

    {
        constexpr auto r{Range<10, 20, 0>{}};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(10 == element);
            lastElement = element;
        }
        CHECK_CLJONIC(10 == lastElement);
    }

    {
        constexpr auto r{Range<20, 10, 0>{}};
        CHECK_CLJONIC(CljonicCollectionMaximumElementCount == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(20 == element);
            lastElement = element;
        }
        CHECK_CLJONIC(20 == lastElement);
    }

    {
        constexpr auto r{Range<3, 10, 4>{}};
        CHECK_CLJONIC(2 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{3}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 4;
            lastElement = element;
        }
        CHECK_CLJONIC(7 == lastElement);
    }

    {
        constexpr auto r{Range<3, 10, 3>{}};
        CHECK_CLJONIC(3 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{3}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 3;
            lastElement = element;
        }
        CHECK_CLJONIC(9 == lastElement);
    }

    {
        constexpr auto r{Range<100, 0, -10>{}};
        CHECK_CLJONIC(10 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{100}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += -10;
            lastElement = element;
        }
        CHECK_CLJONIC(10 == lastElement);
    }

    {
        constexpr auto r{Range<10, -10, -1>{}};
        CHECK_CLJONIC(20 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{10}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += -1;
            lastElement = element;
        }
        CHECK_CLJONIC(-9 == lastElement);
    }

    {
        constexpr auto r{Range<10, 0, -1>{}};
        CHECK_CLJONIC(10 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{10}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += -1;
            lastElement = element;
        }
        CHECK_CLJONIC(1 == lastElement);
    }

    {
        constexpr auto r{Range<5, -5, -2>{}};
        CHECK_CLJONIC(5 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{5}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += -2;
            lastElement = element;
        }
        CHECK_CLJONIC(-3 == lastElement);
    }

    {
        constexpr auto r{Range<-5, 5, -1>{}};
        CHECK_CLJONIC(0 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
    }

    {
        constexpr auto r{Range<-1, -10, -2>{}};
        CHECK_CLJONIC(5 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{-1}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += -2;
            lastElement = element;
        }
        CHECK_CLJONIC(-9 == lastElement);
    }

    {
        constexpr auto r{Range<4, 0, -1>{}};
        CHECK_CLJONIC(4 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{4}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += -1;
            lastElement = element;
        }
        CHECK_CLJONIC(1 == lastElement);
    }

    {
        constexpr auto r{Range<0, -5, -1>{}};
        CHECK_CLJONIC(5 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{0}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += -1;
            lastElement = element;
        }
        CHECK_CLJONIC(-4 == lastElement);
    }

    {
        constexpr auto r{Range<10, 0, -15>{}};
        CHECK_CLJONIC(1 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{10}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += -15;
            lastElement = element;
        }
        CHECK_CLJONIC(10 == lastElement);
    }

    {
        constexpr auto r{Range<-5, -10, -6>{}};
        CHECK_CLJONIC(1 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{-5}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += -6;
            lastElement = element;
        }
        CHECK_CLJONIC(-5 == lastElement);
    }

    {
        constexpr auto r{Range<-5, -10, 6>{}};
        CHECK_CLJONIC(0 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
    }

    {
        constexpr auto r{Range<0, 10, 2>{}};
        CHECK_CLJONIC(5 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{0}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 2;
            lastElement = element;
        }
        CHECK_CLJONIC(8 == lastElement);
    }

    {
        constexpr auto r{Range<1, 10, 3>{}};
        CHECK_CLJONIC(3 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{1}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 3;
            lastElement = element;
        }
        CHECK_CLJONIC(7 == lastElement);
    }

    {
        constexpr auto r{Range<5, 6, 1>{}};
        CHECK_CLJONIC(1 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{5}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 1;
            lastElement = element;
        }
        CHECK_CLJONIC(5 == lastElement);
    }

    {
        constexpr auto r{Range<0, 5, 1>{}};
        CHECK_CLJONIC(5 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{0}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 1;
            lastElement = element;
        }
        CHECK_CLJONIC(4 == lastElement);
    }

    {
        constexpr auto r{Range<10, 5, 2>{}};
        CHECK_CLJONIC(0 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
    }

    {
        constexpr auto r{Range<0, 0, 1>{}};
        CHECK_CLJONIC(0 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
    }

    {
        constexpr auto r{Range<0, 10, 20>{}};
        CHECK_CLJONIC(1 == r.Count());
        auto lastElement{TYPE_CLJONIC_RANGE{1234}};
        auto i{TYPE_CLJONIC_RANGE{0}};
        for (const auto& element : r)
        {
            CHECK_CLJONIC(i == element);
            i += 20;
            lastElement = element;
        }
        CHECK_CLJONIC(0 == lastElement);
    }
    DisableNoHeapMessagePrinting();
}
