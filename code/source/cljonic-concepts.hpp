#ifndef CLJONIC_CONCEPTS_HPP
#define CLJONIC_CONCEPTS_HPP

#include <concepts>
#include <limits>
#include "cljonic-collection-type.hpp"

namespace cljonic
{

template <typename T>
concept IsCljonicCollection = std::same_as<typename T::cljonic_collection, std::true_type>;

template <typename T>
concept IsCljonicArray = std::same_as<typename T::cljonic_collection_type,
                                      std::integral_constant<CljonicCollectionType, CljonicCollectionType::Array>>;

template <typename T>
concept IsCljonicRange = std::same_as<typename T::cljonic_collection_type,
                                      std::integral_constant<CljonicCollectionType, CljonicCollectionType::Range>>;

template <typename T>
concept IsCljonicSet = std::same_as<typename T::cljonic_collection_type,
                                    std::integral_constant<CljonicCollectionType, CljonicCollectionType::Set>>;

template <typename T>
concept IsCljonicArrayOrRange = IsCljonicArray<T> or IsCljonicRange<T>;

template <typename T, typename... Ts>
concept AllCljonicArrayOrRange = (IsCljonicArrayOrRange<T> and ... and IsCljonicArrayOrRange<Ts>);

template <typename T, typename... Ts>
concept AllCljonicCollections = (IsCljonicCollection<T> and ... and IsCljonicCollection<Ts>);

template <typename T, typename... Ts>
concept AllCljonicSets = (IsCljonicSet<T> and ... and IsCljonicSet<Ts>);

template <typename T, typename... Ts>
constexpr bool AllEqualityComparableValueTypes =
    (std::equality_comparable_with<typename T::value_type, typename Ts::value_type> and ...);

template <typename T, typename... Ts>
concept AllSameCljonicCollectionType =
    (std::same_as<typename T::cljonic_collection_type, typename Ts::cljonic_collection_type> and ...);

template <typename T, typename... Ts>
constexpr bool AnyFloatingPointTypes = (std::floating_point<T> or ... or std::floating_point<Ts>);

template <typename T, typename... Ts>
constexpr bool AnyFloatingPointValueTypes =
    (std::floating_point<typename T::value_type> or ... or std::floating_point<typename Ts::value_type>);

template <typename T, typename... Ts>
constexpr bool AllEqualityComparableTypes = (std::equality_comparable_with<T, Ts> and ...);

template <typename T>
concept CString = std::same_as<T, const char*> or std::same_as<T, char*>;

} // namespace cljonic

#endif // CLJONIC_CONCEPTS_HPP
