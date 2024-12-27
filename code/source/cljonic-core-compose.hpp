#ifndef CLJONIC_CORE_COMPOSE_HPP
#define CLJONIC_CORE_COMPOSE_HPP

#include <concepts>
#include <utility>

namespace cljonic
{

namespace core
{

/** \anchor Core_Compose
* The \b Compose function is a so-called \b Higher-Order \b Function that takes a list of two or more functions and
* returns a function that is the composition of the list of functions.  The function returned takes the same number of
* arguments as the rightmost function in the list. The result of the rightmost function is passed to the next function
* to the left, and so on, until the result of the leftmost function is returned.
~~~~~{.cpp}
#include <cstdio>
#include <tuple>
#include "cljonic.hpp"

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

int main()
{
    // Compiler Error: Compose requires at least two function parameters
    // constexpr auto f{Compose(StrDouble)};

    // Compiler Error: Each Compose function must be callable with the return type of the next function
    // constexpr auto f{Compose(StrDouble, Add1p7, Div3, AddXY)};

    constexpr auto f{Compose(StrDouble, Add1p7, Mult2, Div3, AddXY)};

    const auto x{f(234, 1111)};

    // Compiler Error: too few arguments to function
    // const auto x{f(234)};

    return 0;
}
~~~~~
*/
template <typename F, typename... Fs>
static constexpr auto InnerCompose(F&& f, Fs&&... fs) noexcept
{
    if constexpr (sizeof...(fs) == 0)
    {
        return std::forward<F>(f);
    }
    else
    {
        return [=]<typename... T>(T&&... args)
        { return f(InnerCompose(std::forward<Fs>(fs)...)(std::forward<T>(args)...)); };
    }
}

template <typename... Fs>
constexpr auto Compose(Fs&&... fs) noexcept
{
    static_assert(sizeof...(Fs) >= 2, "Compose requires at least two function parameters");

    static_assert(EachFunctionIsInvocableWithNextReturnType<Fs...>,
                  "Each Compose function must be callable with the return type of the next function");

    return InnerCompose(std::forward<Fs>(fs)...);
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_COMPOSE_HPP
