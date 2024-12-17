#include <type_traits>
#include "catch.hpp"
#include "cljonic-repeat.hpp"

using namespace cljonic;

static constexpr auto MAX_INDEX{std::numeric_limits<std::size_t>::max()};

SCENARIO("Repeat", "[CljonicRepeat]")
{
    {
        const auto r{Repeat(1)};
        CHECK(MAX_INDEX == r.Count());
        CHECK(1 == r[0]);
        CHECK(1 == r[1]);
        CHECK(1 == r[MAX_INDEX - 1]);
        CHECK(0 == r[-1]);
        CHECK(0 == r[MAX_INDEX]);
    }

    {
        const auto r{Repeat(5, 1)};
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
        using T = std::variant<int, double, char, const char*>;

        const auto r{Repeat(T{'x'})};
        CHECK(MAX_INDEX == r.Count());
        CHECK('x' == std::get<char>(r[0]));
        CHECK('x' == std::get<char>(r[1]));
        CHECK('x' == std::get<char>(r[MAX_INDEX - 1]));
        CHECK(0 == std::get<int>(r[-1]));
        CHECK(0 == std::get<int>(r[MAX_INDEX]));
    }

    {
        using T = std::variant<int, double, char, const char*>;

        const auto r{Repeat(5, T{'x'})};
        CHECK(5 == r.Count());
        CHECK('x' == std::get<char>(r[0]));
        CHECK('x' == std::get<char>(r[1]));
        CHECK('x' == std::get<char>(r[2]));
        CHECK('x' == std::get<char>(r[3]));
        CHECK('x' == std::get<char>(r[4]));
        CHECK(0 == std::get<int>(r[5]));
        CHECK(0 == std::get<int>(r[MAX_INDEX - 1]));
        CHECK(0 == std::get<int>(r[-1]));
        CHECK(0 == std::get<int>(r[MAX_INDEX]));
    }

    {
        const auto r{Repeat(5, 1)};
        for (const auto& element : r)
        {
            CHECK(1 == element);
        }
    }
}
