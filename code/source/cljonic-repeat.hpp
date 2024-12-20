#ifndef CLJONIC_REPEAT_HPP
#define CLJONIC_REPEAT_HPP

#include <type_traits>
#include "cljonic-collection-type.hpp"

namespace cljonic
{
/** \anchor Repeat
 * The \b Repeat type is a fundamental immutable collection type in cljonic.  It is implemented as a \b lazy \b sequence
 * of repeating values of a specified type. Many \ref Namespace_Core "Core" functions accept Repeat arguments.
 */
template <typename T>
class Repeat
{
    const std::size_t m_elementCount;
    const T m_elementDefault;
    const T m_elementValue;

    class Iterator
    {
        const Repeat& m_repeat;
        std::size_t m_index;

      public:
        Iterator(const Repeat& repeat, const std::size_t index) : m_repeat(repeat), m_index(index)
        {
        }

        int operator*() const
        {
            return m_repeat[m_index];
        }

        Iterator& operator++()
        {
            ++m_index;
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return m_index != other.m_index;
        }
    };

  public:
    /**
    * There are two \b Repeat constructors:
    *     - <b>Repeat(value)</b> returns a \b Repeat of value \b std::numeric_limits<std::size_t>::max() times
    *     - <b>Repeat(count, value)</b> returns a \b Repeat of value \b count times
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        using T = std::variant<int, double, char, const char*>;

        const auto r0{Repeat(1)};            // immutable, 1, std::numeric_limits<std::size_t>::max() times
        const auto r1{Repeat(10, 1)};        // immutable, 1, ten times
        const auto r2{Repeat(T{'x'})};       // immutable, T{'x'}, std::numeric_limits<std::size_t>::max() times
        const auto r3{Repeat(100, T{'x'})};  // immutable, T{'x'}, 100 times

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Repeat>;
    using size_type = std::size_t;
    using value_type = T;

    explicit Repeat(const T& t) noexcept
        : m_elementCount{std::numeric_limits<std::size_t>::max()}, m_elementDefault{T{}}, m_elementValue{t}
    {
    }

    explicit Repeat(const size_type count, const T& t) noexcept
        : m_elementCount{count}, m_elementDefault{T{}}, m_elementValue{t}
    {
    }

    Repeat(const Repeat& other) = default; // Copy constructor
    Repeat(Repeat&& other) = default;      // Move constructor

    [[nodiscard]] Iterator begin() const
    {
        return Iterator{*this, 0};
    }

    [[nodiscard]] Iterator end() const
    {
        return Iterator{*this, m_elementCount};
    }

    const T& operator[](const size_type index) const noexcept
    {
        return ((m_elementCount <= 0) or (index >= m_elementCount)) ? m_elementDefault : m_elementValue;
    }

    [[nodiscard]] size_type Count() const noexcept
    {
        return m_elementCount;
    }
}; // class Repeat

} // namespace cljonic

#endif // CLJONIC_REPEAT_HPP
