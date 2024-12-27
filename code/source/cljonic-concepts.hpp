#ifndef CLJONIC_CONCEPTS_HPP
#define CLJONIC_CONCEPTS_HPP

#include <concepts>
#include <limits>
#include "cljonic-collection-type.hpp"

namespace each_function_is_invocable_with_next_return_type
{

// A base template struct for deducing the return type of function
template <typename F>
struct ReturnType;

// Specialization of ReturnType for deducing the return type of function
template <typename R, typename... Args>
struct ReturnType<R(Args...)>
{
    using type = R; // Function return type
};

// Specialization of ReturnType for deducing the return type of function pointer
template <typename R, typename... Args>
struct ReturnType<R (*)(Args...)>
{
    using type = R; // Function pointer return type
};

// Specialization of ReturnType for deducing the return type of function reference
template <typename R, typename... Args>
struct ReturnType<R (&)(Args...)>
{
    using type = R; //
};

// Specialization of ReturnType for deducing the return type of noexcept function
template <typename R, typename... Args>
struct ReturnType<R(Args...) noexcept>
{
    using type = R; // noexcept function return type
};

// Specialization of ReturnType for deducing the return type of noexcept function pointer
template <typename R, typename... Args>
struct ReturnType<R (*)(Args...) noexcept>
{
    using type = R; // noexcept function pointer return type
};

// Specialization of ReturnType for deducing the return type of noexcept function reference
template <typename R, typename... Args>
struct ReturnType<R (&)(Args...) noexcept>
{
    using type = R; // noexcept function reference return type
};

// A base template struct to check if each function in a list is invocable with the return type of the next function
template <typename F, typename... Fs>
struct AreInvocableWithReturn;

// Specialization of AreInvocableWithReturn for multiple functions
template <typename F1, typename F2, typename... Fs>
struct AreInvocableWithReturn<F1, F2, Fs...>
{
    // Value is true if F1 is invocable with the return type of F2 and the rest of the functions follow the same rule
    static constexpr bool value =
        std::regular_invocable<F1, typename ReturnType<F2>::type> and AreInvocableWithReturn<F2, Fs...>::value;
};

// Specialization of AreInvocableWithReturn for a single function
template <typename F>
struct AreInvocableWithReturn<F>
{
    static constexpr bool value = true; // Single function is always invocable
};

} // namespace each_function_is_invocable_with_next_return_type

namespace cljonic
{

using namespace each_function_is_invocable_with_next_return_type;

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
constexpr bool AllEqualityComparableTypes = (std::equality_comparable_with<T, Ts> and ...);

template <typename T, typename... Ts>
constexpr bool AllEqualityComparableValueTypes =
    (std::equality_comparable_with<typename T::value_type, typename Ts::value_type> and ...);

template <typename T, typename... Ts>
constexpr bool AnyFloatingPointTypes = (std::floating_point<T> or ... or std::floating_point<Ts>);

template <typename T, typename... Ts>
concept AllSameCljonicCollectionType =
    (std::same_as<typename T::cljonic_collection_type, typename Ts::cljonic_collection_type> and ...);

template <typename T, typename... Ts>
constexpr bool AnyFloatingPointValueTypes =
    (std::floating_point<typename T::value_type> or ... or std::floating_point<typename Ts::value_type>);

template <typename T>
concept CString = std::same_as<T, const char*> or std::same_as<T, char*>;

template <typename... Fs>
concept EachFunctionIsInvocableWithNextReturnType = AreInvocableWithReturn<Fs...>::value;

template <typename T>
concept NotCString = (not CString<T>);

template <typename F, IsCljonicCollection T, IsCljonicCollection... Ts>
constexpr bool IsBinaryPredicateForAllCljonicCollections =
    (IsBinaryPredicateForAll<F, typename T::value_type, typename Ts::value_type> and ...);

} // namespace cljonic

#endif // CLJONIC_CONCEPTS_HPP
