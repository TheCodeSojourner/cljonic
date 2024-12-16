#ifndef CLJONIC_ARRAY_HPP
#define CLJONIC_ARRAY_HPP

#include <type_traits>
#include "cljonic-collection-type.hpp"

namespace cljonic
{
/** \anchor Range
 * The \b Range type is a fundamental collection type in cljonic.  It is implemented as a \b lazy \b sequence of
 * \b "int". Many \ref Namespace_Core "Core" functions accept Range arguments.
 */
class Range
{
    class Iterator
    {
        const Range& m_range;
        std::size_t m_index;

      public:
        Iterator(const Range& range, const std::size_t index) : m_range(range), m_index(index)
        {
        }

        int operator*() const
        {
            return m_range[m_index];
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

    std::size_t m_elementCount;
    int m_elementDefault;
    int m_elementStart;
    int m_elementEnd;
    int m_elementStep;

    constexpr void InitializeMembers(const int count, const int start, const int end, const int step) noexcept
    {
        m_elementCount = static_cast<std::size_t>(count);
        m_elementDefault = 0;
        m_elementStart = start;
        m_elementEnd = end;
        m_elementStep = step;
    }

    constexpr void Initialize() noexcept
    {
        InitializeMembers(std::numeric_limits<int>::max(), 0, std::numeric_limits<int>::max(), 1);
    }

    constexpr void InitializeEnd(const int end) noexcept
    {
        if (end <= 0)
            InitializeMembers(0, 0, 0, 1);
        else
            InitializeMembers(end, 0, (end - 1), 1);
    }

    constexpr void InitializeStartEnd(const int start, const int end) noexcept
    {
        if (end <= start)
            InitializeMembers(0, 0, 0, 1);
        else
            InitializeMembers((end - start), start, (end - 1), 1);
    }

    constexpr void InitializeStartEndStepWithNegativeStep(const int start, const int end, const int step) noexcept
    {
        if (start <= end)
            InitializeMembers(0, 0, 0, step);
        else
        {
            const int count{(start - end) / -step};
            InitializeMembers(count, start, (start - (count * step)), step);
        }
    }

    constexpr void InitializeStartEndStepWithPositiveStep(const int start, const int end, const int step) noexcept
    {
        if (end <= start)
            InitializeMembers(0, 0, 0, step);
        else
        {
            const int count{((end - start) / step) + 1};
            InitializeMembers(count, start, (start + (count * step)), step);
        }
    }

    constexpr void InitializeStartEndStep(const int start, const int end, const int step) noexcept
    {
        if ((0 == step) and (start == end))
            InitializeMembers(0, 0, 0, 0);
        else if (0 == step)
            InitializeMembers(std::numeric_limits<int>::max(), start, start, 0);
        else if (step < 0)
            InitializeStartEndStepWithNegativeStep(start, end, step);
        else
            InitializeStartEndStepWithPositiveStep(start, end, step);
    }

  public:
    /**
    * There are four \b Range constructors:
    *     <b>Range()</b> returns a \b Range from \b 0 (zero) to \b std::numeric_limits<int>::max()
    *     <b>Range(end)</b> returns a \b Range from \b 0 (zero) to \b end-1
    *     <b>Range(start, end)</b> returns a \b Range from \b start to \b end-1
    *     <b>Range(start, end, step)</b> returns the \b Range \b start,  \b start+1*step, \b start+2*step, etc., while
    *     \b start+N*step < \b end.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto r0{Range()};             // r0 is immutable and goes from 0 to std::numeric_limits<int>::max() by 1
        const auto r1{Range(0)};            // r1 is immutable and is empty
        const auto r2{Range(-10)};          // r2 is immutable and is empty
        const auto r3{Range(10)};           // r3 is immutable and goes from 0 to 9 by 1
        const auto r4{Range(-3, -10)};      // r4 is immutable and is empty
        const auto r5{Range(-3, -3)};       // r5 is immutable and is empty
        const auto r6{Range(-3, 10)};       // r6 is immutable and goes from -3 to 9 by 1
        const auto r7{Range(10, 10, 0)};    // r7 is immutable and is empty
        const auto r8{Range(10, 20, 0)};    // r8 is immutable and has std::numeric_limits<int>::max() of 10
        const auto r9{Range(20, 10, 0)};    // r9 is immutable and has std::numeric_limits<int>::max() of 10
        const auto r10{Range(3, 10, 4)};    // r10 is immutable and has two elements: 3 and 7
        const auto r11{Range(3, 10, 3)};    // r11 is immutable and has three elements: 3, 6 and 9
        const auto r12{Range(100, 0, -10)}; // r12 is immutable and has ten elements: 100, 90, 80, ... and 10
        const auto r13{Range(10, -10, -1)}; // r13 is immutable and has twenty elements: 10, 9, ..., 0, -1, ... -9

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection = std::true_type;
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Range>;
    using size_type = std::size_t;
    using value_type = int;

    Range() noexcept
        : m_elementCount{static_cast<std::size_t>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementEnd{0},
          m_elementStep{0}
    {
        Initialize();
    }

    explicit Range(const int end) noexcept
        : m_elementCount{static_cast<std::size_t>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementEnd{0},
          m_elementStep{0}
    {
        InitializeEnd(end);
    }

    explicit Range(const int start, const int end) noexcept
        : m_elementCount{static_cast<std::size_t>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementEnd{0},
          m_elementStep{0}
    {
        InitializeStartEnd(start, end);
    }

    explicit Range(const int start, const int end, const int step) noexcept
        : m_elementCount{static_cast<std::size_t>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementEnd{0},
          m_elementStep{0}
    {
        InitializeStartEndStep(start, end, step);
    }

    Range(const Range& other) = default; // Copy constructor
    Range(Range&& other) = default;      // Move constructor

    [[nodiscard]] Iterator begin() const
    {
        return Iterator{*this, 0};
    }

    [[nodiscard]] Iterator end() const
    {
        return Iterator{*this, m_elementCount};
    }

    int operator[](const size_type index) const noexcept
    {
        return ((0 == m_elementCount) or (index >= m_elementCount))
                   ? m_elementDefault
                   : (m_elementStart + (static_cast<int>(index) * m_elementStep));
    }

    [[nodiscard]] size_type Count() const noexcept
    {
        return m_elementCount;
    }
}; // class Range

} // namespace cljonic

#endif // CLJONIC_ARRAY_HPP