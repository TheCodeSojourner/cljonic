#ifndef CLJONIC_COLLECTION_TYPES_HPP
#define CLJONIC_COLLECTION_TYPES_HPP

namespace cljonic
{

enum class CljonicCollectionType
{
    Array,
    Cycle,
    Iterator,
    Range,
    Repeat,
    Set,
    String
};

} // namespace cljonic

#endif // CLJONIC_COLLECTION_TYPES_HPP
