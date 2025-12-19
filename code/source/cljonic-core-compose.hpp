#ifndef CLJONIC_CORE_COMPOSE_HPP
#define CLJONIC_CORE_COMPOSE_HPP

#include <utility>

namespace cljonic
{

namespace core
{

template <typename F1, typename F2, typename... Args>
concept ComposeIsCallableWith = requires(F1 f1, F2 f2, Args&&... args) {
    { f1(f2(std::forward<Args>(args)...)) };
};

template <typename F1, typename F2>
[[nodiscard]] constexpr auto Compose(F1&& f1, F2&& f2) noexcept
{
    return [f1 = std::forward<F1>(f1), f2 = std::forward<F2>(f2)]<typename... T>(T&&... args)
    {
        static_assert(ComposeIsCallableWith<F1, F2, T...>,
                      "Each Compose argument must be callable with one argument of the return type of the argument to "
                      "its right. Was the Compose result function called with the correct number of arguments?");

        return f1(f2(std::forward<T>(args)...));
    };
}

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
    constexpr auto f1{Compose(StrDouble, Add1p7, Mult2, Div3, AddXY)};
    const auto x1{f1(234, 1111)};

    constexpr auto f2{Compose(
        [](const int x) noexcept
        {
            return x + 1.7;
        },
        [](const std::tuple<const int, const int>& x) noexcept
        {
            auto [num, den]{x};
            return 2 * (num / den);
        },
        [](const int x) noexcept
        {
            return std::make_tuple(x, 3);
        },
        [](const int x, const int y) noexcept
        {
            return x + y;
        })};
    const auto x2{f2(234, 1111)};

    constexpr auto f3{Compose(
        StrDouble,
        [](const int x) noexcept
        {
            return x + 1.7;
        },
        Mult2,
        [](const int x) noexcept
        {
            return std::make_tuple(x, 3);
        },
        AddXY)};
    const auto x3{f3(234, 1111)};

    // Compiler Error:
    //     Each Compose argument must be callable with one argument of the return type of the argument to its right. Was
    //     the Compose result function called with the correct number of arguments?
    // constexpr auto f{Compose(Mult2, Add1p7, Div3, AddXY)};
    // const auto x{f(234, 1111)};

    // Compiler Error:
    //     Each Compose argument must be callable with one argument of the return type of the argument to its right. Was
    //     the Compose result function called with the correct number of arguments?
    // constexpr auto f{Compose(Add1p7, Mult2, Div3, AddXY)};
    // const auto x{f(234)};

    return 0;
}
~~~~~
*/
template <typename F1, typename F2, typename... Fs>
[[nodiscard]] constexpr auto Compose(F1&& f1, F2&& f2, Fs&&... fs) noexcept
{
    return Compose(std::forward<F1>(f1), Compose(std::forward<F2>(f2), std::forward<Fs>(fs)...));
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_COMPOSE_HPP
