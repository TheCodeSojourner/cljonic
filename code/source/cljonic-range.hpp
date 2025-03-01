#ifndef CLJONIC_RANGE_HPP
#define CLJONIC_RANGE_HPP

#include <concepts>
#include "cljonic-collection-iterator.hpp"
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

/** \anchor Range
 * The \b Range type is a fundamental immutable collection type in cljonic.  It is implemented as a \b lazy \b sequence
 * of \b "int". Many \ref Namespace_Core "Core" functions accept Range arguments.
 */
template <int... StartEndStep>
class Range : public IndexInterface<int>
{
  private:
    static_assert(sizeof...(StartEndStep) <= 3, "Number of Range parameters must be less than or equal to three");

    using Iterator = CollectionIterator<Range>;

    SizeType m_elementCount;
    int m_elementDefault;
    int m_elementStart;
    int m_elementStep;

    static constexpr auto RangeCount(const int start, const int end, const int step) noexcept
    {
        return (0 == step) ? 0 : ((end - start) / step) + ((((end - start) % step) == 0) ? 0 : 1);
    }

    static constexpr int values[] = {StartEndStep...};

    static constexpr auto MaxElements = []() constexpr
    {
        if constexpr (sizeof...(StartEndStep) == 1) // Range<end>{}
        {
            if constexpr (values[0] < 0) // end is negative
                return static_cast<SizeType>(0);
            else
                return static_cast<SizeType>(values[0]);
        }
        else if constexpr (sizeof...(StartEndStep) == 2) // Range<start, end>{}
        {
            if constexpr (values[1] <= values[0]) // end is less than or equal to start
                return static_cast<SizeType>(0);
            else
                return static_cast<SizeType>(values[1] - values[0]);
        }
        else if constexpr (sizeof...(StartEndStep) == 3) // Range<start, end, step>{}
        {
            // #lizard forgives -- The length and complexity of this function is acceptable
            if constexpr ((0 == values[2]) and (values[0] == values[1])) // step is zero, and start equals end
                return static_cast<SizeType>(0);
            else if constexpr (0 == values[2]) // step is zero
                return CljonicCollectionMaximumElementCount;
            else if constexpr (values[2] < 0) // step is negative
            {
                if constexpr (values[0] <= values[1]) // start is less than or equal to end
                    return static_cast<SizeType>(0);
                else
                    return static_cast<SizeType>(RangeCount(values[0], values[1], values[2]));
            }
            else
            {
                if constexpr (values[1] <= values[0]) // end is less than or equal to start
                    return static_cast<SizeType>(0);
                else
                    return static_cast<SizeType>(RangeCount(values[0], values[1], values[2]));
            }
        }
        else // Range{}
        {
            return CljonicCollectionMaximumElementCount;
        }
    }();

    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create a Range bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT");

    constexpr void InitializeMembers(const int count, const int start, const int step) noexcept
    {
        m_elementCount = MinArgument(static_cast<SizeType>(count), CljonicCollectionMaximumElementCount);
        m_elementDefault = 0;
        m_elementStart = start;
        m_elementStep = step;
    }

    constexpr void Initialize() noexcept
    {
        InitializeMembers(CljonicCollectionMaximumElementCount, 0, 1);
    }

    constexpr void InitializeEnd(const int end) noexcept
    {
        if (end <= 0)
            InitializeMembers(0, 0, 1);
        else
            InitializeMembers(end, 0, 1);
    }

    constexpr void InitializeStartEnd(const int start, const int end) noexcept
    {
        if (end <= start)
            InitializeMembers(0, 0, 1); // LCOV_EXCL_LINE - This line of code may only execute at compile-time
        else
            InitializeMembers((end - start), start, 1);
    }

    constexpr void InitializeStartEndStepWithNegativeStep(const int start, const int end, const int step) noexcept
    {
        if (start <= end)
            InitializeMembers(0, 0, step); // LCOV_EXCL_LINE - This line of code may only execute at compile-time
        else
        {
            const int count{RangeCount(start, end, step)};
            InitializeMembers(count, start, step);
        }
    }

    constexpr void InitializeStartEndStepWithPositiveStep(const int start, const int end, const int step) noexcept
    {
        if (end <= start)
            InitializeMembers(0, 0, step); // LCOV_EXCL_LINE - This line of code may only execute at compile-time
        else
        {
            const int count{RangeCount(start, end, step)};
            InitializeMembers(count, start, step);
        }
    }

