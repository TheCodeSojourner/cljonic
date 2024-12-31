#ifndef CLJONIC_CORE_CYCLE_HPP
#define CLJONIC_CORE_CYCLE_HPP

#include <utility>

namespace cljonic
{

namespace core
{

/** CycleCollection
 * The \b CycleCollection type implements as a \b lazy \b sequence composed of the values returned from a function of
 * one parameter that is an index into an ordered collection of a specified length.  The \b CycleCollection type cycles
 * through the ordered collection over and over again repeatedly providing the elements of the ordered collection. Many
 * \ref Namespace_Core "Core" functions accept CycleCollection arguments.
 */
template <typename C>
class CycleCollection
{
    using MaxElementsType = typename C::size_type;
    using ElementType = typename C::value_type;

    const C m_collection;

    [[nodiscard]] MaxElementsType IndexToElementIndex(const MaxElementsType index) const noexcept
    {
        return (0 == m_collection.Count()) ? 0 : (index % m_collection.Count());
    }

    class CycleIterator
    {
        const CycleCollection& m_cycle;
        MaxElementsType m_index;

      public:
        constexpr CycleIterator(const CycleCollection& cycle, const MaxElementsType index) noexcept
            : m_cycle(cycle), m_index(index)
        {
        }

        constexpr auto operator*() const noexcept -> decltype(m_cycle[m_index])
        {
            return m_cycle[m_cycle.IndexToElementIndex(m_index)];
        }

        constexpr CycleIterator& operator++() noexcept
        {
            ++m_index;
            return *this;
        }

        constexpr bool operator!=(const CycleIterator& other) const noexcept
        {
            return m_index != other.m_index;
        }

        constexpr CycleIterator& operator+=(const int value) noexcept
        {
            m_index += value;
            return *this;
        }

        constexpr CycleIterator operator+(const int value) const noexcept
        {
            CycleIterator temp = *this;
            temp += value;
            return temp;
        }
    }; // CycleIterator

  public:
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Cycle>;
    using size_type = MaxElementsType;
    using value_type = ElementType;

    explicit CycleCollection(const C& collection) : m_collection(collection)
    {
    }

    explicit CycleCollection(C&& collection) : m_collection(std::move(collection))
    {
    }

    constexpr CycleCollection(const CycleCollection& other) = default; // Copy constructor
    constexpr CycleCollection(CycleCollection&& other) = default;      // Move constructor

    [[nodiscard]] constexpr CycleIterator begin() const noexcept
    {
        return CycleIterator(*this, 0);
    }

    [[nodiscard]] constexpr CycleIterator end() const noexcept
    {
        return CycleIterator(*this, MaximumCount());
    }

    [[nodiscard]] constexpr ElementType operator[](const MaxElementsType index) const noexcept
    {
        return m_collection[IndexToElementIndex(index)];
    }

    [[nodiscard]] constexpr MaxElementsType Count() const noexcept
    {
        return MaximumCount();
    }

    [[nodiscard]] constexpr ElementType& DefaultElement() const noexcept
    {
        return m_collection.DefaultElement();
    }

    [[nodiscard]] constexpr std::size_t MaximumCount() const noexcept
    {
        return (0 == m_collection.Count()) ? 0 : std::numeric_limits<MaxElementsType>::max();
    }
}; // class CycleCollection

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

    constexpr auto rng{Range<1, 5>{}};
    const auto cycleRng0{Cycle(rng)};
    const auto cycleRng1{Cycle(Range<1, 5>{})};

    constexpr auto rpt{Repeat<3, int>{11}};
    const auto cycleRpt0{Cycle(rpt)};
    const auto cycleRpt1{Cycle(Repeat<3, int>{11})};

    const auto set{Set{1, 2, 1, 3, 4}};
    const auto cycleSet0{Cycle(set)};
    const auto cycleSet1{Cycle(Set{1, 2, 1, 3, 4})};

    constexpr auto str{String{"Hello"}};
    const auto cycleStr0{Cycle(str)};;
    const auto cycleStr1{Cycle(String{"Hello"})};;

    // Compiler Error: Cycle parameter must be a cljonic collection
    // const auto c{Cycle("Hello")};

    return 0;
}
~~~~~
*/
template <typename C>
constexpr auto Cycle(const C& c) noexcept
{
    static_assert(IsCljonicCollection<C>, "Cycle parameter must be a cljonic collection");

    return CycleCollection{c};
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_CYCLE_HPP
