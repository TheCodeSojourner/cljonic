#ifndef CLJONIC_SET_HPP
#define CLJONIC_SET_HPP

#include <concepts>
#include <cstring>
#include <initializer_list>
#include <type_traits>
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

/** \anchor Set
 * The \b Set type is a fundamental \b cljonic \b collection type.  It is implemented as a C array, and <b>does not use
 * dynamic memory</b>. A \b Set has a specified maximum number of \b unique \b unordered elements each of the same
 * specified type (i.e., It is homogenous).  <b>Note that one could create a Set of a \b UNION to get something like
 * heterogeneity.</b> A \b Set is a function of its elements. A \b Set called with a value not contained within the set
 * will return its \b default \b element.  Many \ref Namespace_Core "Core" functions accept Set arguments.
 */
template <typename T, SizeType MaxElements>
class Set
{
    static_assert(not std::floating_point<T>,
                  "Floating point types should not be compared for equality, hence Sets of floating point types are "
                  "not allowed");

    static_assert(std::equality_comparable<T>, "A Set type must be equality comparable");

    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create a Set bigger than CljonicCollectionMaximumElementCount");

    SizeType m_elementCount{0};
    const T m_elementDefault{T{}};
    std::array<T, maximumElements> m_elements{};

    class SetIterator final
    {
        const T* m_elementsPointer;

      public:
        using value_type = T;

        constexpr explicit SetIterator(const T* elementsPointer) noexcept : m_elementsPointer{elementsPointer}
        {
        }

        [[nodiscard]] constexpr const T& operator*() const noexcept
        {
            return *m_elementsPointer;
        }

        constexpr SetIterator& operator++() noexcept
        {
            ++m_elementsPointer;
            return *this;
        }

        [[nodiscard]] constexpr bool operator!=(const SetIterator& other) const noexcept
        {
            return m_elementsPointer != other.m_elementsPointer;
        }
    };

    [[nodiscard]] constexpr bool IsUniqueElement(const T& element) const noexcept
    {
        const auto endIt{m_elements.begin() + m_elementCount};
        for (auto it{m_elements.begin()}; it != endIt; ++it)
            if (AreEqualValues(element, *it))
                return false;
        return true;
    }

    template <typename F, typename E>
    [[nodiscard]] constexpr bool IsUniqueElementBy(F&& f, E&& element) const noexcept
    {
        const auto endIt{m_elements.begin() + m_elementCount};
        for (auto it{m_elements.begin()}; it != endIt; ++it)
            if (AreEqualValuesBy(std::forward<F>(f), std::forward<E>(element), *it))
                return false;
        return true;
    }

  public:
    /**
    * The \b Set constructor returns an instance of Set initialized with the unique elements in its arguments. If the
    * number of unique elements in its arguments exceeds the Set's maximum allowed number of elements, the extra unique
    * elements in its arguments are \b silently \b ignored.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    class NonEqualityComparable
    {
        int value;

      public:
        explicit NonEqualityComparable(int value) : value(value)
        {
        }
    };

    int main()
    {
        const auto s0{Set<int, 10>{}};                // empty
        const auto s1{Set<int, 10>{1, 2, 3, 4}};      // sparse
        const auto s2{Set<int, 4>{1, 2, 3, 4}};       // full
        const auto s3{Set<int, 4>{1, 2, 3, 4, 5, 6}}; // full, but 5 and 6 are ignored
        const auto s4{Set<int, 4>{1, 2, 1, 4, 5, 6}}; // full of 1, 2, 4, 5
        const auto s5{Set{1, 2, 3, 4}};               // full of four int values
        const auto s6{Set{1, 2, 1, 4}};               // full of three unique int values

        // Compiler Error:
        //     Floating point types should not be compared for equality,
        //     hence Sets of floating point types are not allowed
        // const auto s{Set{1.1, 2.2}};

        // Compiler Error: A Set type must be equality comparable
        // const auto s{Set<NonEqualityComparable, 10>{}};

        // Compiler Error: Attempt to create a Set bigger than CljonicCollectionMaximumElementCount
        // const auto s{Set<int, 1111>{}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Set>;
    using size_type = SizeType;
    using value_type = T;

    constexpr Set() noexcept = default;

    constexpr Set(const std::initializer_list<const T> elements) noexcept : m_elementCount(0), m_elementDefault(T{})
    {
        // #lizard forgives -- The complexity of this function is acceptable

        for (const auto& element : elements)
        {
            if (m_elementCount == MaximumCount())
                break; // LCOV_EXCL_LINE - This line of code may only execute at compile-time
            if (IsUniqueElement(element))
                m_elements[m_elementCount++] = element;
        }
    }

    constexpr Set(const Set& other) noexcept = default; // Copy constructor
    constexpr Set(Set&& other) noexcept = default;      // Move constructor

    [[nodiscard]] constexpr T operator()(const T& t) const noexcept
    {
        return Contains(t) ? t : m_elementDefault;
    }

    [[nodiscard]] constexpr bool operator==(const auto& other) const noexcept
    {
        return AreEqualValues(this, other);
    }

    [[nodiscard]] constexpr SetIterator begin() noexcept
    {
        return SetIterator{m_elements.data()};
    }

    [[nodiscard]] constexpr SetIterator end() noexcept
    {
        return SetIterator{m_elements.data() + m_elementCount};
    }

    [[nodiscard]] constexpr SetIterator begin() const noexcept
    {
        return SetIterator{m_elements.data()};
    }

    [[nodiscard]] constexpr SetIterator end() const noexcept
    {
        return SetIterator{m_elements.data() + m_elementCount};
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept
    {
        return m_elementCount;
    }

    [[nodiscard]] constexpr bool Contains(const T& element) const noexcept
    {
        return not IsUniqueElement(element);
    }

    template <typename F, typename E>
    [[nodiscard]] constexpr bool ContainsBy(F&& f, E&& element) const noexcept
    {
        return not IsUniqueElementBy(std::forward<F>(f), std::forward<E>(element));
    }

    [[nodiscard]] constexpr int DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    [[nodiscard]] static constexpr SizeType MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class Set

// Support declarations like: auto v{Set{1, 2, 3}}; // Equivalent to auto v{Set<int, 3>{1, 2, 3}};
template <typename... Args>
Set(Args...) -> Set<std::common_type_t<Args...>, sizeof...(Args)>;

// Support declarations like: auto v{Set{}}; // Equivalent to auto v{Set<int, 0>{}};
template <typename T = int>
Set() -> Set<T, 0>;

} // namespace cljonic

#endif // CLJONIC_SET_HPP
