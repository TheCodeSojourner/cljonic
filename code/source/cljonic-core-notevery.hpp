#ifndef CLJONIC_CORE_NOTEVERY_HPP
#define CLJONIC_CORE_NOTEVERY_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-core-every.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_NotEvery
* The \b NotEvery function returns \b false if the first parameter, which must be a \b unary \b predicte, returns
* \b true when it is called with every element of the second parameter, which must be a \b cljonic \b collection, else
* \b true. Also returns \b false if the second parameter is empty.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto Less10 = [](const int i) { return i < 10; };
    constexpr auto LessP = [](const char c) { return c < 'p'; };
    constexpr auto b0{NotEvery(Less10, Array<int, 10>{})};    // false when empty Array
    constexpr auto b1{NotEvery(Less10, Array{10})};           // true
    constexpr auto b2{NotEvery(Less10, Array{1, 2, 3})};      // false
    constexpr auto b3{NotEvery(Less10, Range<0>{})};          // false when empty Range
    constexpr auto b4{NotEvery(Less10, Range<11>{})};         // true
    constexpr auto b5{NotEvery(Less10, Range<10>{})};         // false
    constexpr auto b6{NotEvery(Less10, Repeat<0, int>{10})};  // false when empty Repeat
    constexpr auto b7{NotEvery(Less10, Repeat<10, int>{10})}; // true
    constexpr auto b8{NotEvery(Less10, Repeat<4, int>{1})};   // false
    constexpr auto b9{NotEvery(Less10, Set<int, 5>{})};       // false when empty Set
    constexpr auto b10{NotEvery(Less10, Set{10, 11, 12})};    // true
    constexpr auto b11{NotEvery(Less10, Set{3, 5, 7, 9})};    // false
    constexpr auto b12{NotEvery(LessP, String{""})};          // false when empty String
    constexpr auto b13{NotEvery(LessP, String{"xyz"})};       // true
    constexpr auto b14{NotEvery(LessP, String{"hello"})};     // false

    // Compiler Error: NotEvery's second parameter must be a cljonic collection
    // constexpr auto b{NotEvery(Less10, "Hello")};

    // Compiler Error: NotEvery's function is not a valid unary predicate for the collection value type
    // constexpr auto b{NotEvery([](const char* s) { return true; }, Array{1, 2, 3, 4})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
[[nodiscard]] constexpr auto NotEvery(F&& f, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "NotEvery's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<std::decay_t<F>, typename C::value_type>,
                  "NotEvery's function is not a valid unary predicate for the collection value type");

    return (c.Count() != 0) and (not Every(std::forward<F>(f), c));
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_NOTEVERY_HPP
