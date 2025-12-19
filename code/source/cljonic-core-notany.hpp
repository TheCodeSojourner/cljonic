#ifndef CLJONIC_CORE_NOTANY_HPP
#define CLJONIC_CORE_NOTANY_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-core-some.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_NotAny
* The \b NotAny function returns \b false if the first parameter, which must be a \b unary \b predicte, returns \b true
* when it is called with any element of the second parameter, which must be a \b cljonic \b collection, else false. Also
* returns \b true if the second parameter is empty.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto Less10 = [](const int i) { return i < 10; };
    constexpr auto LessP = [](const char c) { return c < 'p'; };
    constexpr auto b0{NotAny(Less10, Array<int, 10>{})};    // true when empty Array
    constexpr auto b1{NotAny(Less10, Array{10})};           // true
    constexpr auto b2{NotAny(Less10, Array{1, 2, 3})};      // false
    constexpr auto b3{NotAny(Less10, Range<0>{})};          // true when empty Range
    constexpr auto b4{NotAny(Less10, Range<11>{})};         // false
    constexpr auto b5{NotAny(Less10, Range<10>{})};         // false
    constexpr auto b6{NotAny(Less10, Repeat<0, int>{10})};  // true when empty Repeat
    constexpr auto b7{NotAny(Less10, Repeat<10, int>{10})}; // true
    constexpr auto b8{NotAny(Less10, Repeat<4, int>{1})};   // false
    constexpr auto b9{NotAny(Less10, Set<int, 5>{})};       // true when empty Set
    constexpr auto b10{NotAny(Less10, Set{10, 11, 12})};    // true
    constexpr auto b11{NotAny(Less10, Set{3, 5, 7, 9})};    // false
    constexpr auto b12{NotAny(LessP, String{""})};          // true when empty String
    constexpr auto b13{NotAny(LessP, String{"xyz"})};       // true
    constexpr auto b14{NotAny(LessP, String{"hello"})};     // false

    // Compiler Error: NotAny's second parameter must be a cljonic collection
    // constexpr auto b{NotAny(Less10, "Hello")};

    // Compiler Error: NotAny's function is not a valid unary predicate for the collection value type
    // constexpr auto b{NotAny([](const char* s) { return true; }, Array{1, 2, 3, 4})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
[[nodiscard]] constexpr auto NotAny(F&& f, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "NotAny's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<std::decay_t<F>, typename C::value_type>,
                  "NotAny's function is not a valid unary predicate for the collection value type");

    return (c.Count() == 0) or (not Some(std::forward<F>(f), c));
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_NOTANY_HPP
