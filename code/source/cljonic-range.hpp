#ifndef CLJONIC_RANGE_HPP
#define CLJONIC_RANGE_HPP

#include <concepts>
#include "cljonic-collection-type.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

/** \anchor Range
 * The \b Range type is a fundamental \b cljonic \b collection type. It is implemented as a \b lazy \b sequence of
 * \b RangeType. Many \ref Namespace_Core "Core" functions accept Range arguments.
 */
template <RangeType... StartEndStep>
class Range
{
    static_assert(sizeof...(StartEndStep) <= 3, "Number of Range parameters must be less than or equal to three");

    static constexpr RangeType values[] = {StartEndStep...};

    [[nodiscard]] static constexpr RangeType RangeStart() noexcept
    {
        return (sizeof...(StartEndStep) < 2) ? 0 : values[0];
    }

    [[nodiscard]] static constexpr RangeType RangeStep() noexcept
    {
        return (sizeof...(StartEndStep) < 3) ? 1 : values[2];
    }

    [[nodiscard]] static constexpr bool
    IsEmptyRange(RangeType elementEnd, RangeType elementStart, RangeType elementStep) noexcept
    {
        return ((elementStep == 0) and (elementStart == elementEnd))   //
               or ((elementStep > 0) and (elementStart >= elementEnd)) //
               or ((elementStep < 0) and (elementStart <= elementEnd));
    }

    static constexpr bool IsRepeatRange(RangeType elementEnd, RangeType elementStart, RangeType elementStep) noexcept
    {
        return ((elementStep == 0) and (elementStart != elementEnd));
    }

    [[nodiscard]] static constexpr SizeType
    RangeCount(RangeType elementEnd, RangeType elementStart, RangeType elementStep) noexcept
    {
        // #lizard forgives -- The length and complexity of this function is acceptable

        return IsEmptyRange(elementEnd, elementStart, elementStep) //
                   ? 0
                   : IsRepeatRange(elementEnd, elementStart, elementStep)
                         ? CljonicCollectionMaximumElementCount
                         : static_cast<SizeType>(((elementEnd - elementStart) / elementStep) +
                                                 ((((elementEnd - elementStart) % elementStep) == 0) ? 0 : 1));
    }

    static constexpr auto MaxElements = []() constexpr
    {
        if constexpr (sizeof...(StartEndStep) == 1)
        {
            return RangeCount(values[0], 0, 1); // Range<end>{}
        }
        else if constexpr (sizeof...(StartEndStep) == 2)
        {
            return RangeCount(values[1], values[0], 1); // Range<start, end>{}
        }
        else if constexpr (sizeof...(StartEndStep) == 3)
        {
            return RangeCount(values[1], values[0], values[2]); // Range<start, end, step>{}
        }
        else
        {
            return RangeCount(static_cast<RangeType>(CljonicCollectionMaximumElementCount), 0, 1); // Range{}
        }
    }();

    static constexpr SizeType maximumElements{MaximumElements(MaxElements)};

    static_assert(maximumElements == MaxElements,
                  "Attempt to create a Range bigger than CljonicCollectionMaximumElementCount");

    static constexpr SizeType m_elementCount{maximumElements};
    static constexpr RangeType m_elementDefault{};
    static constexpr RangeType m_elementStart{RangeStart()};
    static constexpr RangeType m_elementStep{RangeStep()};

    class RangeIterator final
    {
        SizeType m_count;
        RangeType m_current;
        const RangeType m_step;

      public:
        using value_type = RangeType;

        constexpr RangeIterator(const SizeType count, const RangeType current, const RangeType step) noexcept
            : m_count{count}, m_current{current}, m_step{step}
        {
        }

        [[nodiscard]] constexpr RangeIterator(const RangeIterator& other) noexcept = default; // Copy constructor

        [[nodiscard]] constexpr const RangeType& operator*() const noexcept
        {
            return m_current;
        }

