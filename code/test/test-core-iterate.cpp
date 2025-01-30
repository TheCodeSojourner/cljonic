#include "catch.hpp"
#include "cljonic-core-iterate.hpp"

using namespace cljonic;
using namespace cljonic::core;

int TimesTenI(const int x) noexcept
{
    return x * 10;
}

SCENARIO("Iterate", "[CljonicCoreIterate]")
{
    {
        auto i{Iterate([](const int i) { return 1 + i; }, 11)};
        auto j{11};
        for (const auto& v : i)
            CHECK(j++ == v);
        j = 11;
        for (const auto& v : i)
            CHECK(j++ == v);
    }

    {
        auto i{Iterate(TimesTenI, 1)};
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
