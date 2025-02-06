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
    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    constexpr auto r0{Reverse(Array<int, 0>{})};              // empty Array
    constexpr auto r1{Reverse(Array{1, 2})};                  // Array, with 2 and 1
    constexpr auto r2{Reverse(Range<0>{})};                   // empty Array
    const auto r3{Reverse(itr)};                          // Array with CljonicCollectionMaximumElementCount to 1
    constexpr auto r4{Reverse(Repeat<7, const char*>{"11"})}; // Array, with seven "11" elements
    constexpr auto r5{Reverse(Set{'a', 'b'})};                // Array, with 'b' and 'a'
    constexpr auto r6{Reverse(String{"Hello"})};              // Array, with 'o', 'l', 'l', 'e', 'H'
    constexpr auto r7{Reverse(String<10>{"Hi"})};             // Array, with 'i' and 'H'

    // Compiler Error: Reverse's parameter must be a cljonic collection
    // const auto s{Reverse("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Reverse(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Reverse's parameter must be a cljonic collection");

    auto result{Array<typename C::value_type, C::MaximumCount()>{}};
    for (const auto& v : c)
        result.MAppend(v);
    result.MReverse();
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_REVERSE_HPP
