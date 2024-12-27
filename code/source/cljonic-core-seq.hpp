#ifndef CLJONIC_CORE_SEQ_HPP
#define CLJONIC_CORE_SEQ_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_Seq
* The \b Seq function returns a \b cljonic \b Array with the same \b MaximumCount as its second parameter, which must
* be a \b cljonic \b collection, containing all of the elements from its second parameter, in order.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    const auto s0{Seq(Array<int,0>{})};               // immutable, full cljonic Array, with zero elements
    const auto s1{Seq(Array{1, 2})};                  // immutable, full cljonic Array, with 1 and 2
    const auto s2{Seq(Range<0>{})};                   // immutable, full cljonic Array, with zero elements
    const auto s3{Seq(Repeat<7, const char*>{"11"})}; // immutable, full cljonic Array, with sever "11" elements
    const auto s4{Seq(Set{'a', 'b'})};                // immutable, full cljonic Array, with 'a' and 'b'
    const auto s5{Seq(String{"Hello"})};              // immutable, full cljonic Array, with 'H', 'e', 'l', 'l', 'o'
    const auto s6{Seq(String<10>{"Hi"})};             // immutable, sparse cljonic Array, with 'H' and 'i'

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Seq(const C& c) noexcept
{
    return Take(c.MaximumCount(), c);
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_SEQ_HPP
