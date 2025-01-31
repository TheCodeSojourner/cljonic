#ifndef CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_HPP
#define CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_HPP

#include <cstddef>
#include <limits>

namespace cljonic
{

using SizeType = std::size_t;

constexpr SizeType operator"" _sz(unsigned long long int value)
{
    return static_cast<SizeType>(value);
}

constexpr auto CljonicCollectionMaximumElementCount{1000_sz};

constexpr auto CljonicInvalidIndex{std::numeric_limits<SizeType>::max()};

} // namespace cljonic

#endif // CLJONIC_COLLECTION_MAXIMUM_ELEMENT_COUNT_HPP