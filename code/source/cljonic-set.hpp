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
 * The \b Set type is a fundamental immutable collection type in cljonic.  It is implemented as a C array, and
 * <b>does not use dynamic memory</b>. A \b Set has a specified maximum number of \b unique \b unordered elements each
 * of the same specified type (i.e., It is homogenous).  <b>Note that one could create a Set of a \b UNION or
 * \b std::variant to get something like heterogeneity.</b>  Many \ref Namespace_Core "Core" functions accept Set
 * arguments.
 */
template <typename T, std::size_t MaxElements>
class Set
{
    static_assert(not std::floating_point<T>,
                  "Floating point types should not be compared for equality, hence Sets of floating point types are "
                  "not allowed");

    static_assert(std::equality_comparable<T>, "A Set type must be equality comparable");

    using MaxElementsType = decltype(MaxElements);

    MaxElementsType m_elementCount;
    const T m_elementDefault;
    T m_elements[MaxElements];

    bool IsUniqueElementBy(const auto& f, const T& element) const noexcept
    {
        auto result{true};
        for (MaxElementsType i = 0; (result and (i < m_elementCount)); ++i)
            result = not AreEqualBy(f, element, m_elements[i]);
        return result;
    }

    bool IsUniqueElement(const T& element) const noexcept
    {
        auto result{true};
        for (MaxElementsType i = 0; (result and (i < m_elementCount)); ++i)
            result = not AreEqual(element, m_elements[i]);
        return result;
    }

  public:
    /**
    * The \b Set constructor returns an instance of Set initialized with the unique elements in its arguments. If the
    * number of unique elements in its arguments exceeds the maximum number of elements, the extra unique elements in
    * its arguments are silently ignored.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto s0{Set<int, 10>{}};                // immutable, empty
        const auto s1{Set<int, 10>{1, 2, 3, 4}};      // immutable, sparse
        const auto s2{Set<int, 4>{1, 2, 3, 4}};       // immutable, full
        const auto s3{Set<int, 4>{1, 2, 3, 4, 5, 6}}; // immutable, full, but 5 and 6 are ignored
        const auto s4{Set<int, 4>{1, 2, 1, 4, 5, 6}}; // immutable, full with 1, 2, 4, 5
        const auto s5{Set{1, 2, 3, 4}};               // immutable, full of four int values
        const auto s6{Set{1, 2, 1, 4}};               // immutable, full of three unique int values

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Set>;
    using size_type = MaxElementsType;
    using value_type = T;

    Set() noexcept : m_elementCount(0), m_elementDefault(T{})
    {
    }

    explicit Set(const std::initializer_list<const T> elements) noexcept : m_elementCount(0), m_elementDefault(T{})
    {
        // #lizard forgives -- The complexity of this function is acceptable
        for (const auto& element : elements)
        {
            if (m_elementCount == MaxElements)
                break;
            if (IsUniqueElement(element))
                m_elements[m_elementCount++] = element;
        }
    }

    Set(const Set& other) = default; // Copy constructor
    Set(Set&& other) = default;      // Move constructor

    const T* begin() const noexcept
    {
        return m_elements;
    }

    const T* end() const noexcept
    {
        return m_elements + m_elementCount;
    }

    const T& operator[](const MaxElementsType index) const noexcept
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

    [[nodiscard]] MaxElementsType Count() const noexcept
    {
        return m_elementCount;
    }

    bool ContainsBy(const auto& f, const T& element) const noexcept
    {
        return not IsUniqueElementBy(f, element);
    }

    bool Contains(const T& element) const noexcept
    {
        return not IsUniqueElement(element);
    }
}; // class Set

// Support declarations like: auto v{Set{1, 2, 3}}; // Equivalent to auto v{Set<int, 3>{1, 2, 3}};
template <typename... Args>
Set(Args...) -> Set<std::common_type_t<Args...>, sizeof...(Args)>;

} // namespace cljonic

#endif // CLJONIC_SET_HPP
