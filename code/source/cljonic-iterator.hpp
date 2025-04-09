#ifndef CLJONIC_ITERATOR_HPP
#define CLJONIC_ITERATOR_HPP

#include <concepts>
#include <utility>
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{
/** \anchor Iterator
 * The \b Iterator type is a fundamental \b cljonic \b collection type.  It is implemented as a \b lazy \b sequence.
 * The first parameter must be a unary function of the type of the second parameter that returns the same type as the
 * type of the second parameter. The first element in the collection is the second parameter, the second element in the
 * collection is the result of calling the first parameter with the second parameter, the third element in the
 * collection is the result of calling the first parameter with the second element in the collection, etc. If we name
 * the first parameter \b "f" and the second parameter \b "x" then the sequence is: \b x, \b f(x), \b f(f(x)),
 * \b f(f(f(x))), etc.
 */
template <typename F, typename T>
class Iterator
{
    static constexpr T m_elementDefault{};
    F m_f;
    T m_initialValue;

    class IteratorIterator final
    {
        SizeType m_count;
        F m_f; // Remove const qualifier
        T m_value;

      public:
        using value_type = T;

        constexpr IteratorIterator(const SizeType count, F f, const T& value) noexcept
            : m_count{count}, m_f{std::move(f)}, m_value{value}
        {
        }

        [[nodiscard]] constexpr const T& operator*() const noexcept
        {
            return m_value;
        }

        constexpr IteratorIterator& operator++() noexcept
        {
            ++m_count;
            m_value = m_f(m_value);
            return *this;
        }

        [[nodiscard]] constexpr bool operator!=(const IteratorIterator& other) const noexcept
        {
            return m_count != other.m_count;
        }
    };

  public:
    /**
      * The \b Iterator constructor returns an instance of Iterator initialized with its parameters.
    ~~~~~{.cpp}
    #include "cljonic.hpp"
    using namespace cljonic;

    int TimesTen(const int x) noexcept
    {
        return x * 10;
    }

    int main()
    {
        constexpr auto OneOrTwo = [](const int i) { return (1 == i) ? 2 : 1; };

        const auto i0{Iterator{[](const double d) { return 1.1 * d; }, 11.1}};
        const auto i1{Iterator{TimesTen, 1}};
        const auto i2{Iterator{OneOrTwo, 1}};

        // Compiler Error: Iterator constructor's first parameter is not a unary function of its second parameter
        // const auto i{Iterator{[](const double d) { return 1.1 * d; }, "Hello"}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Iterator>;
    using size_type = SizeType;
    using value_type = T;

    template <typename Func, typename InitValue>
    Iterator(Func&& f, InitValue&& initialValue) noexcept
        : m_f(std::forward<Func>(f)), m_initialValue(std::forward<InitValue>(initialValue))
    {
        static_assert(IsUnaryFunction<F, T>,
                      "Iterator constructor's first parameter is not a unary function of its second parameter");
    }

    constexpr Iterator(const Iterator& other) noexcept = default; // Copy constructor
    constexpr Iterator(Iterator&& other) noexcept = default;      // Move constructor

    [[nodiscard]] constexpr bool operator==(const auto& other) const noexcept
    {
        return AreEqualValues(this, other);
    }

    [[nodiscard]] constexpr IteratorIterator begin() noexcept
    {
        return {0, m_f, m_initialValue};
    }

    [[nodiscard]] constexpr IteratorIterator end() noexcept
    {
        return {Count(), m_f, m_initialValue};
    }

    [[nodiscard]] constexpr IteratorIterator begin() const noexcept
    {
        return IteratorIterator{0, m_f, m_initialValue};
    }

    [[nodiscard]] constexpr IteratorIterator end() const noexcept
    {
        return IteratorIterator{Count(), m_f, m_initialValue};
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept
    {
        return CljonicCollectionMaximumElementCount;
    }

    [[nodiscard]] constexpr const T& DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    [[nodiscard]] static constexpr auto MaximumCount() noexcept
    {
        return CljonicCollectionMaximumElementCount;
    }
}; // class Iterator

// Support declarations like:
//     auto v{Iterator{[](const int i){ return 1 + i; }, 1}};
//     auto v{Iterator{F1, 1}};
template <typename F, typename T>
Iterator(F, T) -> Iterator<F, T>;

} // namespace cljonic

#endif // CLJONIC_ITERATOR_HPP
