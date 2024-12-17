#ifndef CLJONIC_ARRAY_HPP
#define CLJONIC_ARRAY_HPP

#include <cstring>
#include <initializer_list>
#include <type_traits>
#include "cljonic-collection-type.hpp"

namespace cljonic
{
/** \anchor Array
 * The \b Array type is a fundamental immutable collection type in cljonic.  It is implemented as a C array, and
 * <b>does not use heap memory</b>.  An \b Array has a specified maximum number of \b ordered elements each  of the same
 * specified type (i.e., It is homogenous).  <b>Note that one could create an Array of a \b UNION or \b std::variant to
 * get something like heterogeneity.</b>  Many \ref Namespace_Core "Core" functions accept Array arguments.
 */
template <typename T, std::size_t MaxElements>
class Array
{
    using MaxElementsType = decltype(MaxElements);

    MaxElementsType m_elementCount;
    T m_elementDefault;
    T m_elements[MaxElements];

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

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection = std::true_type;
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Array>;
    using size_type = MaxElementsType;
    using value_type = T;

    Array() noexcept : m_elementCount(0), m_elementDefault(T{})
    {
    }

    explicit Array(const std::initializer_list<const T> elements) noexcept : m_elementCount(0), m_elementDefault(T{})
    {
        for (const auto& element : elements)
        {
            if (m_elementCount == MaxElements)
                break;
            m_elements[m_elementCount++] = element;
        }
    }

    Array(const Array& other) = default; // Copy constructor
    Array(Array&& other) = default;      // Move constructor

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
}; // class Array

// Support declarations like: auto v{Array{1, 2, 3}}; // Equivalent to auto v{Array<int, 3>{1, 2, 3}};
template <typename... Args>
Array(Args...) -> Array<std::common_type_t<Args...>, sizeof...(Args)>;

} // namespace cljonic

#endif // CLJONIC_ARRAY_HPP
