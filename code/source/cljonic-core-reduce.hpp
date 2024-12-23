#ifndef CLJONIC_CORE_REDUCE_HPP
#define CLJONIC_CORE_REDUCE_HPP

#include <tuple>

namespace cljonic
{

namespace core
{

/** \anchor Core_Reduce
* Two overloads of \b Reduce are implemented.
*
* The first overload of \b Reduce starts by calling \b f on the first two elements of \b c, then calls \b f with the
* result of the first call and the third element of \b c, then calls \b f with the result of the second call and the
* fourth element of \b c, and continues this process until all elements of \b c have been used in a call to \b f.
* If \b c has only one element it is returned, and \b f is never called. If \b c has no elements the \b default
* \b element of \b c is returned, and \b f is never called.
*
* The second overload of \b Reduce starts by calling \b f with an initial value and the first element of \b c, then
* calls \b f with the result of the first call and the second element of \b c, then calls \b f with the result of the
* second call and the third element of \b c, and continues this process until all elements of \b c have been used in a
* call to \b f. If \b c has no elements the initial value is returned, and \b f is never called.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    auto Add = [](const auto& a, const auto& b) { return a + b; };

    const auto aAddEmpty{Array<int, 5>{}};
    const auto nAddEmpty{Reduce(add, aAddEmpty)};
    const auto nAddEmptyI{Reduce(add, 11, aAddEmpty)};

    const auto aAddOne{Array{11}};
    const auto nAdd{Reduce(add, aAddOne)};
    const auto nAddI{Reduce(add, 11, aAddOne)};

    const auto a{Array{1, 2, 3, 4}};
    const auto n0{Reduce(Add, a)};
    const auto n1{Reduce(Add, 11, a)};

    const auto rng{Range(5)};
    const auto n2{Reduce(Add, rng)};
    const auto n3{Reduce(Add, 11, rng)};

    const auto rpt{Repeat(5, 5)};
    const auto n4{Reduce(Add, rpt)};
    const auto n5{Reduce(Add, 11, rpt)};

    const auto s{String{"Hello"}};
    const auto n6{Reduce([](const int count, const char b) { return count + 1; }, 0, s)};

    // Compiler Error: Function cannot be called with two parameters of the collection value type
    // const auto ints{Array{1, 2, 3, 4}};
    // CHECK(10 == Reduce([](const char* a, const int b) { return a[0] + b; }, a));

    // Compiler Error: Function cannot be called with parameters of function result type, and collection value type
    // const auto strs{Array{1, 2, 3, 4}};
    // CHECK(10 == Reduce([]([[maybe_unused]] const int a, [[maybe_unused]] const int b) { return "Hello"; }, a));

    // Compiler Error: Function cannot be called with parameters of initial value type, and collection value type
    // const auto strs{Array{1, 2, 3, 4}};
    // CHECK(10 == Reduce([](const int a, const int b) { return a + b; }, "Hello", a));

    // Compiler Error: Function cannot be called with parameters of function result type, and collection value type
    // const auto strs{Array{1, 2, 3, 4}};
    // CHECK(10 == Reduce([]([[maybe_unused]] const int a, [[maybe_unused]] const int b) { return "Hello"; }, 11, a));

    return 0;
}
~~~~~
*/
template <typename F, typename C>
auto Reduce(F&& f, const C& c)
{
    static_assert(std::regular_invocable<F, typename C::value_type, typename C::value_type>,
                  "Function cannot be called with two parameters of the collection value type");
    static_assert(std::regular_invocable<F,
                                         std::invoke_result_t<F, typename C::value_type, typename C::value_type>,
                                         typename C::value_type>,
                  "Function cannot be called with parameters of function result type, and collection value type");
    return (0 == c.Count()) ? c.DefaultElement()
                            : std::accumulate((c.begin() + 1), c.end(), *c.begin(), std::forward<F>(f));
}

template <typename F, typename T, typename C>
auto Reduce(F&& f, const T& t, const C& c)
{
    static_assert(std::regular_invocable<F, T, typename C::value_type>,
                  "Function cannot be called with parameters of initial value type, and collection value type");
    static_assert(std::regular_invocable<F, std::invoke_result_t<F, T, typename C::value_type>, typename C::value_type>,
                  "Function cannot be called with parameters of function result type, and collection value type");
    return (0 == c.Count()) ? t : std::accumulate(c.begin(), c.end(), t, std::forward<F>(f));
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_REDUCE_HPP
