#ifndef CLJONIC_ARRAY_HPP
#define CLJONIC_ARRAY_HPP

#include <cstring>
#include <initializer_list>
#include <type_traits>
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{
/** \anchor Array
 * The \b Array type is a fundamental immutable collection type in cljonic.  It is implemented as a C array, and
 * <b>does not use heap memory</b>.  An \b Array has a specified maximum number of \b ordered elements each of the same
 * specified type (i.e., It is homogenous).  <b>Note that one could create an Array of a \b UNION to get something like
 * heterogeneity.</b> An \b Array is a function of its indexable elements. An \b Array called with an out-of-bounds
 * index will return its \b default \b element. Many \ref Namespace_Core "Core" functions accept Array arguments.
 */
template <typename T, SizeType MaxElements>
class Array
{
    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create an Array bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT");

    SizeType m_elementCount;
    const T m_elementDefault;
    T m_elements[maximumElements]{};

  public:
    /**
    * The \b Array constructor returns an instance of Array initialized with its arguments. If the number of arguments
    * exceeds the maximum number of elements specified, the extra arguments are silently ignored.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto a0{Array<int, 10>{}};                // immutable and empty
        const auto a1{Array<int, 10>{1, 2, 3, 4}};      // immutable and sparse
        const auto a2{Array<int, 4>{1, 2, 3, 4}};       // immutable and full
        const auto a3{Array<int, 4>{1, 2, 3, 4, 5, 6}}; // immutable and full, and the values 5 and 6 are ignored
        const auto a4{Array{1, 2, 3, 4}};               // immutable and full of four int values

        // Compiler Error: Attempt to create an Array bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT
        // const auto a{Array<int, 1111>{0, 2, 4, 5, 6, 7, 8, 9}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Array>;
    using size_type = SizeType;
    using value_type = T;

    constexpr Array() noexcept : m_elementCount(0), m_elementDefault(T{})
    {
    }

    constexpr explicit Array(const std::initializer_list<const T> elements) noexcept
        : m_elementCount(Min(MaximumCount(), elements.size())), m_elementDefault(T{})
    {
        for (SizeType i{0}; i < m_elementCount; ++i)
            m_elements[i] = *(elements.begin() + i);
    }

    constexpr Array(const Array& other) = default; // Copy constructor
    constexpr Array(Array&& other) = default;      // Move constructor

    constexpr const T* begin() const noexcept
    {
        return m_elements;
    }

    constexpr const T* end() const noexcept
    {
        return m_elements + m_elementCount;
    }

    constexpr const T& operator[](const SizeType index) const noexcept
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

    constexpr const T& operator()(const SizeType index) const noexcept
    {
        return this->operator[](index);
    }

    constexpr void MConj(const T& t) noexcept
    {
        if (m_elementCount < MaximumCount())
            m_elements[m_elementCount++] = t;
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept
    {
        return m_elementCount;
    }

    constexpr const T& DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    static constexpr SizeType MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class Array

// Support declarations like: auto v{Array{1, 2, 3}}; // Equivalent to auto v{Array<int, 3>{1, 2, 3}};
template <typename... Args>
Array(Args...) -> Array<std::common_type_t<Args...>, sizeof...(Args)>;

} // namespace cljonic

#endif // CLJONIC_ARRAY_HPP
