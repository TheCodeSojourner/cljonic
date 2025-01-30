#include "catch.hpp"
#include "cljonic-iterator.hpp"

using namespace cljonic;

int TimesTenLI(const int x) noexcept
{
    return x * 10;
}

SCENARIO("Iterator", "[CljonicIterator]")
{
    {
        auto i{Iterator{[](const int i) { return 1 + i; }, 11}};
        auto j{11};
        for (const auto& v : i)
            CHECK(j++ == v);
        j = 11;
        for (const auto& v : i)
            CHECK(j++ == v);
    }

    {
        auto i{Iterator{TimesTenLI, 1}};
        auto j{1};
        for (const auto& v : i)
        {
            CHECK(j == v);
            j *= 10;
        }
        j = 1;
        for (const auto& v : i)
        {
            CHECK(j == v);
            j *= 10;
        }
    }
}
