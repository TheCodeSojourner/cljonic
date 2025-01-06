#ifndef CLJONIC_CORE_SOME_HPP
#define CLJONIC_CORE_SOME_HPP

#include "cljonic-concepts.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Some
* The \b Some function calls its first parameter, which must be a \b unary \b predicate, with the elements of its second
* parameter, which must be a \b cljonic \b collection, and if any of the results are \b true then \b Some returns true,
* otherwise \b Some returns false. Note that \b Some does short circuit, that is \b true is returned on the first
* \b true result from \b Some's first parameter and no.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto Is13 = [](const int a) { return 13 == a; };

    constexpr auto a{Array{11, 12, 13, 14}};
    const auto someA{Some(Is13, a)};

    constexpr auto b{Array{11, 12, 14}};
    const auto someB{Some(Is13, b)};

    const auto someRng{Some(Is13, Range<0>{})};

    const auto someRpt{Some(Is13, Repeat<4, int>{11})};

    const auto someSet0{Some(Is13, Set{11, 12, 13, 14})};
    const auto someSet1{Some(Is13, Set{11, 12, 14})};

    const auto someStr0{Some([](const char i) { return 'y' == i; }, String{"abcd"})};
    const auto someStr1{Some([](const char i) { return 'y' == i; }, String{"axbycz"})};

    // Compiler Error: Some's second parameter must be a cljonic collection
    // const auto sb{Some(Is13, "Hello")};

    // Compiler Error: Some's function is not a valid unary predicate for the collection value type
    // const auto sb{Some(Is13, Array<const char*, 5>{})};

    return 0;
}
~~~~~
*/
template <typename F, typename C>
constexpr auto Some(F&& f, const C& c) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "Some's second parameter must be a cljonic collection");

    static_assert(IsUnaryPredicate<std::decay_t<F>, typename C::value_type>,
                  "Some's function is not a valid unary predicate for the collection value type");

    for (const auto& i : c)
        if (f(i))
            return true;
    return false;
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_SOME_HPP
