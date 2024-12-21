#ifndef CLJONIC_CORE_PARTIAL_HPP
#define CLJONIC_CORE_PARTIAL_HPP

#include <tuple>

namespace cljonic
{

namespace core
{

/** \anchor Core_Partial
* The \b Partial function takes a function, named \b fn, and one or more of the parameters of \b fn, in order, and
* returns a function that takes the rest of the parameters of \b fn.
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
auto Partial(F&& f, Args&&... args)
{
    return [f, args...](auto&&... rest)
    {
        static_assert(std::invocable<F, Args..., decltype(rest)...>,
                      "Partial function cannot be called with the specified arguments");
        return std::apply(f, std::tuple_cat(std::make_tuple(args...), std::forward_as_tuple(rest...)));
    };
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_PARTIAL_HPP
