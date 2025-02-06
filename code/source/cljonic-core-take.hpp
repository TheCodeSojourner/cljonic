#ifndef CLJONIC_CORE_TAKE_HPP
#define CLJONIC_CORE_TAKE_HPP

#include "cljonic-array.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Take
* There are \b two \b forms of the \b Take function.
*
* The \b first \b form returns a \b cljonic \b Array with the same \b MaximumCount as its second parameter, which must
* be a \b cljonic \b collection, containing the number of elements given by its first parameter copied from its second
* parameter to its result, in order. If its second parameter contains fewer elements than the first parameter specifies
* the returned \b cljonic \b Array will contain all the elements of its first parameter, in order. If the second
* parameter contains more elements than the first parameter specifies the returned \b cljonic \b Array will contain only
* the number of elements specified by the first parameter copied from the second parameter to the result, in order. If
* the second parameter is empty, the returned \b cljonic \b Array will also be empty.
*
* The \b second \b form returns a \b cljonic \b Array with the \b MaximumCount specified by its \b template
* \b parameter. The elements in the returned \b cljonic \b Array are copied from its parameter, which must be a
* \b cljonic \b collection, in order. If its parameter contains fewer elements than its template parameter specifies
* the returned \b cljonic \b Array will contain all the elements of its parameter, in order. If the parameter contains
* more elements than the template parameter specifies the returned \b cljonic \b Array will contain only the number of
* elements specified by the template parameter copied from the parameter to the result, in order. If the parameter is
* empty, the returned \b cljonic \b Array will also be empty.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto itr{Iterate([](const int i) { return i + 1; }, 1)};
    constexpr auto t0{Take(0, a)};                                // immutable, empty Array
    constexpr auto t1{Take(1, a)};                                // immutable, sparse Array, with 0
    constexpr auto t5{Take(5, a)};                                // immutable, sparse Array, with 0 to 4
    constexpr auto t50{Take(50, a)};                              // immutable, full Array, with 0 to 9
    const auto tItr5{Take(5, itr)};                               // immutable, sparse Array, with 1 to 5
    constexpr auto tEmpty{Take(50, Range<0>{})};                  // immutable, empty Array
    constexpr auto tRpt7{Take(50, Repeat<7, const char*>{"11"})}; // immutable, full Array, with seven "11"s
    constexpr auto tSet5{Take(5, Set{'a', 'b'})};                 // immutable, sparse Array, with 'a' and 'b'
    constexpr auto tStr3{Take(3, String{"Hello"})};               // immutable, sparse Array, with 'H', 'e', and 'l'

    // Compiler Error: Take's second parameter must be a cljonic collection
    // constexpr auto t{Take(10, "Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Take(const SizeType count, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Take's second parameter must be a cljonic collection");

    using ResultType = typename C::value_type;
    auto result{Array<ResultType, c.MaximumCount()>{}};
    auto maxIndex{MinArgument(count, c.Count())};
    auto cBegin{c.begin()};
    auto cEnd{cBegin + maxIndex};
    for (auto it{cBegin}; it != cEnd; ++it)
        result.MAppend(static_cast<ResultType>(*it));
    return result;
}

template <SizeType N, typename C>
constexpr auto Take(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Take's parameter must be a cljonic collection");

    using ResultType = typename C::value_type;
    auto result{Array<ResultType, N>{}};
    auto maxIndex{MinArgument(N, c.Count())};
    auto cBegin{c.begin()};
    auto cEnd{cBegin + maxIndex};
    for (auto it{cBegin}; it != cEnd; ++it)
        result.MAppend(static_cast<ResultType>(*it));
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_TAKE_HPP
