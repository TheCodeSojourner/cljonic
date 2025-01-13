#ifndef CLJONIC_CORE_ISFULL_HPP
#define CLJONIC_CORE_ISFULL_HPP

#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_IsEmpty
* The \b IsEmpty function returns true if its parameter, which must be a a \b cljonic \b collection, is empty (i.e., its
* \b Count is zero).
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto b0{IsEmpty(Array<int, 0>{})};              // true
    constexpr auto b1{IsEmpty(Array{1, 2})};                  // false
    constexpr auto b2{IsEmpty(Range<0>{})};                   // true
    constexpr auto b3{IsEmpty(Repeat<7, const char*>{"11"})}; // false
    constexpr auto b4{IsEmpty(Set{'a', 'b'})};                // false
    constexpr auto b5{IsEmpty(String{"Hello"})};              // false
    constexpr auto b6{IsEmpty(String<10>{"Hi"})};             // false

    // Compiler Error: IsEmpty's parameter must be a cljonic collection
    // constexpr auto s{IsEmpty("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto IsEmpty(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "IsEmpty's parameter must be a cljonic collection");

    return 0 == c.Count();
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_ISFULL_HPP
