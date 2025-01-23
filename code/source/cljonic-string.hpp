#ifndef CLJONIC_STRING_HPP
#define CLJONIC_STRING_HPP

#include <cstring>
#include <initializer_list>
#include <type_traits>
#include "cljonic-collection-iterator.hpp"
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{
/** \anchor String
 * The \b String type is a fundamental immutable collection type in cljonic.  It is implemented as a C array of char,
 * and <b>does not use dynamic memory</b>. A \b String has a specified maximum number of \b ordered char elements, which
 * are always \b NUL terminated. A \b String is a function of its indexable elements. A \b String called with an
 * out-of-bounds index will return its \b default \b element (i.e., a NUL char). Many \ref Namespace_Core "Core"
 * functions accept String arguments.
 */
template <SizeType MaxElements>
class String : public IndexInterface<char>
{
    using Iterator = CollectionIterator<String>;

    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create a String bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT");

    SizeType m_elementCount;
    const char m_elementDefault;
    char m_elements[maximumElements + 1]{}; // +1 for the null terminator

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

        // Compiler Error: Attempt to create a String bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT
        // const auto s{String<1111>{"Too Big"}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::String>;
    using size_type = SizeType;
    using value_type = char;

    constexpr String() noexcept : m_elementCount(0), m_elementDefault('\0')
    {
        m_elements[0] = '\0';
    }

    constexpr explicit String(const std::initializer_list<const char> elements) noexcept
        : m_elementCount(0), m_elementDefault('\0')
    {
        for (const auto& element : elements)
        {
            if (m_elementCount == MaximumCount())
                break; // LCOV_EXCL_LINE - This line of code may only execute at compile-time
            m_elements[m_elementCount++] = element;
        }
        m_elements[m_elementCount] = '\0';
    }

    constexpr explicit String(const char* c_str) noexcept : m_elementCount(0), m_elementDefault('\0')
    {
        while ((m_elementCount < MaximumCount()) and ('\0' != c_str[m_elementCount]))
        {
            m_elements[m_elementCount] = c_str[m_elementCount];
            m_elementCount += 1;
        }
        m_elements[m_elementCount] = '\0';
    }

    constexpr String(const String& other) = default; // Copy constructor
    constexpr String(String&& other) = default;      // Move constructor

    [[nodiscard]] constexpr Iterator begin() const noexcept
    {
        return Iterator{*this, 0};
    }

    [[nodiscard]] constexpr Iterator end() const noexcept
    {
        return Iterator{*this, m_elementCount};
    }

    constexpr char operator[](const SizeType index) const noexcept override
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

    constexpr char operator()(const SizeType index) const noexcept
    {
        return this->operator[](index);
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept override
    {
        return m_elementCount;
    }

    constexpr char DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    static constexpr SizeType MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class String

// Support declarations like 'auto v{String{"Hello"}};', which is equivalent to 'auto v{String<5>{"Hello"}};'
template <SizeType N>
String(const char (&)[N]) -> String<N - 1>;

// Support declarations like "auto v{String{'H', 'e', 'l', 'l', 'o'}};", which is equivalent to
// "auto v{String<5>{'H', 'e', 'l', 'l', 'o'}};"
template <typename... Args>
String(Args...) -> String<sizeof...(Args)>;

} // namespace cljonic

#endif // CLJONIC_STRING_HPP
