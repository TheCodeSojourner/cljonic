#ifndef CLJONIC_CORE_ITERATE_HPP
#define CLJONIC_CORE_ITERATE_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-lazyiterate.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Iterate
 * The \b Iterate function returns an immutable collection implemented as a \b lazy \b sequence.  The first element in
 * the sequence is the second parameter, the second element in the sequence is the result of calling the first
 * parameter, which must be a unary function of the type of the second parameter that returns the same type as the type
 * of the second parameter, with the second parameter, the third element in the sequence is the result of calling the
 * first parameter with the second element in the sequence, etc. If we name the first parameter \b "f" and the second
 * parameter \b "x" then the sequence is: \b x, \b f(x), \b f(f(x)), \b f(f(f(x))), etc.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int TimesTen(const int x) noexcept
{
    return x * 10;
}

int main()
{
    auto i0{Iterate([](const double d) { return 1.1 * d; }, 11.1)};
    auto i1{Iterate(TimesTen, 1)};

    // Compiler Error: Iterate's first parameter is not a unary function of its second parameter
    // const auto i{Iterate([](const double d) { return 1.1 * d; }, "Hello")};

    return 0;
}
~~~~~
*/
template <typename F, typename T>
constexpr auto Iterate(F&& f, const T& t) noexcept
{
    static_assert(IsUnaryFunction<F, T>, "Iterate's first parameter is not a unary function of its second parameter");

    return LazyIterate{std::forward<F>(f), t};
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_ITERATE_HPP
