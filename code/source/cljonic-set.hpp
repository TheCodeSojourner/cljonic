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
 * The \b Set type is a fundamental collection type in cljonic.  It is implemented as a C array, and <b>does not
 * use dynamic memory</b>. A \b Set has a specified maximum number of \b unique \b unordered elements each of the same
 * specified type (i.e., It is homogenous).  <b>Note that one could create a Set of a \b UNION or \b std::variant to get
 * something like heterogeneity.</b>  Many \ref Namespace_Core "Core" functions accept Set arguments.
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
    T m_elementDefault;
    T m_elements[MaxElements];

    void InitializeElementDefault() noexcept
    {
        m_elementDefault = T{};
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
        auto s0{Set<int, 10>{}};                // s0 is empty
        auto s1{Set<int, 10>{1, 2, 3, 4}};      // s1 is sparse
        auto s2{Set<int, 4>{1, 2, 3, 4}};       // s2 is full
        auto s3{Set<int, 4>{1, 2, 3, 4, 5, 6}}; // s3 is full, but 5 and 6 are ignored
        auto s4{Set<int, 4>{1, 2, 1, 4, 5, 6}}; // s4 is full with 1, 2, 4, 5
        auto s5{Set{1, 2, 3, 4}};               // s5 is full of four int values
        auto s6{Set{1, 2, 1, 4}};               // s6 is full of three unique int values

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection = std::true_type;
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Set>;
    using value_type = T;

    Set() : m_elementCount(0)
    {
        InitializeElementDefault();
    }

    Set(const std::initializer_list<const T> elements) : m_elementCount(0)
    {
        InitializeElementDefault();
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

    const T& operator[](const MaxElementsType index) const noexcept
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

    [[nodiscard]] MaxElementsType Count() const noexcept
    {
        return m_elementCount;
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
