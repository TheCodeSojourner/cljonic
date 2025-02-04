#ifndef CLJONIC_ITERATOR_HPP
#define CLJONIC_ITERATOR_HPP

#include <concepts>
#include <utility>
#include "cljonic-collection-iterator.hpp"
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{
/** \anchor Iterator
 * The \b Iterator type is a fundamental immutable collection type in \b cljonic.  It is implemented as a \b lazy
 * \b sequence.  The first element in the sequence is the second parameter, the second element in the sequence is
 * the result of calling the first parameter, which must be a unary function of the type of the second parameter that
 * returns the same type as the type of the second parameter, with the second parameter, the third element in the
 * sequence is the result of calling the first parameter with the second element in the sequence, etc. If we name the
 * first parameter \b "f" and the second parameter \b "x" then the sequence is: \b x, \b f(x), \b f(f(x)),
 * \b f(f(f(x))), etc.
 */
template <typename F, typename T>
class Iterator
{
    T m_elementDefault;
    F m_f;
    T m_initialValue;

    class Itr
    {
        F m_f;
        SizeType m_index;
        T m_nextValue;

      public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Itr(const Iterator& iterator, SizeType index)
            : m_f{iterator.m_f}, m_index{index}, m_nextValue{iterator.m_initialValue}
        {
        }

        T operator*() const
        {
            return m_nextValue;
        }

        Itr& operator++()
        {
            if (m_index < Iterator::MaximumCount())
            {
                m_nextValue = m_f(m_nextValue);
                ++m_index;
            }
            return *this;
        }

        Itr operator++(int)
        {
            auto result{*this};
            ++(*this);
            return result;
        }

        Itr operator+(SizeType n) const
        {
            auto result{*this};
            for (SizeType i{0}; (i < n) and (result.m_index < Iterator::MaximumCount()); ++i)
                ++result;
            return result;
        }

        // CAUTION: This operator assumes it will only ever be called like this = (end() - n)
        Itr operator-(SizeType n) const
        {
            auto result{*this};
            result.m_index = (m_index > n) ? (m_index - n) : 0;
            return result;
        }

        bool operator!=(const Itr& other) const
        {
            return m_index != other.m_index;
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
        auto i0{Iterator{[](const double d) { return 1.1 * d; }, 11.1}};
        auto i1{Iterator{TimesTen, 1}};

        // Compiler Error: Iterator constructor's first parameter is not a unary function of its second parameter
        // const auto i{Iterator{[](const double d) { return 1.1 * d; }, "Hello"}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Iterator>;
    using size_type = SizeType;
    using value_type = T;

    constexpr explicit Iterator(F&& f, const T& t) noexcept : m_f{std::forward<F>(f)}, m_initialValue{t}
    {
        static_assert(IsUnaryFunction<F, T>,
                      "Iterator constructor's first parameter is not a unary function of its second parameter");
    }

    constexpr Iterator(const Iterator& other) = default; // Copy constructor
    constexpr Iterator(Iterator&& other) = default;      // Move constructor

    Itr begin() const
    {
        return Itr(*this, 0);
    }

    Itr end() const
    {
        return Itr(*this, MaximumCount());
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept
    {
        return CljonicCollectionMaximumElementCount;
    }

    constexpr const T& DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    [[nodiscard]] static constexpr auto MaximumCount() noexcept
    {
        return CljonicCollectionMaximumElementCount;
    }
}; // class Iterator

// Support declarations like: auto v{Iterator{[](const int i){ return 1 + i; }, 1}};
// Equivalent to auto v{Iterator<type-of-lambda, int>{[](const int i){ return 1 + i; }, 1}};
template <typename F, typename T>
Iterator(F&&, const T&) -> Iterator<F, T>;

} // namespace cljonic

#endif // CLJONIC_ITERATOR_HPP
