#ifndef CLJONIC_SET_HPP
#define CLJONIC_SET_HPP

#include <concepts>
#include <cstring>
#include <initializer_list>
#include <type_traits>
#include "cljonic-collection-type.hpp"
#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

/** \anchor Set
 * The \b Set type is a fundamental immutable collection type in cljonic.  It is implemented as a C array, and
 * <b>does not use dynamic memory</b>. A \b Set has a specified maximum number of \b unique \b unordered elements each
 * of the same specified type (i.e., It is homogenous).  <b>Note that one could create a Set of a \b UNION to get
 * something like heterogeneity.</b> A \b Set is a function of its elements. A \b Set called with a value not contained
 * within the set will return its \b default \b element.  Many \ref Namespace_Core "Core" functions accept Set
 * arguments.
 */
template <ValidCljonicContainerElementType T, SizeType MaxElements>
class Set : public IndexInterface<T>
{
    static_assert(not std::floating_point<T>,
                  "Floating point types should not be compared for equality, hence Sets of floating point types are "
                  "not allowed");

    static_assert(std::equality_comparable<T>, "A Set type must be equality comparable");

    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create a Set bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT");

    SizeType m_elementCount;
    const T m_elementDefault;
    T m_elements[maximumElements]{};

    [[nodiscard]] constexpr bool IsUniqueElementBy(const auto& f, const T& element) const noexcept
    {
        auto result{true};
        for (SizeType i{0}; (result and (i < m_elementCount)); ++i)
            result = not AreEqualBy(f, element, m_elements[i]);
        return result;
    }

    [[nodiscard]] constexpr bool IsUniqueElement(const T& element) const noexcept
    {
        auto result{true};
        for (SizeType i{0}; (result and (i < m_elementCount)); ++i)
            result = not AreEqual(element, m_elements[i]);
        return result;
    }

  public:
    /**
    * The \b Set constructor returns an instance of Set initialized with the unique elements in its arguments.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto s0{Set<int, 10>{}};                // immutable, empty
        const auto s1{Set<int, 10>{1, 2, 3, 4}};      // immutable, sparse
        const auto s2{Set<int, 4>{1, 2, 3, 4}};       // immutable, full
        const auto s5{Set{1, 2, 3, 4}};               // immutable, full of four int values
        const auto s6{Set{1, 2, 1, 4}};               // immutable, sparse with three unique int values

        // Compiler Error: Set initialized with too many elements
        // constexpr auto s{Set<int, 4>{0, 2, 4, 5, 6, 7, 8, 9}};

        // Compiler Error:
        //     Floating point types should not be compared for equality, hence Sets of floating point types are not
    allowed
        // const auto s{Set{1.1, 2.2}};

        // Compiler Error: A Set type must be equality comparable
        // struct NonComparable
        // {
        //     bool operator==(const NonComparable&) const = delete;
        // };
        // const auto s{Set<NonComparable, 10>{}};

        // Compiler Error: Attempt to create a Set bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT
        // const auto s{Set<int, 1111>{}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Set>;
    using size_type = SizeType;
    using value_type = T;

    constexpr Set() noexcept : m_elementCount(0), m_elementDefault(T{})
    {
    }

    template <typename... Args>
    constexpr explicit Set(Args... elements) noexcept : m_elementCount(0), m_elementDefault(T{})
    {
        static_assert(sizeof...(Args) <= MaximumCount(), "Set initialized with too many elements");
        ((IsUniqueElement(elements) ? (m_elements[m_elementCount++] = elements, void()) : void()), ...);
    }

    constexpr Set(const Set& other) noexcept = default; // Copy constructor
    constexpr Set(Set&& other) noexcept = default;      // Move constructor

    [[nodiscard]] constexpr const T* begin() const noexcept
    {
        return m_elements;
    }

    [[nodiscard]] constexpr const T* end() const noexcept
    {
        return m_elements + m_elementCount;
    }

    [[nodiscard]] constexpr T operator[](const SizeType index) const noexcept override
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

    [[nodiscard]] constexpr T operator()(const T& t) const noexcept
    {
        return Contains(t) ? t : m_elementDefault;
    }

    constexpr Set& operator=(const Set& other) noexcept
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

    constexpr Set& operator=(Set&& other) noexcept
    {
        return *this = other; // Delegate to copy assignment
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept override
    {
        return m_elementCount;
    }

    [[nodiscard]] constexpr bool ContainsBy(const auto& f, const T& element) const noexcept
    {
        return not IsUniqueElementBy(f, element);
    }

    [[nodiscard]] constexpr bool Contains(const T& element) const noexcept
    {
        return not IsUniqueElement(element);
    }

    [[nodiscard]] constexpr int DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    [[nodiscard]] constexpr bool ElementAtIndexIsEqualToElement(const SizeType index,
                                                                const T& element) const noexcept override
    {
        return (index < m_elementCount) and Contains(element);
    }

    [[nodiscard]] static consteval SizeType MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class Set

// Support declarations like: auto v{Set{1, 2, 3}}; // Equivalent to auto v{Set<int, 3>{1, 2, 3}};
template <typename... Args>
Set(Args...) -> Set<std::common_type_t<Args...>, sizeof...(Args)>;

} // namespace cljonic

#endif // CLJONIC_SET_HPP
