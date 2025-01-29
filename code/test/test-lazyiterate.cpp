#include "catch.hpp"
#include "cljonic-lazyiterate.hpp"

using namespace cljonic;

int TimesTenLI(const int x) noexcept
{
    return x * 10;
}

SCENARIO("LazyIterate", "[CljonicLazyIterate]")
{
    {
        auto i{LazyIterate{[](const int i) { return 1 + i; }, 11}};
        CHECK(11 == i.Next());
        CHECK(12 == i.Next());
        CHECK(13 == i.Next());
        CHECK(14 == i.Next());
    }

    {
        auto i{LazyIterate{TimesTenLI, 1}};
        CHECK(1 == i.Next());
        CHECK(10 == i.Next());
        CHECK(100 == i.Next());
        CHECK(1000 == i.Next());
    }
}
