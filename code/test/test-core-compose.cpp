#include <string>
#include "catch.hpp"
#include "cljonic-core-compose.hpp"

using namespace cljonic;
using namespace cljonic::core;

int AddXY(const int x, const int y) noexcept
{
    return x + y;
}

std::tuple<int, int> Div3(int x) noexcept
{
    return std::make_tuple(x, 3);
}

int Mult2(const std::tuple<int, int>& x) noexcept
{
    auto [num, den]{x};
    return 2 * (num / den);
}

double Add1p7(int x) noexcept
{
    return x + 1.7;
}

constexpr auto BUFFER_SIZE{100};
char buffer[BUFFER_SIZE];

const char* StrDouble(double x) noexcept
{
    snprintf(buffer, BUFFER_SIZE, "%f", x);
    return buffer;
}

SCENARIO("Compose", "[CljonicCoreCompose]")
{
    constexpr auto f1{Compose(StrDouble, Add1p7, Mult2, Div3, AddXY)};
    CHECK(std::string{"897.700000"} == f1(234, 1111));

    constexpr auto f2{Compose(
        StrDouble,
        [](const int x) noexcept { return x + 1.7; },
        Mult2,
        [](const int x) noexcept { return std::make_tuple(x, 3); },
        AddXY)};
    CHECK(std::string{"897.700000"} == f2(234, 1111));

    constexpr auto f3{Compose(
        [](const std::tuple<const int, const int>& x) noexcept
        {
            auto [num, den]{x};
            return 2 * (num / den);
        },
        [](const int x) noexcept { return std::make_tuple(x, 3); },
        [](const int x, const int y) noexcept { return x + y; })};
    CHECK(896 == f3(234, 1111));
}
