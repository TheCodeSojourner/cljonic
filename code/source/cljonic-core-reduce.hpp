#ifndef CLJONIC_CORE_REDUCE_HPP
#define CLJONIC_CORE_REDUCE_HPP

#include <tuple>
#include <numeric>
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Reduce
* There are two overloads of \b Reduce.
*
* The first overload of \b Reduce starts by calling its first parameter, which must be a function, with the first two
* elements of its second parameter, which must be a \b cljonic \b collection, then calls its first parameter with the
* result of the first call and the third element of its second parameter, then calls its first parameter with the result
* of the second call and the fourth element of its second parameter, and continues this process until all elements of
* its second parameter have been used in a call to its first parameter. If the second parameter has only one element it
* is returned, and its first parameter is never called. If its second parameter has no elements the \b default
* \b element of its second parameter is returned, and its first parameter is never called.
*
* The second overload of \b Reduce starts by calling its first parameter, which must be a function, with its second
* parameter, which is an initial value, and the first element of its thrid parameter, which must be a \b cljonic
* \b collection, then calls its first parameter with the result of the first call and the second element of its third
* parameter, then calls its first parameter with the result of the second call and the third element of its third
* parameter, and continues this process until all elements of its third parameter have been used in a call to its first
* parameter. If the third parameter has no elements the second parameter is returned, and the first parameter is never
* called.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    auto Add = [](const auto& a, const auto& b) { return a + b; };

    const auto aAddEmpty{Array<int, 5>{}};             // an empty Array
    const auto nAddEmpty{Reduce(Add, aAddEmpty)};      // default element value (i.e, 0) is returned, Add not called
    const auto nAddEmptyI{Reduce(Add, 11, aAddEmpty)}; // initial value (i.e., 11) is returned, Add not called

    const auto aAddOne{Array{11}};              // a single-element Array
    const auto nAdd{Reduce(Add, aAddOne)};      // single-element value is returned, Ass not called

    const auto a{Array{1, 2, 3, 4}};
    const auto n0{Reduce(Add, a)};
    const auto n1{Reduce(Add, 11, a)};

    const auto rng{Range<5>{}};
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

    // Compiler Error: Second parameter must be a cljonic collection
    // CHECK(5 == Reduce([](const int a, const char b) { return a + b; }, 5));

    // Compiler Error: Third parameter must be a cljonic collection
    // CHECK(5 == Reduce([](const int count, [[maybe_unused]] const char b) { return count + 1; }, 0, 5));

    return 0;
}
~~~~~
*/
template <typename F, typename C>
auto Reduce(F&& f, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Second parameter must be a cljonic collection");

    static_assert(std::regular_invocable<F, typename C::value_type, typename C::value_type>,
                  "Function cannot be called with two parameters of the collection value type");

    using ResultType = std::invoke_result_t<F, typename C::value_type, typename C::value_type>;
    static_assert(std::regular_invocable<F, ResultType, typename C::value_type>,
                  "Function cannot be called with parameters of function result type, and collection value type");

    return (0 == c.Count()) ? c.DefaultElement()
                            : std::accumulate((c.begin() + 1), c.end(), *c.begin(), std::forward<F>(f));
}

template <typename F, typename T, typename C>
auto Reduce(F&& f, const T& t, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Third parameter must be a cljonic collection");

    static_assert(std::regular_invocable<F, T, typename C::value_type>,
                  "Function cannot be called with parameters of initial value type, and collection value type");

    using ResultType = std::invoke_result_t<F, T, typename C::value_type>;
    static_assert(std::regular_invocable<F, ResultType, typename C::value_type>,
                  "Function cannot be called with parameters of function result type, and collection value type");

    return (0 == c.Count()) ? t : std::accumulate(c.begin(), c.end(), t, std::forward<F>(f));
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_REDUCE_HPP
