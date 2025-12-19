#ifndef CLJONIC_ARRAY_HPP
#define CLJONIC_ARRAY_HPP

#include <concepts>
#include <cstring>
#include <initializer_list>
#include <type_traits>
#include "cljonic-collection-iterator.hpp"
#include "cljonic-collection-type.hpp"
#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{
/** \anchor Array
 * The \b Array type is a fundamental immutable collection type in cljonic.  It is implemented as a C array, and
 * <b>does not use heap memory</b>.  An \b Array has a specified maximum number of \b ordered elements each of the same
 * specified type (i.e., It is homogenous).  <b>Note that one could create an Array of a \b UNION to get something like
 * heterogeneity.</b> An \b Array is a function of its indexable elements. An \b Array called with an out-of-bounds
 * index will return its \b default \b element. Many \ref Namespace_Core "Core" functions accept Array arguments.
 *
 * The \b Array constructor returns an instance of Array initialized with its arguments.
 ~~~~~{.cpp}
 #include "cljonic.hpp"
 using namespace cljonic;
 int main()
 {
     constexpr auto a0{Array<int, 10>{}};           // immutable and empty
     constexpr auto a1{Array<int, 10>{1, 2, 3, 4}}; // immutable and sparse
     constexpr auto a2{Array<int, 4>{1, 2, 3, 4}};  // immutable and full
     constexpr auto a4{Array{1, 2, 3, 4}};          // immutable and full of four int values

     // Compiler Error: Array initialized with too many elements
     // constexpr auto a{Array<int, 4>{0, 2, 4, 5, 6, 7, 8, 9}};

     // Compiler Error: Attempt to create an Array bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT
     // constexpr auto a{Array<int, 1111>{0, 2, 4, 5, 6, 7, 8, 9}};

     return 0;
 }
 ~~~~~
 */
template <ValidCljonicContainerElementType T, SizeType MaxElements>
class Array : public IndexInterface<T>
{
    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create an Array bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT");

    SizeType m_elementCount;
    T m_elementDefault;
    T m_elements[maximumElements]{};

    template <typename U, SizeType N>
    constexpr friend void MConj(Array<U, N>& array, const U& value);

    template <typename U, SizeType N>
    constexpr friend void MSet(Array<U, N>& array, const U& value, const SizeType index);

    [[nodiscard]] constexpr auto ValueAtIndex(const SizeType index) const noexcept
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

  public:
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Array>;
    using size_type = SizeType;
    using value_type = T;

    constexpr Array() noexcept : m_elementCount(0), m_elementDefault(T{})
    {
    }

    template <typename... Args>
    constexpr explicit Array(Args&&... args) noexcept : m_elementCount(0), m_elementDefault(T{})
    {
        static_assert(sizeof...(Args) <= MaximumCount(), "Array initialized with too many elements");
        ((m_elements[m_elementCount++] = args), ...);
    }

    constexpr Array(const Array& other) noexcept = default; // Copy constructor
    constexpr Array(Array&& other) noexcept = default;      // Move constructor

  private:
    using Iterator = CollectionIterator<Array>;

  public:
    [[nodiscard]] constexpr Iterator begin() const noexcept
    {
        return Iterator{*this, 0};
    }

    [[nodiscard]] constexpr Iterator end() const noexcept
    {
        return Iterator{*this, m_elementCount};
    }

    [[nodiscard]] constexpr T operator[](const SizeType index) const noexcept override
    {
        return ValueAtIndex(index);
    }

    [[nodiscard]] constexpr T operator()(const SizeType index) const noexcept
    {
        return this->operator[](index);
    }

    constexpr Array& operator=(const Array& other) noexcept
    {
        if (this != &other)
        {
            m_elementCount = other.m_elementCount;
            m_elementDefault = other.m_elementDefault;
            for (SizeType i{0}; i < m_elementCount; ++i)
                m_elements[i] = other.m_elements[i];
        }
        return *this;
    }

    constexpr Array& operator=(Array&& other) noexcept
    {
        return *this = other; // Delegate to copy assignment
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept override
    {
        return m_elementCount;
    }

    [[nodiscard]] constexpr const T& DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    [[nodiscard]] constexpr bool ElementAtIndexIsEqualToElement(const SizeType index,
                                                                const T& element) const noexcept override
    {
        return (index < m_elementCount) and AreEqual(ValueAtIndex(index), element);
    }

    [[nodiscard]] static consteval SizeType MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class Array

// Support declarations like: auto v{Array{1, 2, 3}}; // Equivalent to auto v{Array<int, 3>{1, 2, 3}};
template <typename... Args>
Array(Args...) -> Array<std::common_type_t<Args...>, sizeof...(Args)>;

template <typename U, SizeType N>
constexpr void MConj(Array<U, N>& array, const U& value)
{
    if (array.m_elementCount < array.MaximumCount())
        array.m_elements[array.m_elementCount++] = value;
}

template <typename U, SizeType N>
constexpr void MSet(Array<U, N>& array, const U& value, const SizeType index)
{
    if (index < array.m_elementCount)
        array.m_elements[index] = value;
}

} // namespace cljonic

#endif // CLJONIC_ARRAY_HPP
