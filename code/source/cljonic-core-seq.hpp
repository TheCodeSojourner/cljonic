#ifndef CLJONIC_CORE_SEQ_HPP
#define CLJONIC_CORE_SEQ_HPP

#include "cljonic-concepts.hpp"
#include "cljonic-core-take.hpp"

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
    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    constexpr auto s0{Seq(Array<int,0>{})};               // immutable, full cljonic Array, with zero elements
    constexpr auto s1{Seq(Array{1, 2})};                  // immutable, full cljonic Array, with 1 and 2
    constexpr auto s2{itr};                               // immutable, full cljonic Array, with 1, 2, 3, 4, 5, ...
    constexpr auto s3{Seq(Range<0>{})};                   // immutable, full cljonic Array, with zero elements
    constexpr auto s4{Seq(Repeat<7, const char*>{"11"})}; // immutable, full cljonic Array, with seven "11" elements
    constexpr auto s5{Seq(Set{'a', 'b'})};                // immutable, full cljonic Array, with 'a' and 'b'
    constexpr auto s6{Seq(String{"Hello"})};              // immutable, full cljonic Array, with 'H', 'e', 'l', 'l', 'o'
    constexpr auto s7{Seq(String<10>{"Hi"})};             // immutable, sparse cljonic Array, with 'H' and 'i'

    // Compiler Error: Seq's parameter must be a cljonic collection
    // const auto s{Seq("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Seq(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Seq's parameter must be a cljonic collection");

    using ResultType = typename C::value_type;
    auto result{Array<ResultType, C::MaximumCount()>{}};
    for (const ResultType& v : c)
        MAppend(result, v);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_SEQ_HPP
