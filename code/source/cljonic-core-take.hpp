#ifndef CLJONIC_CORE_TAKE_HPP
#define CLJONIC_CORE_TAKE_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_Take
* The \b Take function returns a \b cljonic \b Array with the same \b MaximumCount as its second parameter, which must
* be a \b cljonic \b collection, containing the number of elements given by its first parameter copied from its second
* parameter to its result, in order. If its second parameter contains fewer elements than the first parameter specifies
* the returned \b Array will contain all the elements of its first parameter, in order. If the second parameter contains
* more elements than the first parameter specifies the returned \b Array will contain only the number of elements
* specified by the first parameter copied from the second parameter to the result, in order. If the second parameter is
* empty, the returned \b Array will also be empty.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    const auto a{Array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    const auto t0{Take(0, a)};                                // immutable, empty Array
    const auto t1{Take(1, a)};                                // immutable, sparse Array, with 0
    const auto t5{Take(5, a)};                                // immutable, sparse Array, with 0 to 4
    const auto t50{Take(50, a)};                              // immutable, full Array, with 0 to 0
    const auto tEmpty{Take(50, Range<0>{})};                  // immutable, empty Array
    const auto tRpt7{Take(50, Repeat<7, const char*>{"11"})}; // immutable, full Array, with seven "11"s
    const auto tSet5{Take(5, Set{'a', 'b'})};                 // immutable, sparse Array, with 'a' and 'b'
    const auto tStr3{Take(3, String{"Hello"})};               // immutable, sparse Array, with 'H', 'e', and 'l'

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Take(const std::size_t count, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "The second parameter must be a cljonic collection");

    using CountType = decltype(c.Count());
    auto result{Array<typename C::value_type, c.MaximumCount()>{}};
    auto maxIndex{Min(count, c.Count())};
    for (CountType i{0}; (i < maxIndex); ++i)
        result.MConj(c[i]);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_TAKE_HPP
