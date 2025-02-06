#ifndef CLJONIC_CORE_TAKENTH_HPP
#define CLJONIC_CORE_TAKENTH_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_TakeNth
* The \b TakeNth function returns a \b cljonic \b Array of every \b Nth element of a \b cljonic \b collection, starting
* with the first element. If the specified \b Nth element is zero then the \b cljonic \b Array returned will be filled
* with the first element of the \b cljonic \b collection. If the \b cljonic \b collection is empty then an empty
* \b cljonic \b Array will be returned.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto t0{TakeNth(0, a)};                                // immutable, full Array, of ten 0s
    constexpr auto t1{TakeNth(1, a)};                                // immutable, full Array, with 0 through 9
    constexpr auto t2{TakeNth(2, a)};                                // immutable, sparse Array, with 0, 2, 4, 6, and 8
    constexpr auto t3{TakeNth(3, a)};                                // immutable, sparse Array, with 0, 3, 6, and 9
    constexpr auto t4{TakeNth(4, a)};                                // immutable, sparse Array, with 0, 4, and 8
    constexpr auto t5{TakeNth(5, a)};                                // immutable, sparse Array, with 0 and 5
    constexpr auto t6{TakeNth(6, a)};                                // immutable, sparse Array, with 0 and 6
    constexpr auto t7{TakeNth(7, a)};                                // immutable, sparse Array, with 0 and 7
    constexpr auto t8{TakeNth(8, a)};                                // immutable, sparse Array, with 0 and 8
    constexpr auto t9{TakeNth(9, a)};                                // immutable, sparse Array, with 0 and 9
    constexpr auto t10{TakeNth(10, a)};                              // immutable, sparse Array, with 0
    constexpr auto t50{TakeNth(50, a)};                              // immutable, sparse Array, with 0
    constexpr auto tEmpty{TakeNth(50, Range<0>{})};                  // immutable, empty Array
    constexpr auto tRpt7{TakeNth(50, Repeat<7, const char*>{"11"})}; // immutable, sparse Array, with one "11"
    constexpr auto tSet5{TakeNth(5, Set{'a', 'b'})};                 // immutable, sparse Array, with 'a'
    constexpr auto tStr3{TakeNth(3, String{"abcde"})};               // immutable, sparse Array, with 'a', and 'd'

    // Compiler Error: TakeNth's second parameter must be a cljonic collection
    // constexpr auto t{TakeNth(10, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto TakeNth(const SizeType nth, const C& c) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "TakeNth's second parameter must be a cljonic collection");

    using ResultType = Array<typename C::value_type, c.MaximumCount()>;
    using ValueType = typename C::value_type;

    constexpr auto FillArray = [](ResultType& r, const ValueType& v) noexcept
    {
        for (SizeType i{0}; i < r.MaximumCount(); ++i)
            r.MAppend(v);
    };
    constexpr auto FillArrayNth = [](ResultType& r, const C& vArray, const SizeType nth) noexcept
    {
        auto i{SizeType{0}};
        while (i < vArray.Count())
        {
            r.MAppend(vArray[i]);
            i += nth;
        }
    };
    auto result{ResultType{}};
    if ((c.Count() > 0) and (0 == nth))
        FillArray(result, c[0]);
    else if (c.Count() > 0)
        FillArrayNth(result, c, nth);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_TAKENTH_HPP
