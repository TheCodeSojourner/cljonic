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
    CollectionIterator(const T& collection, const std::size_t index) : m_collection(collection), m_index(index)
    {
    }

    auto operator*() const -> decltype(m_collection[m_index])
    {
        return m_collection[m_index];
    }

    CollectionIterator& operator++()
    {
        ++m_index;
        return *this;
    }

    bool operator!=(const CollectionIterator& other) const
    {
        return m_index != other.m_index;
    }
}; // CollectionIterator

} // namespace cljonic

#endif // CLJONIC_COLLECTION_ITERATOR_HPP
