#ifndef CLJONIC_CORE_EVERY_HPP
#define CLJONIC_CORE_EVERY_HPP

#include "cljonic-collection-maximum-element-count.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Every
* The \b Every function returns true if the first parameter, which must be a \b unary \b predicte, returns true when
* it is called with every element of the second parameter, which must be a \b cljonic \b collection, else false. Also
* returns \b true if the second parameter is empty.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto Less10 = [](const int i) { return i < 10; };
    constexpr auto LessP = [](const char c) { return c < 'p'; };
    const auto b0{Every(Less10, Array<int, 10>{})}; // true when empty Array
    const auto b1{Every(Less10, Array{10})};        // false
    const auto b2{Every(Less10, Array{1, 2, 3})};   // true
    const auto b3{Every(Less10, Range(0))};         // true when empty Range
    const auto b4{Every(Less10, Range(11))};        // false
    const auto b5{Every(Less10, Range(10))};        // true
    const auto b6{Every(Less10, Repeat(0, 10))};    // true when empty Repeat
    const auto b7{Every(Less10, Repeat(10, 10))};   // false
    const auto b8{Every(Less10, Repeat(4, 1))};     // true
    const auto b9{Every(Less10, Set<int, 5>{})};    // true when empty Set
    const auto b10{Every(Less10, Set{10, 11, 12})}; // false
    const auto b11{Every(Less10, Set{3, 5, 7, 9})}; // true
    const auto b12{Every(LessP, String{""})};       // true when empty String
    const auto b13{Every(LessP, String{"xyz"})};    // false
    const auto b14{Every(LessP, String{"hello"})};  // true

    return 0;
}
~~~~~
*/
template <typename F, typename C>
constexpr auto Every(const F& f, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Every's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<F, typename C::value_type>,
                  "Every's function is not a valid unary predicate for the collection value type");

    auto result{true};
    for (SizeType i{0}; (result and (i < c.Count())); ++i)
        result = f(c[i]);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_EVERY_HPP
