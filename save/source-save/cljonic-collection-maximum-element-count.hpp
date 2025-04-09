#ifndef CONSTANT_CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_HPP
#define CONSTANT_CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_HPP

#include <cstddef>
#include <limits>

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

    constexpr auto CljonicCollectionMaximumElementCount{static_cast<SizeType>(CONSTANT_CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT)};
    constexpr auto CljonicInvalidIndex{std::numeric_limits<SizeType>::max()};

} // namespace cljonic

#endif // CONSTANT_CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_HPP