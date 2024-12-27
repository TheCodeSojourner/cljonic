#ifndef CLJONIC_COLLECTION_ITERATOR_HPP
#define CLJONIC_COLLECTION_ITERATOR_HPP

namespace cljonic
{

template <typename T>
class CollectionIterator
{
    const T& m_collection;
    std::size_t m_index;

  public:
    CollectionIterator(const T& collection, const std::size_t index) noexcept : m_collection(collection), m_index(index)
    {
    }

    auto operator*() const noexcept -> decltype(m_collection[m_index])
    {
        return m_collection[m_index];
    }

    CollectionIterator& operator++() noexcept
    {
        ++m_index;
        return *this;
    }

    bool operator!=(const CollectionIterator& other) const noexcept
    {
        return m_index != other.m_index;
    }

    CollectionIterator& operator+=(int value) noexcept
    {
        m_index += value;
        return *this;
    }

    CollectionIterator operator+(int value) const noexcept
    {
        CollectionIterator temp = *this;
        temp += value;
        return temp;
    }
}; // CollectionIterator

} // namespace cljonic

#endif // CLJONIC_COLLECTION_ITERATOR_HPP
