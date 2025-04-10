#ifndef CLJONIC_SHARED_HPP
#define CLJONIC_SHARED_HPP

#include <concepts>
#include <cstddef>
#include <limits>
#include <string_view>
#include "cljonic-concepts.hpp"

#ifndef TYPE_CLJONIC_CHAR
#define TYPE_CLJONIC_CHAR char
#endif

#ifndef TYPE_CLJONIC_RANGE
#define TYPE_CLJONIC_RANGE int
#endif

#ifndef CONSTANT_CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT
#define CONSTANT_CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT 1000
#endif

namespace cljonic
{

using CharType = TYPE_CLJONIC_CHAR;
using RangeType = TYPE_CLJONIC_RANGE;
using SizeType = std::size_t;

constexpr auto CljonicCollectionMaximumElementCount{
    static_cast<SizeType>(CONSTANT_CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT)};
constexpr auto CljonicInvalidIndex{std::numeric_limits<SizeType>::max()};

constexpr bool CStringsEqual(const char* str1, const char* str2) noexcept
{
    while (*str1 and (*str1 == *str2))
    {
        ++str1;
        ++str2;
    }
    return *str1 == *str2;
}

template <typename T, typename U>
constexpr bool AreEqualValues(const T& t, const U& u)
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    if constexpr (IsCString<T> and IsCString<U>)
    {
        return CStringsEqual(t, u);
    }
    else if constexpr (IsCString<T> and IsCljonicString<U>)
    {
        return CStringsEqual(t, u.c_str());
    }
    else if constexpr (IsCljonicString<T> and IsCString<U>)
    {
        return CStringsEqual(t.c_str(), u);
    }
    else if constexpr (IsCljonicString<T> and IsCljonicString<U>)
    {
        return CStringsEqual(t.c_str(), u.c_str());
    }
    else if constexpr ((IsCljonicCollection<T> and IsNotCljonicCollection<U>) or
                       (IsNotCljonicCollection<T> and IsCljonicCollection<U>))
    {
        return false;
    }
    else if constexpr (IsCljonicCollection<T> and IsCljonicCollection<U>)
    {
        if constexpr ((not IsCljonicSet<T>) and (not IsCljonicSet<U>))
        {
            static constexpr auto AreNonSetCollectionsEqual =
                []<typename FirstCollection, typename SecondCollection>(const SizeType firstCount,
                                                                        const SizeType secondCount,
                                                                        const FirstCollection& firstCollection,
                                                                        const SecondCollection& secondCollection)
            {
                if constexpr ((not EqualityComparableValueTypes<FirstCollection, SecondCollection>) //
                              or std::floating_point<typename FirstCollection::value_type>          //
                              or std::floating_point<typename SecondCollection::value_type>         //
                              or IsReferenceAndIntegralValueType<FirstCollection, SecondCollection>)
                {
                    return false;
                }
                else if constexpr (IsCljonicRepeat<FirstCollection> and IsCljonicRepeat<SecondCollection>)
                {
                    return (firstCount != secondCount) ? false
                                                       : (*firstCollection.begin() == *secondCollection.begin());
                }
                else
                {
                    if (firstCount != secondCount)
                        return false;
                    for ( // initialization
                        auto itFirst{firstCollection.begin()},
                        endFirst{firstCollection.end()},
                        itSecond{secondCollection.begin()},
                        endSecond{secondCollection.end()};
                        // condition
                        ((itFirst != endFirst) and (itSecond != endSecond));
                        // iteration
                        ++itFirst,
                        ++itSecond)
                    {
                        if (not AreEqualValues(*itFirst, *itSecond))
                            return false;
                    }
                    return true;
                }
            };

            if constexpr (IsCljonicArray<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicArray<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicArray<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicArray<T> and IsCljonicRepeat<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicArray<T> and IsCljonicString<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicRepeat<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicString<U>)
            {
                return AreNonSetCollectionsEqual(u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicRepeat<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicString<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicRepeat<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicString<U>)
            {
                return AreEqualValues(*t.begin(), u.c_str());
            }
            else if constexpr (IsCljonicString<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicString<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicString<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicString<T> and IsCljonicRepeat<U>)
            {
                return AreEqualValues(t.c_str(), *u.begin());
            }
            else
            {
                return false;
            }
        }
        else if constexpr (IsCljonicSet<T> and IsCljonicSet<U>)
        {
            if constexpr ((not EqualityComparableValueTypes<T, U>)       //
                          or std::floating_point<typename T::value_type> //
                          or std::floating_point<typename U::value_type> //
                          or IsReferenceAndIntegralValueType<T, U>)
            {
                return false;
            }
            else
            {
                if (t.Count() != u.Count())
                    return false; // LCOV_EXCL_LINE - This line of code may only execute at compile-time
                for (const auto& vt : t)
                    if (not u.Contains(vt))
                        return false; // LCOV_EXCL_LINE - This line of code may only execute at compile-time
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    else if constexpr ((not std::equality_comparable_with<T, U>) //
                       or std::floating_point<T>                 //
                       or std::floating_point<U>                 //
                       or IsReferenceAndIntegral<T, U>)
    {
        return false;
    }
    else
    {
        return t == u;
    }
}

template <typename F, typename T, typename U>
constexpr bool AreEqualValuesBy(F&& f, const T& t, const U& u)
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    if constexpr (IsCljonicCollection<T> and IsCljonicCollection<U>)
    {
        if constexpr (not IsBinaryPredicate<F, typename T::value_type, typename U::value_type>)
        {
            return false;
        }
        else if constexpr ((not IsCljonicSet<T>) and (not IsCljonicSet<U>))
        {
            static constexpr auto AreNonSetCollectionsEqual =
                []<typename FN, typename FirstCollection, typename SecondCollection>(
                    FN&& fn,
                    const SizeType firstCount,
                    const SizeType secondCount,
                    const FirstCollection& firstCollection,
                    const SecondCollection& secondCollection)
            {
                if constexpr (IsCljonicRepeat<FirstCollection> and IsCljonicRepeat<SecondCollection>)
                {
                    return std::forward<FN>(fn)(*firstCollection.begin(), *secondCollection.begin());
                }
                else
                {
                    if (firstCount != secondCount)
                        return false;
                    for ( // initialization
                        auto itFirst{firstCollection.begin()},
                        endFirst{firstCollection.end()},
                        itSecond{secondCollection.begin()},
                        endSecond{secondCollection.end()};
                        // condition
                        ((itFirst != endFirst) and (itSecond != endSecond));
                        // iteration
                        ++itFirst,
                        ++itSecond)
                    {
                        if (not std::forward<FN>(fn)(*itFirst, *itSecond))
                            return false;
                    }
                    return true;
                }
            };

            if constexpr (IsCljonicArray<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicArray<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicArray<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicArray<T> and IsCljonicRepeat<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicArray<T> and IsCljonicString<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicRepeat<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicIterator<T> and IsCljonicString<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicRepeat<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicRange<T> and IsCljonicString<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicRepeat<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicRepeat<T> and IsCljonicString<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), u.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicString<T> and IsCljonicArray<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicString<T> and IsCljonicIterator<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicString<T> and IsCljonicRange<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else if constexpr (IsCljonicString<T> and IsCljonicRepeat<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), t.Count(), t, u);
            }
            else if constexpr (IsCljonicString<T> and IsCljonicString<U>)
            {
                return AreNonSetCollectionsEqual(std::forward<F>(f), t.Count(), u.Count(), t, u);
            }
            else
            {
                return false;
            }
        }
        else if constexpr (IsCljonicSet<T> and IsCljonicSet<U>)
        {
            if (t.Count() != u.Count())
                return false; // LCOV_EXCL_LINE - This line of code may only execute at compile-time
            for (const auto& vt : t)
                if (not u.ContainsBy(std::forward<F>(f), vt))
                    return false; // LCOV_EXCL_LINE - This line of code may only execute at compile-time
            return true;
        }
        else if constexpr (IsBinaryPredicate<F, T, U>)
        {
            return std::forward<F>(f)(t, u);
        }
        else
        {
            return false;
        }
    }
    else if constexpr (IsBinaryPredicate<F, T, U>)
    {
        return std::forward<F>(f)(t, u);
    }
    else
    {
        return false;
    }
}

template <typename CollectionIterator, SizeType MaxElements>
constexpr bool AreUniqueValues(const CollectionIterator& begin, const CollectionIterator& end)
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    std::array<typename CollectionIterator::value_type, MaxElements> collectionElements{};
    SizeType count{0};
    for (auto it{begin}; it != end; ++it)
    {
        for (SizeType i{0}; i < count; ++i)
            if (AreEqualValues(collectionElements[i], *it))
                return false;
        if (count < MaxElements)
            collectionElements[count++] = *it;
        else
            return false; // LCOV_EXCL_LINE - This line of code may only execute at compile-time
    }
    return true;
}

