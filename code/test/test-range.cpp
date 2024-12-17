#include <type_traits>
#include "catch.hpp"
#include "cljonic-range.hpp"

using namespace cljonic;

static constexpr int MAX_INT{std::numeric_limits<int>::max()};

SCENARIO("Range", "[CljonicRange]")
{
    {
        const auto r{Range()};
        CHECK(MAX_INT == r.Count());
        CHECK(0 == r[0]);
        CHECK(1 == r[1]);
        CHECK((MAX_INT - 1) == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(0)};
        CHECK(0 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[1]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(-10)};
        CHECK(0 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[1]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(10)};
        CHECK(10 == r.Count());
        CHECK(0 == r[0]);
        CHECK(1 == r[1]);
        CHECK(2 == r[2]);
        CHECK(3 == r[3]);
        CHECK(4 == r[4]);
        CHECK(5 == r[5]);
        CHECK(6 == r[6]);
        CHECK(7 == r[7]);
        CHECK(8 == r[8]);
        CHECK(9 == r[9]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);

        auto i{0};
        for (const auto& element : r)
        {
            CHECK(i == element);
            ++i;
        }
    }

    {
        const auto r{Range(-3, -10)};
        CHECK(0 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[1]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(-3, -3)};
        CHECK(0 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[1]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(-3, 10)};
        CHECK(13 == r.Count());
        CHECK(-3 == r[0]);
        CHECK(-2 == r[1]);
        CHECK(-1 == r[2]);
        CHECK(0 == r[3]);
        CHECK(1 == r[4]);
        CHECK(2 == r[5]);
        CHECK(3 == r[6]);
        CHECK(4 == r[7]);
        CHECK(5 == r[8]);
        CHECK(6 == r[9]);
        CHECK(7 == r[10]);
        CHECK(8 == r[11]);
        CHECK(9 == r[12]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(10, 10, 0)};
        CHECK(0 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[1]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(10, 20, 0)};
        CHECK(MAX_INT == r.Count());
        CHECK(10 == r[0]);
        CHECK(10 == r[1]);
        CHECK(10 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(20, 10, 0)};
        CHECK(MAX_INT == r.Count());
        CHECK(20 == r[0]);
        CHECK(20 == r[1]);
        CHECK(20 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(3, 10, 4)};
        CHECK(2 == r.Count());
        CHECK(3 == r[0]);
        CHECK(7 == r[1]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(3, 10, 3)};
        CHECK(3 == r.Count());
        CHECK(3 == r[0]);
        CHECK(6 == r[1]);
        CHECK(9 == r[2]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(100, 0, -10)};
        CHECK(10 == r.Count());
        CHECK(100 == r[0]);
        CHECK(90 == r[1]);
        CHECK(80 == r[2]);
        CHECK(70 == r[3]);
        CHECK(60 == r[4]);
        CHECK(50 == r[5]);
        CHECK(40 == r[6]);
        CHECK(30 == r[7]);
        CHECK(20 == r[8]);
        CHECK(10 == r[9]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(10, -10, -1)};
        CHECK(20 == r.Count());
        CHECK(10 == r[0]);
        CHECK(9 == r[1]);
        CHECK(8 == r[2]);
        CHECK(7 == r[3]);
        CHECK(6 == r[4]);
        CHECK(5 == r[5]);
        CHECK(4 == r[6]);
        CHECK(3 == r[7]);
        CHECK(2 == r[8]);
        CHECK(1 == r[9]);
        CHECK(0 == r[10]);
        CHECK(-1 == r[11]);
        CHECK(-2 == r[12]);
        CHECK(-3 == r[13]);
        CHECK(-4 == r[14]);
        CHECK(-5 == r[15]);
        CHECK(-6 == r[16]);
        CHECK(-7 == r[17]);
        CHECK(-8 == r[18]);
        CHECK(-9 == r[19]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(10, 0, -1)};
        CHECK(10 == r.Count());
        CHECK(10 == r[0]);
        CHECK(9 == r[1]);
        CHECK(8 == r[2]);
        CHECK(7 == r[3]);
        CHECK(6 == r[4]);
        CHECK(5 == r[5]);
        CHECK(4 == r[6]);
        CHECK(3 == r[7]);
        CHECK(2 == r[8]);
        CHECK(1 == r[9]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(5, -5, -2)};
        CHECK(5 == r.Count());
        CHECK(5 == r[0]);
        CHECK(3 == r[1]);
        CHECK(1 == r[2]);
        CHECK(-1 == r[3]);
        CHECK(-3 == r[4]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(-5, 5, -1)};
        CHECK(0 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(-1, -10, -2)};
        CHECK(5 == r.Count());
        CHECK(-1 == r[0]);
        CHECK(-3 == r[1]);
        CHECK(-5 == r[2]);
        CHECK(-7 == r[3]);
        CHECK(-9 == r[4]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(4, 0, -1)};
        CHECK(4 == r.Count());
        CHECK(4 == r[0]);
        CHECK(3 == r[1]);
        CHECK(2 == r[2]);
        CHECK(1 == r[3]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(0, -5, -1)};
        CHECK(5 == r.Count());
        CHECK(0 == r[0]);
        CHECK(-1 == r[1]);
        CHECK(-2 == r[2]);
        CHECK(-3 == r[3]);
        CHECK(-4 == r[4]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(10, 0, -15)};
        CHECK(1 == r.Count());
        CHECK(10 == r[0]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(-5, -10, -6)};
        CHECK(1 == r.Count());
        CHECK(-5 == r[0]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(-5, -10, 6)};
        CHECK(0 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(0, 10, 2)};
        CHECK(5 == r.Count());
        CHECK(0 == r[0]);
        CHECK(2 == r[1]);
        CHECK(4 == r[2]);
        CHECK(6 == r[3]);
        CHECK(8 == r[4]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(1, 10, 3)};
        CHECK(3 == r.Count());
        CHECK(1 == r[0]);
        CHECK(4 == r[1]);
        CHECK(7 == r[2]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(5, 6, 1)};
        CHECK(1 == r.Count());
        CHECK(5 == r[0]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(0, 5, 1)};
        CHECK(5 == r.Count());
        CHECK(0 == r[0]);
        CHECK(1 == r[1]);
        CHECK(2 == r[2]);
        CHECK(3 == r[3]);
        CHECK(4 == r[4]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(10, 5, 2)};
        CHECK(0 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(0, 0, 1)};
        CHECK(0 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }

    {
        const auto r{Range(0, 10, 20)};
        CHECK(1 == r.Count());
        CHECK(0 == r[0]);
        CHECK(0 == r[MAX_INT - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INT]);
    }
}
