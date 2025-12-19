#ifndef CLJONIC_CONCEPTS_HPP
#define CLJONIC_CONCEPTS_HPP

#include <concepts>
#include <limits>
#include <type_traits>
#include "cljonic-collection-type.hpp"

namespace cljonic
{

template <typename T>
concept IsArithmetic = std::integral<T> or std::floating_point<T>;

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
concept IsCljonicIterator =
    std::same_as<typename T::cljonic_collection_type,
                 std::integral_constant<CljonicCollectionType, CljonicCollectionType::Iterator>>;

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
concept IsUnaryFunction = requires(P p, T t) {
    { p(t) } -> std::convertible_to<T>;
};

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
concept AllConvertibleTypes = (std::convertible_to<T, Ts> && ...);

template <typename T, typename... Ts>
concept AllConvertibleValueTypes = (AllConvertibleTypes<typename T::value_type, typename Ts::value_type> && ...);

template <typename T, typename... Ts>
concept AllEqualityComparableTypes = (std::equality_comparable_with<T, Ts> && ...);

template <typename T, typename... Ts>
concept AllEqualityComparableValueTypes =
    (std::equality_comparable_with<typename T::value_type, typename Ts::value_type> && ...);

template <typename T, typename... Ts>
concept AnyFloatingPointTypes = (std::floating_point<T> || ... || std::floating_point<Ts>);

template <typename T, typename... Ts>
concept AllNotCljonicCollections = (IsNotCljonicCollection<T> and ... and IsNotCljonicCollection<Ts>);

template <typename T, typename... Ts>
concept AllSameCljonicCollectionType =
    (std::same_as<typename T::cljonic_collection_type, typename Ts::cljonic_collection_type> and ...);

template <typename T, typename... Ts>
concept AnyFloatingPointValueTypes =
    (std::floating_point<typename T::value_type> || ... || std::floating_point<typename Ts::value_type>);

template <typename T>                           // eliminate types that use exceptions in some places
concept ValidCljonicContainerElementType =      //
    std::is_nothrow_copy_constructible_v<T> and //
    std::is_nothrow_move_constructible_v<T> and //
    std::is_nothrow_copy_assignable_v<T> and    //
    std::is_nothrow_move_assignable_v<T> and    //
    std::is_nothrow_destructible_v<T>;

template <typename T, typename... Ts>
using FindCommonType = std::common_type_t<T, Ts...>;

template <typename T, typename... Ts>
using FindCommonValueType = std::common_type_t<typename T::value_type, typename Ts::value_type...>;

template <typename F, IsCljonicCollection T, IsCljonicCollection... Ts>
constexpr bool IsBinaryPredicateForAllCljonicCollections =
    (IsBinaryPredicateForAll<F, typename T::value_type, typename Ts::value_type> and ...);

} // namespace cljonic

#endif // CLJONIC_CONCEPTS_HPP