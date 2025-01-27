#ifndef CLJONIC_CONCEPTS_HPP
#define CLJONIC_CONCEPTS_HPP

#include <concepts>
#include <limits>
#include <type_traits>
#include "cljonic-collection-type.hpp"

namespace inner_find_common_type
{

// Base template struct for finding the best common type among multiple types
template <typename T, typename... Ts>
struct InnerFindCommonType;

// Specialization of InnerFindCommonType for a single type
template <typename T>
struct InnerFindCommonType<T>
{
    using type = T; // Common type is the single type itself
};

// Specialization of InnerFindCommonType for multiple types
template <typename T, typename U, typename... Ts>
struct InnerFindCommonType<T, U, Ts...>
{
    // Determines the common type among T, U, and Ts. If U and all Ts are convertible to T, the common type is T.
    // Otherwise, it recursively determines the common type among U and Ts.
    using type = std::conditional_t<(std::convertible_to<U, T> && ... && std::convertible_to<Ts, T>),
                                    typename InnerFindCommonType<T, Ts...>::type,
                                    typename InnerFindCommonType<U, Ts...>::type>;
};

} // namespace inner_find_common_type

namespace cljonic
{

using namespace inner_find_common_type;

template <typename P, typename T, typename U>
concept IsBinaryPredicate = requires(P p, T a, U b) {
    { p(a, b) } -> std::convertible_to<bool>;
};

template <typename P, typename T, typename... Ts>
concept IsBinaryPredicateForAll = (IsBinaryPredicate<P, T, Ts> && ...);

template <typename T>
concept IsCljonicArray = std::same_as<typename T::cljonic_collection_type,
                                      std::integral_constant<CljonicCollectionType, CljonicCollectionType::Array>>;

template <typename T>
concept IsCljonicCollection = requires { typename T::cljonic_collection_type; };

template <typename T>
concept IsCljonicRange = std::same_as<typename T::cljonic_collection_type,
                                      std::integral_constant<CljonicCollectionType, CljonicCollectionType::Range>>;

template <typename T>
concept IsCljonicRepeat = std::same_as<typename T::cljonic_collection_type,
                                       std::integral_constant<CljonicCollectionType, CljonicCollectionType::Repeat>>;

template <typename T>
concept IsCljonicSet = std::same_as<typename T::cljonic_collection_type,
                                    std::integral_constant<CljonicCollectionType, CljonicCollectionType::Set>>;

template <typename T>
concept IsCljonicArrayRangeOrRepeat = IsCljonicArray<T> or IsCljonicRange<T> or IsCljonicRepeat<T>;

template <typename T>
concept IsConvertibleToIntegral = std::convertible_to<T, char>     //
                                  or std::convertible_to<T, short> //
                                  or std::convertible_to<T, int>   //
                                  or std::convertible_to<T, long>  //
                                  or std::convertible_to<T, long long>;
template <typename T>
concept IsCString = std::same_as<std::decay_t<T>, char*> or std::same_as<std::decay_t<T>, const char*>;

template <typename T>
concept IsNotCljonicCollection = not IsCljonicCollection<T>;

template <typename P, typename T>
concept IsUnaryPredicate = requires(P p, T t) {
    { p(t) } -> std::convertible_to<bool>;
};

template <typename T, typename... Ts>
concept AllCljonicArrayRangeOrRepeat = (IsCljonicArrayRangeOrRepeat<T> and ... and IsCljonicArrayRangeOrRepeat<Ts>);

template <typename T, typename... Ts>
concept AllCljonicCollections = (IsCljonicCollection<T> and ... and IsCljonicCollection<Ts>);

template <typename T, typename... Ts>
concept AllCljonicSets = (IsCljonicSet<T> and ... and IsCljonicSet<Ts>);

template <typename T, typename... Ts>
constexpr bool AllConvertibleTypes = (std::convertible_to<T, Ts> and ...);

template <typename T, typename... Ts>
constexpr bool AllConvertibleValueTypes =
    (AllConvertibleTypes<typename T::value_type, typename Ts::value_type> and ...);

template <typename T, typename... Ts>
constexpr bool AllEqualityComparableTypes = (std::equality_comparable_with<T, Ts> and ...);

template <typename T, typename... Ts>
constexpr bool AllEqualityComparableValueTypes =
    (std::equality_comparable_with<typename T::value_type, typename Ts::value_type> and ...);

template <typename T, typename... Ts>
constexpr bool AnyFloatingPointTypes = (std::floating_point<T> or ... or std::floating_point<Ts>);

template <typename T, typename... Ts>
concept AllNotCljonicCollections = (IsNotCljonicCollection<T> and ... and IsNotCljonicCollection<Ts>);

template <typename T, typename... Ts>
concept AllSameCljonicCollectionType =
    (std::same_as<typename T::cljonic_collection_type, typename Ts::cljonic_collection_type> and ...);

template <typename T, typename... Ts>
constexpr bool AnyFloatingPointValueTypes =
    (std::floating_point<typename T::value_type> or ... or std::floating_point<typename Ts::value_type>);

template <typename T, typename... Ts>
using FindCommonType = typename InnerFindCommonType<T, Ts...>::type;

template <typename T, typename... Ts>
using FindCommonValueType = typename InnerFindCommonType<typename T::value_type, typename Ts::value_type...>::type;

template <typename F, IsCljonicCollection T, IsCljonicCollection... Ts>
constexpr bool IsBinaryPredicateForAllCljonicCollections =
    (IsBinaryPredicateForAll<F, typename T::value_type, typename Ts::value_type> and ...);

} // namespace cljonic

#endif // CLJONIC_CONCEPTS_HPP