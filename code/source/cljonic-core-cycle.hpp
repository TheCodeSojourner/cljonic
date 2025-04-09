#ifndef CLJONIC_CORE_CYCLE_HPP
#define CLJONIC_CORE_CYCLE_HPP

#include <utility>
#include "cljonic-collection-type.hpp"
#include "cljonic-concepts.hpp"
#include "cljonic-iterator.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Cycle
* The \b Cycle function returns an essentially infinite \b lazy \b sequence of the elements contained in a \b cljonic
* \b collection repeated over and over again. Many \ref Namespace_Core "Core" functions accept the result of \b Cycle
* as arguments.
~~~~~{.cpp}
#include "cljonic.hpp"

using namespace cljonic;
using namespace cljonic::core;

int main()
{
    constexpr auto a{Array{11, 12, 13, 14}};
    const auto cycleA0{Cycle(a)};
    const auto cycleA1{Cycle(Array{11, 12, 13, 14})};

    const auto cycleItr{Cycle(Iterate([](const int i) { return i + 1; }, 0))};

    constexpr auto rng{Range<1, 5>{}};
    const auto cycleRng0{Cycle(rng)};
    const auto cycleRng1{Cycle(Range<1, 5>{})};

    constexpr auto rpt{Repeat<3, int>{11}};
    const auto cycleRpt0{Cycle(rpt)};
    const auto cycleRpt1{Cycle(Repeat<3, int>{11})};

    constexpr auto set{Set{1, 2, 1, 3, 4}};
    const auto cycleSet0{Cycle(set)};
    const auto cycleSet1{Cycle(Set{1, 2, 1, 3, 4})};

    constexpr auto str{String{"Hello"}};
    const auto cycleStr0{Cycle(str)};
    const auto cycleStr1{Cycle(String{"Hello"})};

    // Compiler Error: Cycle's parameter must be a cljonic collection
    // const auto c{Cycle("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Cycle(const C& c) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    static_assert(IsCljonicCollection<C>, "Cycle's parameter must be a cljonic collection");

    auto defaultElement{c.DefaultElement()};
    auto empty{0 == c.Count()};
    auto initialElement{empty ? defaultElement : *c.begin()};
    return Iterator{[collection = std::move(c), defaultElement, empty, initialized = false, it = c.begin()] //
                    ([[maybe_unused]] const int _)                                                          //
                    mutable noexcept
                    {
                        auto NextElement =
                            [&, collectionBegin = collection.begin(), collectionEnd = collection.end()] //
                            ()                                                                          //
                            noexcept
                        {
                            if (not initialized)
                            {
                                it = collection.begin();
                                initialized = true;
                            }
                            ++it;
                            if (not(collectionEnd != it))
                                it = collectionBegin;
                            return *it;
                        };
                        return empty ? defaultElement : NextElement();
                    },
                    initialElement};
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_CYCLE_HPP