template <typename F, typename CollectionIterator, SizeType MaxElements>
constexpr bool AreUniqueValuesBy(F&& f, const CollectionIterator& begin, const CollectionIterator& end)
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    std::array<typename CollectionIterator::value_type, MaxElements> collectionElements{};
    SizeType count{0};
    for (auto it{begin}; it != end; ++it)
    {
        for (SizeType i{0}; i < count; ++i)
            if (AreEqualValuesBy(std::forward<F>(f), collectionElements[i], *it))
                return false;
        if (count < MaxElements)
            collectionElements[count++] = *it;
        else
            return false;
    }
    return true;
}

template <typename T, typename U>
constexpr bool FirstLessThanSecond(const T& t, const U& u) noexcept
{
    // #lizard forgives -- The length and complexity of this function is acceptable

    if constexpr (IsCString<T> and IsCString<U>)
    {
        while (*t and (*t == *u))
        {
            ++t;
            ++u;
        }
        return *t < *u;
    }
    else
    {
        return t < u;
    }
}

template <typename T, typename... Ts>
constexpr auto MinArgument(T a, Ts... args) noexcept
{
    if constexpr (sizeof...(args) == 0)
    {
        return a;
    }
    else
    {
        return (a < MinArgument(args...)) ? a : MinArgument(args...);
    }
}

template <typename C, typename... Cs>
constexpr auto MinimumOfCljonicCollectionMaximumCounts()
{
    if constexpr (sizeof...(Cs) == 0)
    {
        return C::MaximumCount();
    }
    else
    {
        return (MinArgument(C::MaximumCount(), Cs::MaximumCount()), ...);
    }
}

template <typename C, typename... Cs>
constexpr auto SumOfCljonicCollectionMaximumCounts()
{
    if constexpr (sizeof...(Cs) == 0)
    {
        return C::MaximumCount();
    }
    else
    {
        return (C::MaximumCount() + ... + Cs::MaximumCount());
    }
}

constexpr SizeType MaximumElements(const SizeType count) noexcept
{
    return MinArgument(count, CljonicCollectionMaximumElementCount);
}

} // namespace cljonic

#endif // CLJONIC_SHARED_HPP
