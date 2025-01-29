#ifndef CLJONIC_LAZYITERATE_HPP
#define CLJONIC_LAZYITERATE_HPP

#include <concepts>
#include <utility>
#include "cljonic-collection-iterator.hpp"
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{
/** \anchor LazyIterate
 * The \b LazyIterate type is a fundamental immutable collection type in \b cljonic.  It is implemented as a \b lazy
 * \b sequence.  The first element in the sequence is the second parameter, the second element in the sequence is
 * the result of calling the first parameter, which must be a unary function of the type of the second parameter that
 * returns the same type as the type of the second parameter, with the second parameter, the third element in the
 * sequence is the result of calling the first parameter with the second element in the sequence, etc. If we name the
 * first parameter \b "f" and the second parameter \b "x" then the sequence is: \b x, \b f(x), \b f(f(x)),
 * \b f(f(f(x))), etc.
 */
template <typename F, typename T>
class LazyIterate
{
    F m_f;
    T m_nextValue;

  public:
    /**
      * The \b LazyIterate constructor returns an instance of LazyIterate initialized with its parameters.
    ~~~~~{.cpp}
    #include "cljonic.hpp"
    using namespace cljonic;

    int TimesTen(const int x) noexcept
    {
        return x * 10;
    }

    int main()
    {
        auto i0{LazyIterate{[](const double d) { return 1.1 * d; }, 11.1}};
        auto i1{LazyIterate{TimesTen, 1}};

        // Compiler Error: LazyIterate constructor's first parameter is not a unary function of its second parameter
        // const auto i{LazyIterate{[](const double d) { return 1.1 * d; }, "Hello"}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::LazyIterate>;
    using size_type = SizeType;
    using value_type = T;

    constexpr explicit LazyIterate(F&& f, const T& t) noexcept : m_f{std::forward<F>(f)}, m_nextValue{t}
    {
        static_assert(IsUnaryFunction<F, T>,
                      "LazyIterate constructor's first parameter is not a unary function of its second parameter");
    }

    constexpr LazyIterate(const LazyIterate& other) = default; // Copy constructor
    constexpr LazyIterate(LazyIterate&& other) = default;      // Move constructor

    [[nodiscard]] constexpr SizeType Count() const noexcept
    {
        return CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT;
    }

    [[nodiscard]] static constexpr auto MaximumCount() noexcept
    {
        return CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT;
    }

    [[nodiscard]] constexpr auto Next() noexcept
    {
        auto result{m_nextValue};
        m_nextValue = m_f(m_nextValue);
        return result;
    }
}; // class LazyIterate

// Support declarations like: auto v{LazyIterate{[](const int i){ return 1 + i; }, 1}};
// Equivalent to auto v{LazyIterate<type-of-lambda, int>{[](const int i){ return 1 + i; }, 1}};
template <typename F, typename T>
LazyIterate(F&&, const T&) -> LazyIterate<F, T>;

} // namespace cljonic

#endif // CLJONIC_LAZYITERATE_HPP
