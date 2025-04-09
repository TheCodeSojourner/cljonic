#ifndef CLJONIC_ARRAY_HPP
#define CLJONIC_ARRAY_HPP

#include <array>
#include <cstring>
#include <initializer_list>
#include <type_traits>
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{
/** \anchor Array
 * The \b Array type is a fundamental \b cljonic \b collection type.  It is implemented as a std::array, and <b>does not
 * use heap memory</b>.  An \b Array has a specified maximum number of \b ordered elements each of the same specified
 * type (i.e., It is homogenous). <b>Note that one could create an Array of a heterogeneous type, like a \b UNION, to
 * get something like heterogeneity.</b> An \b Array is a function of its indexable elements. An \b Array called with an
 * out-of-bounds index will return its \b default \b element. Many \ref Namespace_Core "Core" functions accept Array
 * arguments.
 */
template <typename T, SizeType MaxElements>
class Array
{
    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create an Array bigger than CljonicCollectionMaximumElementCount");

    SizeType m_elementCount{0};
    T m_elementDefault{T{}};
    std::array<T, MaxElements> m_elements{};

    template <typename U, SizeType N>
    constexpr friend void MConj(Array<U, N>& array, const U& value) noexcept;

    template <typename U, SizeType N>
    constexpr friend void MSet(Array<U, N>& array, const U& value, const SizeType index) noexcept;

    class ArrayIterator final
    {
        const T* m_elementsPointer;

      public:
        using value_type = T;

        constexpr explicit ArrayIterator(const T* elementsPointer) noexcept : m_elementsPointer{elementsPointer}
        {
        }

        [[nodiscard]] constexpr const T& operator*() const noexcept
        {
            return *m_elementsPointer;
        }

        constexpr ArrayIterator& operator++() noexcept
        {
            ++m_elementsPointer;
            return *this;
        }

        [[nodiscard]] constexpr bool operator!=(const ArrayIterator& other) const noexcept
        {
            return m_elementsPointer != other.m_elementsPointer;
        }
    };

  public:
    /**
    * The \b Array constructor returns an instance of Array initialized with its arguments. If the number of arguments
    * exceeds the Array's maximum allowed number of elements the extra arguments are \b silently \b ignored.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        constexpr auto a0{Array<int, 10>{}};                // empty
        constexpr auto a1{Array<int, 10>{1, 2, 3, 4}};      // sparse
        constexpr auto a2{Array<int, 4>{1, 2, 3, 4}};       // full
        constexpr auto a3{Array<int, 4>{1, 2, 3, 4, 5, 6}}; // full, and the values 5 and 6 are silently ignored
        constexpr auto a4{Array{1, 2, 3, 4}};               // full of four int values

        // Compiler Error: Attempt to create an Array bigger than CljonicCollectionMaximumElementCount
        // constexpr auto a{Array<int, 1111>{0, 2, 4, 5, 6, 7, 8, 9}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Array>;
    using size_type = SizeType;
    using value_type = T;

    constexpr Array() noexcept = default;

    constexpr Array(const std::initializer_list<T>& init) noexcept
        : m_elementCount{std::min(init.size(), m_elements.size())}, m_elementDefault(T{})
    {
        std::copy(init.begin(), (init.begin() + m_elementCount), m_elements.begin());
    }

    constexpr Array(const Array& other) noexcept = default; // Copy constructor
    constexpr Array(Array&& other) noexcept = default;      // Move constructor

    [[nodiscard]] constexpr bool operator==(const auto& other) const noexcept
    {
        return AreEqualValues(this, other);
    }

    [[nodiscard]] constexpr ArrayIterator begin() noexcept
    {
        return ArrayIterator{m_elements.data()};
    }

    [[nodiscard]] constexpr ArrayIterator end() noexcept
    {
        return ArrayIterator{m_elements.data() + m_elementCount};
    }

    [[nodiscard]] constexpr ArrayIterator begin() const noexcept
    {
        return ArrayIterator{m_elements.data()};
    }

    [[nodiscard]] constexpr ArrayIterator end() const noexcept
    {
        return ArrayIterator{m_elements.data() + m_elementCount};
    }

    [[nodiscard]] constexpr T operator()(const SizeType index) const noexcept
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept
    {
        return m_elementCount;
    }

    [[nodiscard]] constexpr const T& DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    [[nodiscard]] static constexpr SizeType MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class Array

// Support declarations like: auto v{Array{1, 2, 3}}; // Equivalent to auto v{Array<int, 3>{1, 2, 3}};
template <typename... Args>
Array(Args...) -> Array<std::common_type_t<Args...>, sizeof...(Args)>;

// Support declarations like: auto v{Array{}}; // Equivalent to auto v{Array<int, 0>{}};
template <typename T = int>
Array() -> Array<T, 0>;

template <typename U, SizeType N>
constexpr void MConj(Array<U, N>& array, const U& value) noexcept
{
    if (array.m_elementCount < array.MaximumCount())
        array.m_elements[array.m_elementCount++] = value;
}

template <typename U, SizeType N>
constexpr void MSet(Array<U, N>& array, const U& value, const SizeType index) noexcept
{
    if (index < array.m_elementCount)
        array.m_elements[index] = value;
}

} // namespace cljonic

#endif // CLJONIC_ARRAY_HPP
