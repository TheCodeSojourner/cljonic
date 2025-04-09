#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-conj.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Conj", "[CljonicCoreConj]")
{
    EnableNoHeapMessagePrinting();
    {
        constexpr auto a{Array{11, 12, 13, 14}};
        constexpr auto cA{Conj(a, 15, 16)};
        CHECK_CLJONIC(6 == cA.Count());
        auto expected{11};
        for (const auto& i : cA)
        {
            CHECK_CLJONIC(i == expected);
            ++expected;
        }
    }

    {
        constexpr auto rng{Range<1, 5>{}};
        constexpr auto cRng{Conj(rng, 5)};
        CHECK_CLJONIC(5 == cRng.Count());
        auto expected{1};
        for (const auto& i : cRng)
        {
            CHECK_CLJONIC(i == expected);
            ++expected;
        }
    }

    {
        constexpr auto rpt{Repeat<3, int>{11}};
        constexpr auto cRpt{Conj(rpt, 11)};
        CHECK_CLJONIC(4 == cRpt.Count());
        auto expected{11};
        for (const auto& i : cRpt)
            CHECK_CLJONIC(i == expected);
    }

    {
        constexpr auto set{Set{1, 2, 1, 3, 4}};
        constexpr auto cSet{Conj(set, 5)};
        CHECK_CLJONIC(5 == cSet.Count());
        auto expected{1};
        for (const auto& i : cSet)
        {
            CHECK_CLJONIC(i == expected);
            ++expected;
        }
    }

    {
        constexpr auto str{String{"Hello"}};
        constexpr auto hello{"Hello"};

        constexpr auto cStr{Conj(str)};
        CHECK_CLJONIC(5 == cStr.Count());
        auto expected{hello};
        for (const auto& i : cStr)
        {
            CHECK_CLJONIC(i == *expected);
            ++expected;
        }
    }

    {
        constexpr auto str{String{"Hello"}};
        constexpr auto hello{"Hello!!!"};

        constexpr auto cStrBang{Conj(str, '!', '!', '!')};
        CHECK_CLJONIC(8 == cStrBang.Count());
        auto expected{hello};
        for (const auto& i : cStrBang)
        {
            CHECK_CLJONIC(i == *expected);
            ++expected;
        }
    }
    DisableNoHeapMessagePrinting();
}
