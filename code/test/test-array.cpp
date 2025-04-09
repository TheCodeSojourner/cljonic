#include <string>
#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"

using namespace cljonic;

SCENARIO("Array", "[CljonicArray]")
{
    EnableNoHeapMessagePrinting();
    {
        constexpr auto a0{Array<int, 10>{}};
        constexpr auto a1{Array<int, 10>{1, 2, 3, 4}};
        constexpr auto a2{Array<int, 4>{1, 2, 3, 4}};
        constexpr auto a3{Array<int, 4>{1, 2, 3, 4, 5, 6}};
        constexpr auto a4{Array{1, 2, 3, 4}};

        CHECK_CLJONIC(0 == a0.Count());
        for ([[maybe_unused]] const auto& element : a0)
            CHECK_CLJONIC(false);

        CHECK_CLJONIC(4 == a1.Count());
        auto e1{1};
        for (const auto& element : a1)
            CHECK_CLJONIC(e1++ == element);

        CHECK_CLJONIC(4 == a2.Count());
        auto e2{1};
        for (const auto& element : a1)
            CHECK_CLJONIC(e2++ == element);

        CHECK_CLJONIC(4 == a3.Count());
        auto e3{1};
        for (const auto& element : a1)
            CHECK_CLJONIC(e3++ == element);

        CHECK_CLJONIC(4 == a4.Count());
        auto e4{1};
        for (const auto& element : a1)
            CHECK_CLJONIC(e4++ == element);
    }

    {
        constexpr auto a0{Array<float, 10>{}};
        CHECK_CLJONIC(0 == a0.Count());
        for ([[maybe_unused]] const auto& element : a0)
            CHECK_CLJONIC(false);

        constexpr auto a1{Array<float, 10>{1.1, 2.1, 3.1, 4.1}};
        CHECK_CLJONIC(4 == a1.Count());
        auto e1{1.1};
        for (const auto& element : a1)
        {
            CHECK_CLJONIC(e1 == Approx(element).epsilon(0.0001));
            e1 += 1.0;
        }

        constexpr auto a2{Array<float, 4>{1.1, 2.1, 3.1, 4.1}};
        CHECK_CLJONIC(4 == a2.Count());
        auto e2{1.1};
        for (const auto& element : a1)
        {
            CHECK_CLJONIC(e2 == Approx(element).epsilon(0.0001));
            e2 += 1.0;
        }

        constexpr auto a3{Array<float, 4>{1.1, 2.1, 3.1, 4.1, 5.1, 6.1}};
        CHECK_CLJONIC(4 == a3.Count());
        auto e3{1.1};
        for (const auto& element : a1)
        {
            CHECK_CLJONIC(e3 == Approx(element).epsilon(0.0001));
            e3 += 1.0;
        }

        constexpr auto a4{Array{1.1f, 2.1f, 3.1f, 4.1f}};
        CHECK_CLJONIC(4 == a4.Count());
        auto e4{1.1};
        for (const auto& element : a1)
        {
            CHECK_CLJONIC(e4 == Approx(element).epsilon(0.0001));
            e4 += 1.0;
        }
    }

    {
        constexpr auto a0{Array<double, 10>{}};
        constexpr auto a1{Array<double, 10>{1.1, 2.1, 3.1, 4.1}};
        constexpr auto a2{Array<double, 4>{1.1, 2.1, 3.1, 4.1}};
        constexpr auto a3{Array<double, 4>{1.1, 2.1, 3.1, 4.1, 5.1, 6.1}};
        constexpr auto a4{Array{1.1d, 2.1d, 3.1d, 4.1d}};

        CHECK_CLJONIC(0 == a0.Count());
        for ([[maybe_unused]] const auto& element : a0)
            CHECK_CLJONIC(false);

        CHECK_CLJONIC(4 == a1.Count());
        auto e1{1.1};
        for (const auto& element : a1)
        {
            CHECK_CLJONIC(e1 == Approx(element).epsilon(0.0001));
            e1 += 1.0;
        }

        CHECK_CLJONIC(4 == a2.Count());
        auto e2{1.1};
        for (const auto& element : a1)
        {
            CHECK_CLJONIC(e2 == Approx(element).epsilon(0.0001));
            e2 += 1.0;
        }
        CHECK_CLJONIC(4 == a3.Count());
        auto e3{1.1};
        for (const auto& element : a1)
        {
            CHECK_CLJONIC(e3 == Approx(element).epsilon(0.0001));
            e3 += 1.0;
        }
        CHECK_CLJONIC(4 == a4.Count());
        auto e4{1.1};
        for (const auto& element : a1)
        {
            CHECK_CLJONIC(e4 == Approx(element).epsilon(0.0001));
            e4 += 1.0;
        }
    }

    {
        constexpr auto a0{Array<const char*, 10>{}};
        constexpr auto a1{Array<const char*, 10>{"1", "2", "3", "4"}};
        constexpr auto a2{Array<const char*, 4>{"1", "2", "3", "4"}};
        constexpr auto a3{Array<const char*, 4>{"1", "2", "3", "4", "5", "6"}};
        constexpr auto a4{Array{"1", "2", "3", "4"}};

        CHECK_CLJONIC(0 == a0.Count());
        for ([[maybe_unused]] const auto& element : a0)
            CHECK_CLJONIC(false);

        CHECK_CLJONIC(4 == a1.Count());
        auto e1{1};
        for (const auto& element : a1)
            CHECK_CLJONIC(std::to_string(e1++) == element);

        CHECK_CLJONIC(4 == a2.Count());
        auto e2{1};
        for (const auto& element : a1)
            CHECK_CLJONIC(std::to_string(e2++) == element);

        CHECK_CLJONIC(4 == a3.Count());
        auto e3{1};
        for (const auto& element : a1)
            CHECK_CLJONIC(std::to_string(e3++) == element);

        CHECK_CLJONIC(4 == a4.Count());
        auto e4{1};
        for (const auto& element : a1)
            CHECK_CLJONIC(std::to_string(e4++) == element);
    }

    {
        // an Array is a function of its index
        constexpr auto a{Array<int, 10>{1, 2, 3, 4}};
        CHECK_CLJONIC(1 == a(0));
        CHECK_CLJONIC(2 == a(1));
        CHECK_CLJONIC(3 == a(2));
        CHECK_CLJONIC(4 == a(3));
        CHECK_CLJONIC(0 == a(4)); // index out-of-bounds so return default element, which is 0 in this case
    }
    DisableNoHeapMessagePrinting();
}
