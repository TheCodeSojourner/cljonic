#ifndef CLJONIC_CORE_REMOVE_HPP
#define CLJONIC_CORE_REMOVE_HPP

#include "cljonic-array.hpp"
#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Remove
* The \b Remove function calls its first parameter, which must be a \b unary \b predicate, with each element of its
* second parameter, which must be a \b cljonic \b collection, and if the result is \b false a copy of the element is
* concatenated to the end of \b Remove's result, which is a \b cljonic \b Array with the same \b MaximumCount as the
* second parameter.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto Even = [](const int i) { return (0 == (i % 2)); };

    constexpr auto a{Array<int, 10>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
    constexpr auto fA{Remove(Even, a)}; // immutable, sparse Array, with 1, 3, 5, 7, and 9

    constexpr auto rng{Range<10>{}};
    constexpr auto fRng{Remove(Even, rng)}; // immutable, sparse Array, with 1, 3, 5, 7, and 9

    constexpr auto rpt{Repeat<10, int>{1}};
    constexpr auto fRpt{Remove(Even, rpt)}; // immutable, full Array of ten ones

    constexpr auto s{Set<int, 4>{1, 2, 3, 4, 5, 6}};
    constexpr auto fS{Remove(Even, s)}; // immutable, sparse Array, with 1 and 3

    constexpr auto fStr{Remove([](const char c) { return ('l' == c); }, // immutable, sparse Array, with 'H', 'e', 'o'
                               String{"Hello"})};

    // Compiler Error: Remove's second parameter must be a cljonic collection
    // constexpr auto tw{Remove(Even, "Hello")};

    // Compiler Error: Remove's function is not a valid unary predicate for the collection value type
    // constexpr auto tw{Remove(Even, Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
constexpr auto Remove(F&& f, const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Remove's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<std::decay_t<F>, typename C::value_type>,
                  "Remove's function is not a valid unary predicate for the collection value type");

    auto result{Array<typename C::value_type, c.MaximumCount()>{}};
    for (const auto& element : c)
        if (not f(element))
            MConj(result, element);
    return result;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_REMOVE_HPP
