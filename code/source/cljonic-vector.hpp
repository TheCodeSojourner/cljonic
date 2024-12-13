#ifndef CLJONIC_VECTOR_HPP
#define CLJONIC_VECTOR_HPP

#include <cstring>
#include <initializer_list>
#include <type_traits>
#include "cljonic-collection-type.hpp"

namespace cljonic
{
/** \anchor Vector
 * The \b Vector type is a fundamental collection type in cljonic.  It is implemented as a C array, and <b>does
 * not use heap memory</b>.  A \b Vector has a specified maximum number of \b ordered elements each of the same
 * specified type (i.e., It is homogenous).  <b>Note that one could create a Vector of a \b UNION or \b std::variant to
 * get something like heterogeneity.</b>  Many \ref Namespace_Core "Core" functions accept Vector arguments.
 */
template <typename T, std::size_t MaxElements>
class Vector
{
    using MaxElementsType = decltype(MaxElements);

    MaxElementsType m_elementCount;
    T m_elementDefault;
    T m_elements[MaxElements];

    void InitializeElementDefault() noexcept
    {
        m_elementDefault = T{};
    }

  public:
    /** \anchor Vector
    * The \b Vector constructor returns an instance of Vector initialized with its arguments. If the number of arguments
    * exceeds the maximum number of elements specified, the extra arguments are silently ignored.
    ~~~~~{.cpp}
    #include <iostream>
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
    auto v0{Vector<int, 10>{}};                // v0 is empty
    auto v1{Vector<int, 10>{1, 2, 3, 4}};      // v1 is sparse
    auto v2{Vector<int, 4>{1, 2, 3, 4}};       // v2 is full
    auto v3{Vector<int, 4>{1, 2, 3, 4, 5, 6}}; // v3 is full, but the values 5 and 6 are ignored
    auto v4{Vector{1, 2, 3, 4}};               // v4 is full of four int values

    return 0;
    }
    ~~~~~
    */
    using cljonic_collection = std::true_type;
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Vector>;
    using value_type = T;

    Vector() : m_elementCount(0)
    {
        InitializeElementDefault();
    }

    Vector(const std::initializer_list<const T> elements) : m_elementCount(0)
    {
        InitializeElementDefault();
        for (const auto& element : elements)
        {
            if (m_elementCount == MaxElements)
                break;
            m_elements[m_elementCount++] = element;
        }
    }

    Vector(const Vector& other) = default; // Copy constructor
    Vector(Vector&& other) = default;      // Move constructor

    const T& operator[](const MaxElementsType index) const noexcept
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

    [[nodiscard]] MaxElementsType Count() const noexcept
    {
        return m_elementCount;
    }
}; // class Vector

// Support declarations like: auto v{Vector{1, 2, 3}}; // Equivalent to auto v{Vector<int, 3>{1, 2, 3}};
template <typename... Args>
Vector(Args...) -> Vector<std::common_type_t<Args...>, sizeof...(Args)>;

} // namespace cljonic

#endif // CLJONIC_VECTOR_HPP
