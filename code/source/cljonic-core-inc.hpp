#ifndef CLJONIC_CORE_INC_HPP
#define CLJONIC_CORE_INC_HPP

#include <limits>
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Inc
* The \b Inc function returns a number whose value is one greater than its argument's value. If its argument's value is
* already the maximum possible value for the type of its argument then \b Inc will return the value of its argument
* unchanged.
~~~~~{.cpp}
#include <limits>
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto i0{Inc(-10)};                                // -9
    constexpr auto i1{Inc(10)};                                 // 11
    constexpr auto i2{Inc(-11.1)};                              // -10.1
    constexpr auto i3{Inc(11.1)};                               // 12.1
    constexpr auto i4{Inc(std::numeric_limits<int>::max())};    // std::numeric_limits<int>::max()
    constexpr auto i5{Inc(std::numeric_limits<double>::max())}; // std::numeric_limits<double>::max()

    // Compiler Error: Inc's parameter must be an arithmetic type
    // constexpr auto s{Inc("Hello")};

    return 0;
}
~~~~~
*/
template <typename T>
[[nodiscard]] constexpr auto Inc(const T t) noexcept
{
    static_assert(IsArithmetic<T>, "Inc's parameter must be an arithmetic type");

    return (std::numeric_limits<T>::max() == t) ? t : (t + 1);
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_INC_HPP
