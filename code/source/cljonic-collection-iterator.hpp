#ifndef CLJONIC_COLLECTION_ITERATOR_HPP
#define CLJONIC_COLLECTION_ITERATOR_HPP

#include "cljonic-collection-maximum-element-count.hpp"

namespace cljonic
{

template <typename T>
class CollectionIterator
{
    const T& m_collection;
    SizeType m_index;

  public:
    constexpr CollectionIterator(const T& collection, const SizeType index) noexcept
        : m_collection(collection), m_index(index)
    {
    }

    constexpr auto operator*() const noexcept -> decltype(m_collection[m_index])
    {
        return m_collection[m_index];
    }

    constexpr CollectionIterator& operator++() noexcept
    {
        ++m_index;
        return *this;
    }

    constexpr CollectionIterator operator++(int) noexcept
    {
        auto result{*this};
        ++(*this);
        return result;
    }

    constexpr CollectionIterator& operator--() noexcept
    {
        --m_index;
        return *this;
    }

    constexpr bool operator!=(const CollectionIterator& other) const noexcept
    {
        return m_index != other.m_index;
    }

    constexpr CollectionIterator& operator+=(SizeType value) noexcept
    {
        m_index += value;
        return *this;
    }

    constexpr CollectionIterator operator+(SizeType value) const noexcept
    {
        auto result{*this};
        result += value;
        return result;
    }

    constexpr CollectionIterator& operator-=(SizeType value) noexcept
    {
        m_index -= value;
        return *this;
    }

    constexpr CollectionIterator operator-(SizeType value) const noexcept
    {
        auto result{*this};
        result -= value;
        return result;
    }
}; // CollectionIterator

} // namespace cljonic

#endif // CLJONIC_COLLECTION_ITERATOR_HPP
