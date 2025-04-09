#ifndef CLJONIC_STRING_HPP
#define CLJONIC_STRING_HPP

#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <string_view>
#include <type_traits>
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{
/** \anchor String
 * The \b String type is a fundamental \b cljonic \b collection type.  It is implemented as a C array of \b CharType,
 * and <b>does not use dynamic memory</b>. A \b String has a specified maximum number of \b ordered character elements,
 * which are always \b NUL terminated. A \b String is a function of its indexable elements. A \b String called with an
 * out-of-bounds index will return its \b default \b element (i.e., a NUL). Many \ref Namespace_Core "Core" functions
 * accept String arguments.
 */
template <SizeType MaxElements>
class String
{
    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create a String bigger than CljonicCollectionMaximumElementCount");

    SizeType m_elementCount{0};
    static constexpr CharType m_elementDefault{'\0'};
    std::array<CharType, (maximumElements + 1)> m_elements{}; // +1 for null termination

    class StringIterator final
    {
        const CharType* m_elementsPointer;

      public:
        using value_type = CharType;

        constexpr explicit StringIterator(const CharType* elementsPointer) noexcept : m_elementsPointer{elementsPointer}
        {
        }

        [[nodiscard]] constexpr const CharType& operator*() const noexcept
        {
            return *m_elementsPointer;
        }

        constexpr StringIterator& operator++() noexcept
        {
            ++m_elementsPointer;
            return *this;
        }

        [[nodiscard]] constexpr bool operator!=(const StringIterator& other) const noexcept
        {
            return m_elementsPointer != other.m_elementsPointer;
        }
    };

  public:
    /**
    * The \b String constructor returns an instance of String initialized with the characters in its argument(s). If the
    * number of characters in its argument(s) exceeds the String's maximum allowed number of elements, the extra
    * characters are \b silently \b ignored.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto s0{String<10>{}};                       // empty
        const auto s1{String<10>{"Hello"}};                // sparse
        const auto s2{String<5>{"Hello"}};                 // full
        const auto s3{String<5>{"Hello, World"}};          // full, and contains "Hello"
        const auto s4{String<3>{'H', 'e', 'l', 'l', 'o'}}; // sparse
        const auto s5{String{"Hello"}};                    // full
        const auto s6{String{'H', 'e', 'l', 'l', 'o'}};    // full

        // Compiler Error: Attempt to create a String bigger than CljonicCollectionMaximumElementCount
        // const auto s{String<1111>{"Too Big"}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::String>;
    using size_type = SizeType;
    using value_type = CharType;

    constexpr String() noexcept : m_elementCount(0)
    {
        m_elements[0] = '\0';
    }

    constexpr String(const std::initializer_list<CharType>& init) noexcept
        : m_elementCount{std::min(init.size(), maximumElements)}
    {
        std::copy(init.begin(), {init.begin() + m_elementCount}, m_elements.begin());
        m_elements[m_elementCount] = '\0';
    }

    constexpr explicit String(const CharType* str) noexcept
        : m_elementCount{std::min(std::strlen(str), maximumElements)}
    {
        auto strView{std::string_view{str}};
        std::copy(strView.begin(), {strView.begin() + m_elementCount}, m_elements.begin());
        m_elements[m_elementCount] = '\0';
    }

    constexpr String(const String& other) noexcept = default; // Copy constructor
    constexpr String(String&& other) noexcept = default;      // Move constructor

    [[nodiscard]] constexpr bool operator==(const auto& other) const noexcept
    {
        return AreEqualValues(this, other);
    }

    [[nodiscard]] constexpr StringIterator begin() noexcept
    {
        return StringIterator{m_elements.data()};
    }

    [[nodiscard]] constexpr StringIterator end() noexcept
    {
        return StringIterator{m_elements.data() + m_elementCount};
    }

    [[nodiscard]] constexpr StringIterator begin() const noexcept
    {
        return StringIterator{m_elements.data()};
    }

    [[nodiscard]] constexpr StringIterator end() const noexcept
    {
        return StringIterator{m_elements.data() + m_elementCount};
    }

    [[nodiscard]] constexpr CharType operator()(const SizeType index) const noexcept
    {
        return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
    }

    [[nodiscard]] constexpr const CharType* c_str() const noexcept
    {
        return m_elements.data();
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept
    {
        return m_elementCount;
    }

    [[nodiscard]] constexpr CharType DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    [[nodiscard]] static constexpr SizeType MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class String

// Support declarations like 'auto v{String{"Hello"}};', which is equivalent to 'auto v{String<5>{"Hello"}};'
template <SizeType N>
String(const CharType (&)[N]) -> String<N - 1>;

// Support declarations like "auto v{String{'H', 'e', 'l', 'l', 'o'}};", which is equivalent to
// "auto v{String<5>{'H', 'e', 'l', 'l', 'o'}};"
template <typename... Args>
String(Args...) -> String<sizeof...(Args)>;

} // namespace cljonic

#endif // CLJONIC_STRING_HPP
