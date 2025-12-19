#ifndef CLJONIC_CORE_REVERSE_HPP
#define CLJONIC_CORE_REVERSE_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-core-take.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Reverse
* The \b Reverse function returns a \b cljonic \b Array with the same \b MaximumCount as its parameter, which must
* be a \b cljonic \b collection, containing all of the elements from its parameter, in reverse order.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    const auto r0{Reverse(Array<int, 0>{})};              // empty Array
    const auto r1{Reverse(Array{1, 2})};                  // Array, with 2 and 1
    const auto r2{Reverse(Range<0>{})};                   // empty Array
    const auto r3{Reverse(Repeat<7, const char*>{"11"})}; // Array, with seven "11" elements
    const auto r4{Reverse(Set{'a', 'b'})};                // Array, with 'b' and 'a'
    const auto r5{Reverse(String{"Hello"})};              // Array, with 'o', 'l', 'l', 'e', 'H'
    const auto r6{Reverse(String<10>{"Hi"})};             // Array, with 'i' and 'H'

    // Compiler Error: Reverse's parameter must be a cljonic collection
    // const auto s{Reverse("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
[[nodiscard]] constexpr auto Reverse(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Reverse's parameter must be a cljonic collection");

    auto result{Array<typename C::value_type, C::MaximumCount()>{}};
    for (SizeType i{c.Count()}; i != 0; --i)
        MConj(result, c[i - 1]);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_REVERSE_HPP