    constexpr void InitializeStartEndStep(const int start, const int end, const int step) noexcept
    {
        // #lizard forgives -- The complexity of this function is acceptable
        if ((0 == step) and (start == end))
            InitializeMembers(0, 0, 0);
        else if (0 == step)
            InitializeMembers(CljonicCollectionMaximumElementCount, start, 0);
        else if (step < 0)
            InitializeStartEndStepWithNegativeStep(start, end, step);
        else
            InitializeStartEndStepWithPositiveStep(start, end, step);
    }

    constexpr auto ValueAtIndex(const SizeType index) const noexcept
    {
        return ((0 == m_elementCount) or (index >= m_elementCount))
                   ? m_elementDefault
                   : (m_elementStart + (static_cast<int>(index) * m_elementStep));
    }

  public:
    /**
    * There are four ways to create a \b Range:
    *     - <b>Range{}}</b> returns a \b Range from \b 0 (zero) to \b std::numeric_limits<int>::max() by one
    *     - <b>Range<end>{}</b> returns a \b Range from \b 0 (zero) to \b end-1 by one
    *     - <b>Range<start, end>{}</b> returns a \b Range from \b start to \b end-1 by one
    *     - <b>Range<start, end, step>{}</b> returns the \b Range \b start,  \b start+1*step, \b start+2*step, etc.,
    *       while \b start+N*step is less than \b end.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto r0{Range{}};               // immutable, goes from 0 to std::numeric_limits<int>::max() by 1
        const auto r1{Range<0>{}};            // immutable, empty
        const auto r2{Range<-10>{}};          // immutable, empty
        const auto r3{Range<10>{}};           // immutable, goes from 0 to 9 by 1
        const auto r4{Range<-3, -10>{}};      // immutable, empty
        const auto r5{Range<-3, -3>{}};       // immutable, empty
        const auto r6{Range<-3, 10>{}};       // immutable, goes from -3 to 9 by 1
        const auto r7{Range<10, 10, 0>{}};    // immutable, empty
        const auto r8{Range<10, 20, 0>{}};    // immutable, has std::numeric_limits<int>::max() of 10
        const auto r9{Range<20, 10, 0>{}};    // immutable, has std::numeric_limits<int>::max() of 20
        const auto r10{Range<3, 10, 4>{}};    // immutable, has two elements: 3 and 7
        const auto r11{Range<3, 10, 3>{}};    // immutable, has three elements: 3, 6 and 9
        const auto r12{Range<100, 0, -10>{}}; // immutable, has ten elements: 100, 90, 80, ... and 10
        const auto r13{Range<10, -10, -1>{}}; // immutable, has twenty elements: 10, 9, ..., 0, -1, ... -9

        // Compiler Error: Number of Range parameters must be less than or equal to three
        // const auto r{Range<10, -10, -1, 1>{}};

        // Compiler Error: Attempt to create a Range bigger than CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT
        // const auto r{Range<-11111, 11111>{}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Range>;
    using size_type = SizeType;
    using value_type = int;

    constexpr Range() noexcept
        : m_elementCount{static_cast<SizeType>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementStep{0}
    {
        // #lizard forgives -- The complexity and length of this function is acceptable
        if constexpr (sizeof...(StartEndStep) == 1) // Range<end>{}
        {
            InitializeEnd(values[0]);
        }
        else if constexpr (sizeof...(StartEndStep) == 2) // Range<start, end>{}
        {
            InitializeStartEnd(values[0], values[1]);
        }
        else if constexpr (sizeof...(StartEndStep) == 3) // Range<start, end, step>{}
        {
            InitializeStartEndStep(values[0], values[1], values[2]);
        }
        else // Range{}
        {
            Initialize();
        }
    }

    constexpr Range(const Range& other) = default; // Copy constructor
    constexpr Range(Range&& other) = default;      // Move constructor

    [[nodiscard]] constexpr Iterator begin() const noexcept
    {
        return Iterator{*this, 0};
    }

    [[nodiscard]] constexpr Iterator end() const noexcept
    {
        return Iterator{*this, m_elementCount};
    }

    constexpr int operator[](const SizeType index) const noexcept override
    {
        return ValueAtIndex(index);
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept override
    {
        return m_elementCount;
    }

    constexpr int DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    constexpr bool ElementAtIndexIsEqualToElement(const SizeType index, const int& element) const noexcept override
    {
        return (index < m_elementCount) and AreEqual(ValueAtIndex(index), element);
    }

    static constexpr auto MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class Range

// Deduction guide for Range{}
Range() -> Range<>;

} // namespace cljonic

#endif // CLJONIC_RANGE_HPP
