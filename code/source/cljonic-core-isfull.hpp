#ifndef CLJONIC_CORE_ISFULL_HPP
#define CLJONIC_CORE_ISFULL_HPP

#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_IsFull
* The \b IsFull function returns true if its parameter, which must be a a \b cljonic \b collection, is full (i.e., its
* \b Count is the same as its \b MaximumCount).
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    const auto b0{IsFull(Array<int,0>{})};               // true
    const auto b1{IsFull(Array{1, 2})};                  // true
    const auto b2{IsFull(Range<0>{})};                   // true
    const auto b3{IsFull(Repeat<7, const char*>{"11"})}; // true
    const auto b4{IsFull(Set{'a', 'b'})};                // true
    const auto b5{IsFull(String{"Hello"})};              // true
    const auto b6{IsFull(String<10>{"Hi"})};             // false

    // Compiler Error: IsFull's parameter must be a cljonic collection
    // const auto s{IsFull("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto IsFull(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "IsFull's parameter must be a cljonic collection");

    return c.Count() == c.MaximumCount();
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_ISFULL_HPP
