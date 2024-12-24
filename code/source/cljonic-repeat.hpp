#ifndef CLJONIC_REPEAT_HPP
#define CLJONIC_REPEAT_HPP

#include <concepts>
#include <limits>
#include "cljonic-collection-iterator.hpp"
#include "cljonic-collection-type.hpp"

namespace cljonic
{
/** \anchor Repeat
 * The \b Repeat type is a fundamental immutable collection type in cljonic.  It is implemented as a \b lazy \b sequence
 * of a specified length of a specified value. Many \ref Namespace_Core "Core" functions accept Repeat arguments.
 */
template <std::size_t MaxElements, typename T>
class Repeat
{
    using Iterator = CollectionIterator<Repeat>;

    const std::size_t m_elementCount;
    const T m_elementDefault;
    const T m_elementValue;

  public:
    /**
    * There are two ways to create a \b Repeat:
    *     - <b>Repeat{value}</b> returns a \b Repeat of value \b std::numeric_limits<std::size_t>::max() times
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
        using T = std::variant<int, double, char, const char*>;

        const auto r0{Repeat{1}};              // immutable, 1, std::numeric_limits<std::size_t>::max() times
        const auto r1{Repeat<10, int>{1}};     // immutable, 1, ten times
        const auto r2{Repeat{T{'x'}}};         // immutable, T{'x'}, std::numeric_limits<std::size_t>::max() times
        const auto r3{Repeat<100, T>{T{'x'}}}; // immutable, T{'x'}, 100 times

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Repeat>;
    using size_type = std::size_t;
    using value_type = T;

    explicit Repeat(const T& t) noexcept : m_elementCount{MaxElements}, m_elementDefault{T{}}, m_elementValue{t}
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

    const T& DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    static constexpr auto MaximumCount() noexcept
    {
        return MaxElements;
    }
}; // class Repeat

// deduction guide to transform Repeat{1} into Repeat<std::numeric_limits<std::size_t>::max(), int>(1)
template <typename T>
Repeat(T) -> Repeat<std::numeric_limits<std::size_t>::max(), T>;

} // namespace cljonic

#endif // CLJONIC_REPEAT_HPP
