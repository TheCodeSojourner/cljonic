#include <type_traits>
#include "catch.hpp"
#include "cljonic-collection-maximum-element-count.hpp"
#include "cljonic-repeat.hpp"

using namespace cljonic;

SCENARIO("Repeat", "[CljonicRepeat]")
{
    constexpr auto r{Repeat{1}};
    auto MAX_INDEX{CljonicCollectionMaximumElementCount};

    {
        constexpr auto r{Repeat{1}};
        CHECK(MAX_INDEX == r.Count());
        CHECK(1 == r[0]);
        CHECK(1 == r[1]);
        CHECK(1 == r[MAX_INDEX - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INDEX]);
        CHECK(0 == r.DefaultElement());
    }

    {
        constexpr auto r{Repeat<5, int>{1}};
        CHECK(5 == r.Count());
        CHECK(1 == r[0]);
        CHECK(1 == r[1]);
        CHECK(1 == r[2]);
        CHECK(1 == r[3]);
        CHECK(1 == r[4]);
        CHECK(0 == r[5]);
        CHECK(0 == r[MAX_INDEX - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INDEX]);
    }

    {
        constexpr auto r{Repeat<5, int>{1}};
        for (const auto& element : r)
        {
            CHECK(1 == element);
        }
    }
}
