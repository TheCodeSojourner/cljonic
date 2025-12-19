#ifndef CLJONIC_CORE_EVERY_HPP
#define CLJONIC_CORE_EVERY_HPP

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
    constexpr auto b0{Every(Less10, Array<int, 10>{})};    // true when empty Array
    constexpr auto b1{Every(Less10, Array{10})};           // false
    constexpr auto b2{Every(Less10, Array{1, 2, 3})};      // true
    constexpr auto b3{Every(Less10, Range<0>{})};          // true when empty Range
    constexpr auto b4{Every(Less10, Range<11>{})};         // false
    constexpr auto b5{Every(Less10, Range<10>{})};         // true
    constexpr auto b6{Every(Less10, Repeat<0, int>{10})};  // true when empty Repeat
    constexpr auto b7{Every(Less10, Repeat<10, int>{10})}; // false
    constexpr auto b8{Every(Less10, Repeat<4, int>{1})};   // true
    constexpr auto b9{Every(Less10, Set<int, 5>{})};       // true when empty Set
    constexpr auto b10{Every(Less10, Set{10, 11, 12})};    // false
    constexpr auto b11{Every(Less10, Set{3, 5, 7, 9})};    // true
    constexpr auto b12{Every(LessP, String{""})};          // true when empty String
    constexpr auto b13{Every(LessP, String{"xyz"})};       // false
    constexpr auto b14{Every(LessP, String{"hello"})};     // true

    // Compiler Error: Every's second parameter must be a cljonic collection
    // constexpr auto b{Every(Less10, "Hello")};

    // Compiler Error: Every's function is not a valid unary predicate for the collection value type
    // constexpr auto b{Every([](const char* s) { return true; }, Array{1, 2, 3, 4})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
[[nodiscard]] constexpr auto Every(F&& f, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Every's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<std::decay_t<F>, typename C::value_type>,
                  "Every's function is not a valid unary predicate for the collection value type");

    auto result{true};
    for (SizeType i{0}; (result and (i < c.Count())); ++i)
        result = f(c[i]);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_EVERY_HPP
