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
        CHECK(11 == i.Next());
        CHECK(12 == i.Next());
        CHECK(13 == i.Next());
        CHECK(14 == i.Next());
    }

    {
        auto i{Iterate(TimesTenI, 1)};
        CHECK(1 == i.Next());
        CHECK(10 == i.Next());
        CHECK(100 == i.Next());
        CHECK(1000 == i.Next());
    }
}