        constexpr RangeIterator& operator++() noexcept
        {
            m_count += 1;
            m_current += m_step;
            return *this;
        }

        [[nodiscard]] constexpr bool operator!=(const RangeIterator& other) const noexcept
        {
            return m_count != other.m_count;
        }

        constexpr RangeIterator& operator=(const RangeIterator& other) noexcept
        {
            if (this != &other)
            {
                m_count = other.m_count;
                m_current = other.m_current;
            }
            return *this;
        }
    };

  public:
    /**
    * There are four ways to create a \b Range:
    *     - <b>Range{}}</b> returns a \b Range from \b 0 (zero) to \b CljonicCollectionMaximumElementCount-1 by one
    *     - <b>Range<end>{}</b> returns a \b Range from \b 0 (zero) to \b end-1 by one
    *     - <b>Range<start, end>{}</b> returns a \b Range from \b start to \b end-1 by one
    *     - <b>Range<start, end, step>{}</b> returns the \b Range \b start,  \b start+(1*step), \b start+(2*step), etc.,
    *       while \b start+(N*step) is less than \b end.
    ~~~~~{.cpp}
    #include "cljonic.hpp"

    using namespace cljonic;

    int main()
    {
        const auto r0{Range{}};               // goes from 0 to CljonicCollectionMaximumElementCount-1 by 1
        const auto r1{Range<0>{}};            // empty
        const auto r2{Range<-10>{}};          // empty
        const auto r3{Range<10>{}};           // goes from 0 to 9 by 1
        const auto r4{Range<-3, -10>{}};      // empty
        const auto r5{Range<-3, -3>{}};       // empty
        const auto r6{Range<-3, 10>{}};       // goes from -3 to 9 by 1
        const auto r7{Range<10, 10, 0>{}};    // empty
        const auto r8{Range<10, 20, 0>{}};    // has CljonicCollectionMaximumElementCount of 10
        const auto r9{Range<20, 10, 0>{}};    // has CljonicCollectionMaximumElementCount of 20
        const auto r10{Range<3, 10, 4>{}};    // has two elements: 3 and 7
        const auto r11{Range<3, 10, 3>{}};    // has three elements: 3, 6 and 9
        const auto r12{Range<100, 0, -10>{}}; // has ten elements: 100, 90, 80, ... and 10
        const auto r13{Range<10, -10, -1>{}}; // has twenty elements: 10, 9, ..., 0, -1, ... -9

        // Compiler Error: Number of Range parameters must be less than or equal to three
        // const auto r{Range<10, -10, -1, 1>{}};

        // Compiler Error: Attempt to create a Range bigger than CljonicCollectionMaximumElementCount
        // const auto r{Range<-11111, 11111>{}};

        return 0;
    }
    ~~~~~
    */
    using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Range>;
    using size_type = SizeType;
    using value_type = int;

    constexpr Range() noexcept = default;                   // Default constructor
    constexpr Range(const Range& other) noexcept = default; // Copy constructor
    constexpr Range(Range&& other) noexcept = default;      // Move constructor

    [[nodiscard]] constexpr bool operator==(const auto& other) const noexcept
    {
        return AreEqualValues(this, other);
    }

    [[nodiscard]] constexpr RangeIterator begin() const noexcept
    {
        return RangeIterator{0, m_elementStart, m_elementStep};
    }

    [[nodiscard]] constexpr RangeIterator end() const noexcept
    {
        return RangeIterator{Count(), 0, 0};
    }

    [[nodiscard]] constexpr SizeType Count() const noexcept
    {
        return m_elementCount;
    }

    [[nodiscard]] constexpr const RangeType& DefaultElement() const noexcept
    {
        return m_elementDefault;
    }

    [[nodiscard]] static constexpr auto MaximumCount() noexcept
    {
        return maximumElements;
    }
}; // class Range

// Deduction guide for Range{}
Range() -> Range<>;

} // namespace cljonic

#endif // CLJONIC_RANGE_HPP