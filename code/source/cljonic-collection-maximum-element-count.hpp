#ifndef CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_HPP
#define CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_HPP

#include <cstddef>

#ifndef CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT
#define CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT 1000
#endif

namespace cljonic
{

using SizeType = std::size_t;

constexpr auto CljonicCollectionMaximumElementCount{static_cast<SizeType>(CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT)};

} // namespace cljonic

#endif // CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_HPP