#ifndef CLJONIC_CORE_PARTIAL_HPP
#define CLJONIC_CORE_PARTIAL_HPP

#include <utility>

namespace cljonic
{

namespace core
{

/** \anchor Core_Partial
* The \b Partial function is a so-called \b Higher-Order \b Function that takes a function as its first parameter, and
* one or more valid parameters of that function, in order, and returns a function that takes the rest of the allowed
* parameters of the first parameter function.
*
* This is useful for creating a set of functions based upon a base function, and a set of initial values to the base
* function.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

auto multiply2 = [](int x, int y) { return x * y; };
auto multiply3 = [](int x, int y, int z) { return x * y * z; };
auto multiply2_10 = Partial(multiply2, 10);
auto multiply3_10 = Partial(multiply3, 10);
auto multiply3_10_11 = Partial(multiply3, 10, 11);
auto multiply3_10_11_12 = Partial(multiply3, 10, 11, 12);

int main()
{
    // const auto v{multiply2_10(1, 2)}; // Error: Partial function cannot be called with the specified arguments
    const auto v0{multiply2_10(2)};
    const auto v1{multiply3_10(2, 4)};
    const auto v2{multiply3_10_11(2)};
    const auto v3{multiply3_10_11_12()};

    return 0;
}
~~~~~
*/
template <typename F, typename... Args>
constexpr auto Partial(F&& f, Args&&... args) noexcept
{
    return [f = std::forward<F>(f), ... args = std::forward<Args>(args)](auto&&... rest)
    {
        static_assert(std::regular_invocable<F, Args..., decltype(rest)...>,
                      "Partial function cannot be called with the specified arguments");

        return f(args..., std::forward<decltype(rest)>(rest)...);
    };
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_PARTIAL_HPP
