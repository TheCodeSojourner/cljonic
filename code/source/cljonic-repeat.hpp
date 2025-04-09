#ifndef CLJONIC_REPEAT_HPP
#define CLJONIC_REPEAT_HPP

#include <concepts>
#include <limits>
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

/** \anchor Repeat
 * The \b Repeat type is a fundamental \b cljonic \b collection type.  It is implemented as a \b lazy \b sequence of a
 * specified length of a specified value. Many \ref Namespace_Core "Core" functions accept Repeat arguments.
 */
template <SizeType MaxElements, typename T>
class Repeat
{
    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create a Repeat bigger than CljonicCollectionMaximumElementCount");

    static constexpr SizeType m_elementCount{maximumElements};
    static constexpr T m_elementDefault{T{}};
    const T m_elementValue;

    class RepeatIterator final
    {
        SizeType m_count;
        const T m_value;

      public:
        using value_type = T;

        constexpr explicit RepeatIterator(const SizeType count, const T& value) noexcept
            : m_count{count}, m_value{value}
        {
        }

        [[nodiscard]] constexpr RepeatIterator(const RepeatIterator& other) noexcept = default; // Copy constructor

        [[nodiscard]] constexpr const T& operator*() const noexcept
        {
            return m_value;
        }

        constexpr RepeatIterator& operator++() noexcept
        {
            ++m_count;
            return *this;
        }

        [[nodiscard]] constexpr bool operator!=(const RepeatIterator& other) const noexcept
        {
            return m_count != other.m_count;
        }

        constexpr RepeatIterator& operator=(const RepeatIterator& other) noexcept
        {
            if (this != &other)
                m_count = other.m_count;
            return *this;
        }
    };

  public:
    /**
    * There are two ways to create a \b Repeat:
    *     - <b>Repeat{value}</b>: \b Repeat of value \b CljonicCollectionMaximumElementCount times
    *     - <b>Repeat<count, valueType>{value}</b>: \b Repeat of valueType value \b count times
    *
    * Note: The \b Repeat type, unlike \b Array and \b Set, specifies the number of elements \b before the type. This is
    *       consistent with \b Clojure where something like <b>(repeat 10 "Hello")<\b> is used to create a repeating
    *       sequence of ten instances of the value "Hello".
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto r0{Repeat{1}};          // 1, CljonicCollectionMaximumElementCount times
        const auto r1{Repeat<10, int>{1}}; // 1, 10 times

        // Compiler Error: Attempt to create a Repeat with invalid value type
        // const auto r{Repeat<1111, int>{11}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Repeat>;
    using size_type = SizeType;
    using value_type = T;

    constexpr explicit Repeat(const T& t) noexcept : m_elementValue{t}
    {
    }

    constexpr explicit Repeat(T&& t) noexcept : m_elementValue{std::forward<T>(t)}
    {
    }

    constexpr Repeat(const Repeat& other) noexcept = default; // Copy constructor
    constexpr Repeat(Repeat&& other) noexcept = default;      // Move constructor

    [[nodiscard]] constexpr bool operator==(const auto& other) const noexcept
    {
        return AreEqualValues(this, other);
    }

    [[nodiscard]] constexpr RepeatIterator begin() noexcept
    {
        return RepeatIterator{0, m_elementValue};
    }

    [[nodiscard]] constexpr RepeatIterator end() noexcept
    {
        return RepeatIterator{m_elementCount, m_elementValue};
    }

    [[nodiscard]] constexpr RepeatIterator begin() const noexcept
    {
        return RepeatIterator{0, m_elementValue};
    }

    [[nodiscard]] constexpr RepeatIterator end() const noexcept
    {
        return RepeatIterator{m_elementCount, m_elementValue};
    }

    [[nodiscard]] constexpr static SizeType Count() noexcept
    {
        return m_elementCount;
    }

    [[nodiscard]] constexpr const T& DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    [[nodiscard]] static constexpr auto MaximumCount() noexcept
    {
        return m_elementCount;
    }
}; // class Repeat

// deduction guide to transform Repeat{1} into Repeat<CljonicCollectionMaximumElementCount, int>{1}
template <typename T>
Repeat(T) -> Repeat<CljonicCollectionMaximumElementCount, T>;

} // namespace cljonic

#endif // CLJONIC_REPEAT_HPP
