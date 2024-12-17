#ifndef CLJONIC_STRING_HPP
#define CLJONIC_STRING_HPP

#include <cstring>
#include <initializer_list>
#include <type_traits>
#include "cljonic-collection-type.hpp"

namespace cljonic
{
/** \anchor String
 * The \b String type is a fundamental immutable collection type in cljonic.  It is implemented as a C array of char,
 * and <b>does not use dynamic memory</b>. A \b String has a specified maximum number of \b ordered char elements, which
 * are always \b NUL terminated. Many \ref Namespace_Core "Core" functions accept String arguments.
 */
template <std::size_t MaxElements>
class String
{
    using MaxElementsType = decltype(MaxElements);

    MaxElementsType m_elementCount;
    char m_elementDefault;
    char m_elements[MaxElements + 1]; // +1 for the null terminator

    class Iterator
    {
        const String& m_string;
        std::size_t m_index;

      public:
        Iterator(const String& string, const std::size_t index) : m_string(string), m_index(index)
        {
        }

        char operator*() const
        {
            return m_string[m_index];
        }

        Iterator& operator++()
        {
            ++m_index;
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return m_index != other.m_index;
        }
    };

  public:
    /**
    * The \b String constructor returns an instance of String initialized with the chars in its argument(s). If the
    * number of chars in its argument(s) exceeds the maximum number of elements, the extras are silently ignored.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto s0{String<10>{}};                       // immutable, empty
        const auto s1{String<10>{"Hello"}};                // immutable, sparse
        const auto s2{String<5>{"Hello"}};                 // immutable, full
        const auto s3{String<5>{"Hello, World"}};          // immutable, full, and contains "Hello"
        const auto s4{String<3>{'H', 'e', 'l', 'l', 'o'}}; // immutable, sparse
        const auto s5{String{"Hello"}};                    // immutable, full
        const auto s6{String{'H', 'e', 'l', 'l', 'o'}};    // immutable, full

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection = std::true_type;
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::String>;
    using size_type = MaxElementsType;
    using value_type = char;

    String() noexcept : m_elementCount(0), m_elementDefault('\0')
    {
        m_elements[0] = '\0';
    }

    explicit String(const std::initializer_list<const char> elements) noexcept
        : m_elementCount(0), m_elementDefault('\0')
    {
        for (const auto& element : elements)
        {
            if (m_elementCount == MaxElements)
                break;
            m_elements[m_elementCount++] = element;
        }
        m_elements[m_elementCount] = '\0';
    }

    explicit String(const char* c_str) noexcept : m_elementCount(0), m_elementDefault('\0')
    {
        while ((m_elementCount < MaxElements) and ('\0' != c_str[m_elementCount]))
        {
            m_elements[m_elementCount] = c_str[m_elementCount];
            m_elementCount += 1;
        }
        m_elements[m_elementCount] = '\0';
    }

    String(const String& other) = default; // Copy constructor
    String(String&& other) = default;      // Move constructor

    [[nodiscard]] Iterator begin() const
    {
        return Iterator{*this, 0};
    }

    [[nodiscard]] Iterator end() const
    {
        return Iterator{*this, m_elementCount};
    }

    char operator[](const MaxElementsType index) const noexcept
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

    [[nodiscard]] MaxElementsType Count() const noexcept
    {
        return m_elementCount;
    }
}; // class String

// Support declarations like 'auto v{String{"Hello"}};', which is equivalent to 'auto v{String<5>{"Hello"}};'
template <std::size_t N>
String(const char (&)[N]) -> String<N - 1>;

// Support declarations like "auto v{String{'H', 'e', 'l', 'l', 'o'}};", which is equivalent to
// "auto v{String<5>{'H', 'e', 'l', 'l', 'o'}};"
template <typename... Args>
String(Args...) -> String<sizeof...(Args)>;

} // namespace cljonic

#endif // CLJONIC_STRING_HPP
