#ifndef CLJONIC_REPEAT_HPP
#define CLJONIC_REPEAT_HPP

#include <concepts>
#include <limits>
#include "cljonic-collection-iterator.hpp"
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{
/** \anchor Repeat
 * The \b Repeat type is a fundamental immutable collection type in cljonic.  It is implemented as a \b lazy \b sequence
 * of a specified length of a specified value. Many \ref Namespace_Core "Core" functions accept Repeat arguments.
 */
template <SizeType MaxElements, typename T>
class Repeat : public SequentialInterface<T>
{
    using Iterator = CollectionIterator<Repeat>;

    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create a Repeat bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT");

    const SizeType m_elementCount;
    const T m_elementDefault;
    const T m_elementValue;

  public:
    /**
    * There are two ways to create a \b Repeat:
    *     - <b>Repeat{value}</b> returns a \b Repeat of value \b CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT times
    *     - <b>Repeat<count, valueType>{value}</b> returns a \b Repeat of value, which is of type valueType,
    *     \b count times
    *
    * Note: The \b Repeat type, unlike \b Array and \b Set, specifies the number of elements \b before the type. This is
    *       consistent with \b Clojure where something like <b>(repeat 10 "Hello")<\b> is used to create a repeating
    *       sequence of ten instances of the value "Hello".
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto r0{Repeat{1}};              // immutable, 1, CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT times
        const auto r1{Repeat<10, int>{1}};     // immutable, 1, ten times

        // Compiler Error: Attempt to create a Repeat bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT
        // const auto r{Repeat<1111, int>{1}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Repeat>;
    using size_type = SizeType;
    using value_type = T;

    constexpr explicit Repeat(const T& t) noexcept
        : m_elementCount{maximumElements}, m_elementDefault{T{}}, m_elementValue{t}
    {
    }

    constexpr Repeat(const Repeat& other) = default; // Copy constructor
    constexpr Repeat(Repeat&& other) = default;      // Move constructor

    [[nodiscard]] constexpr Iterator begin() const noexcept
    {
        return Iterator{*this, 0};
    }

    [[nodiscard]] constexpr Iterator end() const noexcept
    {
        return Iterator{*this, m_elementCount};
    }

    constexpr T operator[](const SizeType index) const noexcept override
    {
        return ((m_elementCount <= 0) or (index >= m_elementCount)) ? m_elementDefault : m_elementValue;
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept override
    {
        return m_elementCount;
    }

    constexpr const T& DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    static constexpr auto MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class Repeat

// deduction guide to transform Repeat{1} into Repeat<CljonicCollectionMaximumElementCount, int>(1)
template <typename T>
Repeat(T) -> Repeat<CljonicCollectionMaximumElementCount, T>;

} // namespace cljonic

#endif // CLJONIC_REPEAT_HPP
