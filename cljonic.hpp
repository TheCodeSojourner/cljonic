#ifndef CLJONIC_H_
#define CLJONIC_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DO NOT EDIT THIS FILE.  IT IS AUTOMATICALLY GENERATED BY INVOKING 'make cljonic' WITHIN THE cljonic DIRECTORY.
//
// TO UPDATE THIS FILE, MAKE CHANGES TO THE FILES IN THE code/source DIRECTORY.
//
// ONLY cljonic DEVELOPERS SHOULD MAKE CHANGES.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Paul Whittington All rights reserved. The use and distribution terms for this software are covered by
// the Apache License Version 2.0, January 2004 (http://www.apache.org/licenses/LICENSE-2.0). By using this software in
// any fashion, you are agreeing to be bound by the terms of this license. You must not remove this notice, or any
// other, from this software.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This file was generated Wed Jan  1 01:25:21 PM MST 2025

namespace cljonic {

template <typename T>
class CollectionIterator {
const T& m_collection;
std::size_t m_index;

public:
constexpr CollectionIterator(const T& collection, const std::size_t index) noexcept
    : m_collection(collection), m_index(index) {
}

constexpr auto operator*() const noexcept -> decltype(m_collection[m_index]) {
return m_collection[m_index];
}

constexpr CollectionIterator& operator++() noexcept {
++m_index;
return *this;
}

constexpr bool operator!=(const CollectionIterator& other) const noexcept {
return m_index != other.m_index;
}

constexpr CollectionIterator& operator+=(int value) noexcept {
m_index += value;
return *this;
}

constexpr CollectionIterator operator+(int value) const noexcept {
CollectionIterator temp = *this;
temp += value;
return temp;
}
};

} // namespace cljonic

namespace cljonic {

enum class CljonicCollectionType {
    Array,
    Cycle,
    Range,
    Repeat,
    Set,
    String
};

}

#include <concepts>
#include <limits>
#include <type_traits>

namespace inner_find_common_type {

template <typename T, typename... Ts>
struct InnerFindCommonType;

template <typename T>
struct InnerFindCommonType<T> {
using type = T;
};

template <typename T, typename U, typename... Ts>
struct InnerFindCommonType<T, U, Ts...> {

using type = std::conditional_t<(std::convertible_to<U, T> && ... && std::convertible_to<Ts, T>),
                                typename InnerFindCommonType<T, Ts...>::type,
                                typename InnerFindCommonType<U, Ts...>::type>;
};

} // namespace inner_find_common_type

namespace cljonic {

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
concept AllSameCljonicCollectionType =
    (std::same_as<typename T::cljonic_collection_type, typename Ts::cljonic_collection_type> and ...);

template <typename T, typename... Ts>
constexpr bool AnyFloatingPointValueTypes =
    (std::floating_point<typename T::value_type> or ... or std::floating_point<typename Ts::value_type>);

template <typename T>
concept CString = std::same_as<T, const char*> or std::same_as<T, char*>;

template <typename T, typename... Ts>
using FindCommonType = typename InnerFindCommonType<T, Ts...>::type;

template <typename T, typename... Ts>
using FindCommonValueType = typename InnerFindCommonType<typename T::value_type, typename Ts::value_type...>::type;

template <typename T>
concept NotCString = (not CString<T>);

template <typename F, IsCljonicCollection T, IsCljonicCollection... Ts>
constexpr bool IsBinaryPredicateForAllCljonicCollections =
    (IsBinaryPredicateForAll<F, typename T::value_type, typename Ts::value_type> and ...);

} // namespace cljonic

#include <concepts>
#include <cstring>

namespace cljonic {

template <typename F, typename T, typename U>
constexpr bool AreEqualBy(const F& f, const T& t, const U& u) noexcept {
static_assert(std::predicate<F, T, U>, "Function is not a valid binary predicate for the parameters");
return f(t, u);
}

constexpr bool AreEqual(NotCString auto a, NotCString auto b) noexcept {
return a == b;
}

constexpr bool AreEqual(CString auto a, CString auto b) noexcept {
return std::strcmp(a, b) == 0;
}

constexpr auto Min(auto a, auto b) noexcept {
return (a < b) ? a : b;
}

template <typename C, typename... Cs>
constexpr auto MinimumOfCljonicCollectionMaximumCounts() {
if constexpr(sizeof...(Cs) == 0) {
return C::MaximumCount();
} else {
return (Min(C::MaximumCount(), Cs::MaximumCount()), ...);
}
}

template <typename C, typename... Cs>
constexpr auto SumOfCljonicCollectionMaximumCounts() {
if constexpr(sizeof...(Cs) == 0) {
return C::MaximumCount();
} else {
return (C::MaximumCount() + ... + Cs::MaximumCount());
}
}

} // namespace cljonic

#include <cstring>
#include <initializer_list>
#include <type_traits>

namespace cljonic {
template <typename T, std::size_t MaxElements>
class Array {
using MaxElementsType = decltype(MaxElements);

MaxElementsType m_elementCount;
const T m_elementDefault;
T m_elements[MaxElements];

public:
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Array>;
using size_type = MaxElementsType;
using value_type = T;

constexpr Array() noexcept : m_elementCount(0), m_elementDefault(T{}) {
}

constexpr explicit Array(const std::initializer_list<const T> elements) noexcept
    : m_elementCount(Min(MaxElements, elements.size())), m_elementDefault(T{}) {
for(size_type i{0}; i < m_elementCount; ++i)
m_elements[i] = *(elements.begin() + i);
}

constexpr Array(const Array& other) = default;
constexpr Array(Array&& other) = default;

constexpr const T* begin() const noexcept {
return m_elements;
}

constexpr const T* end() const noexcept {
return m_elements + m_elementCount;
}

constexpr const T& operator[](const MaxElementsType index) const noexcept {
return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
}

constexpr const T& operator()(const MaxElementsType index) const noexcept {
return this->operator[](index);
}

constexpr void MConj(const T& t) noexcept {
if(m_elementCount < MaxElements)
m_elements[m_elementCount++] = t;
}

[[nodiscard]] constexpr MaxElementsType Count() const noexcept {
return m_elementCount;
}

constexpr const T& DefaultElement() const noexcept {
return m_elementDefault;
}

static constexpr std::size_t MaximumCount() noexcept {
return MaxElements;
}
};

template <typename... Args>
Array(Args...) -> Array<std::common_type_t<Args...>, sizeof...(Args)>;

} // namespace cljonic

#include <concepts>
#include <limits>

namespace cljonic {

template <int... StartEndStep>
class Range {
private:
static_assert(sizeof...(StartEndStep) <= 3, "Number of Range parameters must be less than or equal to three");

using Iterator = CollectionIterator<Range>;
using SizeType = std::size_t;

SizeType m_elementCount;
int m_elementDefault;
int m_elementStart;
int m_elementStep;

static constexpr auto RangeCount(const int start, const int end, const int step) noexcept {
return (0 == step) ? 0 : ((end - start) / step) + ((((end - start) % step) == 0) ? 0 : 1);
}

static constexpr int values[] = {StartEndStep...};

static constexpr auto MaxElements = []() constexpr {
if constexpr(sizeof...(StartEndStep) == 1)
return static_cast<SizeType>(values[0]);
else if constexpr(sizeof...(StartEndStep) == 2)
return static_cast<SizeType>(values[1] - values[0]);
else if constexpr(sizeof...(StartEndStep) == 3) {
return static_cast<SizeType>(RangeCount(StartEndStep...));
} else {
return std::numeric_limits<SizeType>::max();
}
}();

constexpr void InitializeMembers(const int count, const int start, const int step) noexcept {
m_elementCount = static_cast<SizeType>(count);
m_elementDefault = 0;
m_elementStart = start;
m_elementStep = step;
}

constexpr void Initialize() noexcept {
InitializeMembers(std::numeric_limits<int>::max(), 0, 1);
}

constexpr void InitializeEnd(const int end) noexcept {
if(end <= 0)
InitializeMembers(0, 0, 1);
else
InitializeMembers(end, 0, 1);
}

constexpr void InitializeStartEnd(const int start, const int end) noexcept {
if(end <= start)
InitializeMembers(0, 0, 1);
else
InitializeMembers((end - start), start, 1);
}

constexpr void InitializeStartEndStepWithNegativeStep(const int start, const int end, const int step) noexcept {
if(start <= end)
InitializeMembers(0, 0, step);
else {
const int count{RangeCount(start, end, step)};
InitializeMembers(count, start, step);
}
}

constexpr void InitializeStartEndStepWithPositiveStep(const int start, const int end, const int step) noexcept {
if(end <= start)
InitializeMembers(0, 0, step);
else {
const int count{RangeCount(start, end, step)};
InitializeMembers(count, start, step);
}
}

constexpr void InitializeStartEndStep(const int start, const int end, const int step) noexcept {

if((0 == step) and (start == end))
InitializeMembers(0, 0, 0);
else if(0 == step)
InitializeMembers(std::numeric_limits<int>::max(), start, 0);
else if(step < 0)
InitializeStartEndStepWithNegativeStep(start, end, step);
else
InitializeStartEndStepWithPositiveStep(start, end, step);
}

public:
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Range>;
using size_type = SizeType;
using value_type = int;

constexpr Range() noexcept
    : m_elementCount{static_cast<std::size_t>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementStep{0} {

if constexpr(sizeof...(StartEndStep) == 1) {
InitializeEnd(values[0]);
} else if constexpr(sizeof...(StartEndStep) == 2) {
InitializeStartEnd(values[0], values[1]);
} else if constexpr(sizeof...(StartEndStep) == 3) {
InitializeStartEndStep(values[0], values[1], values[2]);
} else {
Initialize();
}
}

constexpr Range(const Range& other) = default;
constexpr Range(Range&& other) = default;

[[nodiscard]] constexpr Iterator begin() const noexcept {
return Iterator{*this, 0};
}

[[nodiscard]] constexpr Iterator end() const noexcept {
return Iterator{*this, m_elementCount};
}

constexpr int operator[](const size_type index) const noexcept {
return ((0 == m_elementCount) or (index >= m_elementCount))
           ? m_elementDefault
           : (m_elementStart + (static_cast<int>(index) * m_elementStep));
}

[[nodiscard]] constexpr size_type Count() const noexcept {
return m_elementCount;
}

constexpr int DefaultElement() const noexcept {
return m_elementDefault;
}

static constexpr auto MaximumCount() noexcept {
return MaxElements;
}
};

} // namespace cljonic

#include <concepts>
#include <limits>

namespace cljonic {

template <std::size_t MaxElements, typename T>
class Repeat {
using Iterator = CollectionIterator<Repeat>;

const std::size_t m_elementCount;
const T m_elementDefault;
const T m_elementValue;

public:
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Repeat>;
using size_type = std::size_t;
using value_type = T;

constexpr explicit Repeat(const T& t) noexcept
    : m_elementCount{MaxElements}, m_elementDefault{T{}}, m_elementValue{t} {
}

constexpr Repeat(const Repeat& other) = default;
constexpr Repeat(Repeat&& other) = default;

[[nodiscard]] constexpr Iterator begin() const noexcept {
return Iterator{*this, 0};
}

[[nodiscard]] constexpr Iterator end() const noexcept {
return Iterator{*this, m_elementCount};
}

constexpr const T& operator[](const size_type index) const noexcept {
return ((m_elementCount <= 0) or (index >= m_elementCount)) ? m_elementDefault : m_elementValue;
}

[[nodiscard]] constexpr size_type Count() const noexcept {
return m_elementCount;
}

constexpr const T& DefaultElement() const noexcept {
return m_elementDefault;
}

static constexpr auto MaximumCount() noexcept {
return MaxElements;
}
};

template <typename T>
Repeat(T) -> Repeat<std::numeric_limits<std::size_t>::max(), T>;

} // namespace cljonic

#include <concepts>
#include <cstring>
#include <initializer_list>
#include <type_traits>

namespace cljonic {
template <typename T, std::size_t MaxElements>
class Set {
static_assert(not std::floating_point<T>,
              "Floating point types should not be compared for equality, hence Sets of floating point types are "
              "not allowed");

static_assert(std::equality_comparable<T>, "A Set type must be equality comparable");

using MaxElementsType = decltype(MaxElements);

MaxElementsType m_elementCount;
const T m_elementDefault;
T m_elements[MaxElements];

constexpr bool IsUniqueElementBy(const auto& f, const T& element) const noexcept {
auto result{true};
for(MaxElementsType i{0}; (result and (i < m_elementCount)); ++i)
result = not AreEqualBy(f, element, m_elements[i]);
return result;
}

constexpr bool IsUniqueElement(const T& element) const noexcept {
auto result{true};
for(MaxElementsType i{0}; (result and (i < m_elementCount)); ++i)
result = not AreEqual(element, m_elements[i]);
return result;
}

public:
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Set>;
using size_type = MaxElementsType;
using value_type = T;

constexpr Set() noexcept : m_elementCount(0), m_elementDefault(T{}) {
}

constexpr explicit Set(const std::initializer_list<const T> elements) noexcept
    : m_elementCount(0), m_elementDefault(T{}) {

for(const auto& element : elements) {
if(m_elementCount == MaxElements)
break;
if(IsUniqueElement(element))
m_elements[m_elementCount++] = element;
}
}

constexpr Set(const Set& other) = default;
constexpr Set(Set&& other) = default;

constexpr const T* begin() const noexcept {
return m_elements;
}

constexpr const T* end() const noexcept {
return m_elements + m_elementCount;
}

constexpr const T& operator[](const MaxElementsType index) const noexcept {
return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
}

constexpr const T& operator()(const T& t) const noexcept {
return Contains(t) ? t : m_elementDefault;
}

[[nodiscard]] constexpr MaxElementsType Count() const noexcept {
return m_elementCount;
}

constexpr bool ContainsBy(const auto& f, const T& element) const noexcept {
return not IsUniqueElementBy(f, element);
}

constexpr bool Contains(const T& element) const noexcept {
return not IsUniqueElement(element);
}

constexpr int DefaultElement() const noexcept {
return m_elementDefault;
}

static constexpr std::size_t MaximumCount() noexcept {
return MaxElements;
}
};

template <typename... Args>
Set(Args...) -> Set<std::common_type_t<Args...>, sizeof...(Args)>;

} // namespace cljonic

#include <cstring>
#include <initializer_list>
#include <type_traits>

namespace cljonic {

template <std::size_t MaxElements>
class String {
using Iterator = CollectionIterator<String>;
using MaxElementsType = decltype(MaxElements);

MaxElementsType m_elementCount;
const char m_elementDefault;
char m_elements[MaxElements + 1];

public:
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::String>;
using size_type = MaxElementsType;
using value_type = char;

constexpr String() noexcept : m_elementCount(0), m_elementDefault('\0') {
m_elements[0] = '\0';
}

constexpr explicit String(const std::initializer_list<const char> elements) noexcept
    : m_elementCount(0), m_elementDefault('\0') {
for(const auto& element : elements) {
if(m_elementCount == MaxElements)
break;
m_elements[m_elementCount++] = element;
}
m_elements[m_elementCount] = '\0';
}

constexpr explicit String(const char* c_str) noexcept : m_elementCount(0), m_elementDefault('\0') {
while((m_elementCount < MaxElements) and ('\0' != c_str[m_elementCount])) {
m_elements[m_elementCount] = c_str[m_elementCount];
m_elementCount += 1;
}
m_elements[m_elementCount] = '\0';
}

constexpr String(const String& other) = default;
constexpr String(String&& other) = default;

[[nodiscard]] constexpr Iterator begin() const noexcept {
return Iterator{*this, 0};
}

[[nodiscard]] constexpr Iterator end() const noexcept {
return Iterator{*this, m_elementCount};
}

constexpr char operator[](const MaxElementsType index) const noexcept {
return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
}

constexpr char operator()(const MaxElementsType index) const noexcept {
return this->operator[](index);
}

[[nodiscard]] constexpr MaxElementsType Count() const noexcept {
return m_elementCount;
}

constexpr int DefaultElement() const noexcept {
return m_elementDefault;
}

static constexpr std::size_t MaximumCount() noexcept {
return MaxElements;
}
};

template <std::size_t N>
String(const char (&)[N]) -> String<N - 1>;

template <typename... Args>
String(Args...) -> String<sizeof...(Args)>;

} // namespace cljonic

namespace cljonic {

namespace core {
}

}

#include <utility>

namespace cljonic {

namespace core {

template <typename F1, typename F2, typename... Args>
concept ComposeIsCallableWith = requires(F1 f1, F2 f2, Args&&... args) {
{ f1(f2(std::forward<Args>(args)...)) };
};

template <typename F1, typename F2>
constexpr auto Compose(F1&& f1, F2&& f2) noexcept {
return [f1 = std::forward<F1>(f1), f2 = std::forward<F2>(f2)]<typename... T>(T&&... args) {
static_assert(ComposeIsCallableWith<F1, F2, T...>,
              "Each Compose argument must be callable with one argument of the return type of the argument to "
              "its right. Was the Compose result function called with the correct number of arguments?");

return f1(f2(std::forward<T>(args)...));
};
}
template <typename F1, typename F2, typename... Fs>
constexpr auto Compose(F1&& f1, F2&& f2, Fs&&... fs) noexcept {
return Compose(std::forward<F1>(f1), Compose(std::forward<F2>(f2), std::forward<Fs>(fs)...));
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename C, typename... Cs>
constexpr auto Concat(const C& c, const Cs&... cs) noexcept {

static_assert(AllCljonicCollections<C, Cs...>, "All Concat parameters must be cljonic collections");

static_assert(AllConvertibleValueTypes<C, Cs...>,
              "All Concat cljonic collections value types must be interconvertible");

using ResultType = FindCommonValueType<C, Cs...>;
using SizeType = decltype(c.Count());
constexpr auto count{SumOfCljonicCollectionMaximumCounts<C, Cs...>()};
auto result{Array<ResultType, count>{}};
const auto MConjCollectionOntoResult = [&](const auto& c) {
for(SizeType i{0}; i < c.Count(); ++i)
result.MConj(c[i]);
};
(MConjCollectionOntoResult(c), ..., MConjCollectionOntoResult(cs));
return result;
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename C, typename... Es>
constexpr auto Conj(const C& c, const Es&... es) noexcept {

static_assert(IsCljonicCollection<C>, "First Conj parameter must be a cljonic collection");

static_assert(AllConvertibleTypes<typename C::value_type, Es...>,
              "Second through last Conj parameters must be convertible to cljonic collection value type");

using ResultType = typename C::value_type;
using SizeType = decltype(c.Count());
constexpr auto count{C::MaximumCount() + sizeof...(Es)};
auto result{Array<ResultType, count>{}};
const auto MConjElementOntoResult = [&](const auto& e) { result.MConj(e); };
for(SizeType i{0}; i < c.Count(); ++i)
MConjElementOntoResult(c[i]);
(MConjElementOntoResult(es), ...);
return result;
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename C>
constexpr auto Count(const C& c) noexcept {
static_assert(IsCljonicCollection<C>, "Count parameter must be a cljonic collection");

return c.Count();
}

}

} // namespace cljonic::core

#include <utility>

namespace cljonic {

namespace core {

template <typename C>
class CycleCollection {
using MaxElementsType = typename C::size_type;
using ElementType = typename C::value_type;

const C m_collection;

[[nodiscard]] MaxElementsType IndexToElementIndex(const MaxElementsType index) const noexcept {
return (0 == m_collection.Count()) ? 0 : (index % m_collection.Count());
}

class CycleIterator {
const CycleCollection& m_cycle;
MaxElementsType m_index;

public:
constexpr CycleIterator(const CycleCollection& cycle, const MaxElementsType index) noexcept
    : m_cycle(cycle), m_index(index) {
}

constexpr auto operator*() const noexcept -> decltype(m_cycle[m_index]) {
return m_cycle[m_cycle.IndexToElementIndex(m_index)];
}

constexpr CycleIterator& operator++() noexcept {
++m_index;
return *this;
}

constexpr bool operator!=(const CycleIterator& other) const noexcept {
return m_index != other.m_index;
}

constexpr CycleIterator& operator+=(const int value) noexcept {
m_index += value;
return *this;
}

constexpr CycleIterator operator+(const int value) const noexcept {
CycleIterator temp = *this;
temp += value;
return temp;
}
};

public:
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Cycle>;
using size_type = MaxElementsType;
using value_type = ElementType;

explicit CycleCollection(const C& collection) : m_collection(collection) {
}

explicit CycleCollection(C&& collection) : m_collection(std::move(collection)) {
}

constexpr CycleCollection(const CycleCollection& other) = default;
constexpr CycleCollection(CycleCollection&& other) = default;

[[nodiscard]] constexpr CycleIterator begin() const noexcept {
return CycleIterator(*this, 0);
}

[[nodiscard]] constexpr CycleIterator end() const noexcept {
return CycleIterator(*this, MaximumCount());
}

[[nodiscard]] constexpr ElementType operator[](const MaxElementsType index) const noexcept {
return m_collection[IndexToElementIndex(index)];
}

[[nodiscard]] constexpr MaxElementsType Count() const noexcept {
return MaximumCount();
}

[[nodiscard]] constexpr ElementType& DefaultElement() const noexcept {
return m_collection.DefaultElement();
}

[[nodiscard]] constexpr std::size_t MaximumCount() const noexcept {
return (0 == m_collection.Count()) ? 0 : std::numeric_limits<MaxElementsType>::max();
}
};
template <typename C>
constexpr auto Cycle(const C& c) noexcept {
static_assert(IsCljonicCollection<C>, "Cycle parameter must be a cljonic collection");

return CycleCollection{c};
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename T, typename... Ts>
constexpr auto Equal(const T& t, const Ts&... ts) noexcept {
return EqualBy([](const auto& a, const auto& b) { return AreEqual(a, b); }, t, ts...);
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename F, typename T, typename... Ts>
constexpr auto EqualBy(const F& f, const T& t, const Ts&... ts) noexcept {

if constexpr(sizeof...(Ts) <= 0) {
if constexpr(IsCljonicCollection<T>)
static_assert((not std::floating_point<typename T::value_type>),
              "cljonic floating point collection value types should not be compared for equality");
else
static_assert((not std::floating_point<T>), "Floating point types should not be compared for equality");
return true;
} else if constexpr(AllCljonicCollections<T, Ts...>) {
static_assert(AllSameCljonicCollectionType<T, Ts...> or AllCljonicArrayRangeOrRepeat<T, Ts...>,
              "cljonic collection types are not all the same, or all Array, Range or Repeat types");

static_assert(not AnyFloatingPointValueTypes<T, Ts...>,
              "cljonic floating point collection value types should not be compared for equality");

static_assert(IsBinaryPredicateForAllCljonicCollections<F, T, Ts...>,
              "Function is not a valid binary predicate for all cljonic collection value types");

if constexpr(sizeof...(Ts) <= 0) {
return true;
} else if constexpr(AllCljonicSets<T, Ts...>) {
auto EqualSets = [&](const auto& c1, const auto& c2) {
using CountType = decltype(c1.Count());
auto result{c1.Count() == c2.Count()};
for(CountType i{0}; (result and (i < c1.Count())); ++i)
result = c2.ContainsBy(f, c1[i]);
return result;
};
return (EqualSets(t, ts) and ...);
} else {
auto EqualCollections = [&](const auto& c1, const auto& c2) {
using CountType = decltype(c1.Count());
auto result{c1.Count() == c2.Count()};
for(CountType i{0}; (result and (i < c1.Count())); ++i)
result = AreEqualBy(f, c1[i], c2[i]);
return result;
};
return (EqualCollections(t, ts) and ...);
}
} else {
static_assert(not AnyFloatingPointTypes<T, Ts...>, "Floating point types should not be compared for equality");

static_assert(AllEqualityComparableTypes<T, Ts...>, "Not all types are equality comparable");

static_assert(IsBinaryPredicateForAll<F, T, Ts...>,
              "Function is not a valid binary predicate for all parameters");

auto EqualParameters = [&](const auto& p1, const auto& p2) { return AreEqualBy(f, p1, p2); };
return (EqualParameters(t, ts) and ...);
}
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename F, typename C>
constexpr auto Every(const F& f, const C& c) noexcept {
static_assert(IsCljonicCollection<C>, "The second parameter must be a cljonic collection");

static_assert(IsUnaryPredicate<F, typename C::value_type>,
              "Function is not a valid unary predicate for the collection value type");

using CountType = decltype(c.Count());
auto result{true};
for(CountType i{0}; (result and (i < c.Count())); ++i)
result = f(c[i]);
return result;
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename F, typename C>
constexpr auto Filter(const F& f, const C& c) noexcept {
static_assert(IsCljonicCollection<C>, "The second parameter must be a cljonic collection");

static_assert(IsUnaryPredicate<F, typename C::value_type>,
              "Function is not a valid unary predicate for the collection value type");

auto result{Array<typename C::value_type, c.MaximumCount()>{}};
for(const auto& element : c)
if(f(element))
result.MConj(element);
return result;
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename C>
constexpr auto First(const C& coll) noexcept {
static_assert(IsCljonicCollection<C>, "First parameter must be a cljonic collection");

return coll[0];
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename F, typename C, typename... Cs>
constexpr auto Map(F&& f, const C& c, const Cs&... cs) noexcept {
static_assert(AllCljonicCollections<C, Cs...>, "The second through last parameters must be cljonic collections");

static_assert(std::invocable<F, typename C::value_type, typename Cs::value_type...>,
              "Function cannot be called with values from the specified cljonic collections");

using ResultType = decltype(f(std::declval<typename C::value_type>(), std::declval<typename Cs::value_type>()...));
using SizeType = decltype(c.Count());
auto result{Array<ResultType, MinimumOfCljonicCollectionMaximumCounts<C, Cs...>()>{}};
for(SizeType i{0}; i < c.Count(); ++i)
result.MConj(f(c[i], cs[i]...));
return result;
}

}

} // namespace cljonic::core

#include <utility>

namespace cljonic {

namespace core {
template <typename F, typename... Args>
constexpr auto Partial(F&& f, Args&&... args) noexcept {
return [f = std::forward<F>(f), ... args = std::forward<Args>(args)](auto&&... rest) {
static_assert(std::regular_invocable<F, Args..., decltype(rest)...>,
              "Partial function cannot be called with the specified arguments");

return f(args..., std::forward<decltype(rest)>(rest)...);
};
}

}

} // namespace cljonic::core

#include <numeric>
#include <tuple>

namespace cljonic {

namespace core {
template <typename F, typename C>
constexpr auto Reduce(F&& f, const C& c) noexcept {
static_assert(IsCljonicCollection<C>, "Second parameter must be a cljonic collection");

static_assert(std::regular_invocable<F, typename C::value_type, typename C::value_type>,
              "Function cannot be called with two parameters of the collection value type");

using ResultType = std::invoke_result_t<F, typename C::value_type, typename C::value_type>;
static_assert(std::regular_invocable<F, ResultType, typename C::value_type>,
              "Function cannot be called with parameters of function result type, and collection value type");

return (0 == c.Count()) ? c.DefaultElement()
                        : std::accumulate((c.begin() + 1), c.end(), *c.begin(), std::forward<F>(f));
}

template <typename F, typename T, typename C>
constexpr auto Reduce(F&& f, const T& t, const C& c) noexcept {
static_assert(IsCljonicCollection<C>, "Third parameter must be a cljonic collection");

static_assert(std::regular_invocable<F, T, typename C::value_type>,
              "Function cannot be called with parameters of initial value type, and collection value type");

using ResultType = std::invoke_result_t<F, T, typename C::value_type>;
static_assert(std::regular_invocable<F, ResultType, typename C::value_type>,
              "Function cannot be called with parameters of function result type, and collection value type");

return (0 == c.Count()) ? t : std::accumulate(c.begin(), c.end(), t, std::forward<F>(f));
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename C>
constexpr auto Take(const std::size_t count, const C& c) noexcept {
static_assert(IsCljonicCollection<C>, "Take second parameter must be a cljonic collection");

using CountType = decltype(c.Count());
auto result{Array<typename C::value_type, c.MaximumCount()>{}};
auto maxIndex{Min(count, c.Count())};
for(CountType i{0}; (i < maxIndex); ++i)
result.MConj(c[i]);
return result;
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename C>
constexpr auto Seq(const C& c) noexcept {
return Take(c.MaximumCount(), c);
}

}

} // namespace cljonic::core

namespace cljonic {

namespace core {
template <typename C>
constexpr auto TakeLast(const std::size_t count, const C& c) noexcept {
static_assert(IsCljonicCollection<C>, "TakeLast second parameter must be a cljonic collection");

using CountType = decltype(c.Count());
auto result{Array<typename C::value_type, c.MaximumCount()>{}};
auto startIndex{(c.Count() > count) ? (c.Count() - count) : 0};
for(CountType i{startIndex}; (i < c.Count()); ++i)
result.MConj(c[i]);
return result;
}

}

} // namespace cljonic::core

#endif // CLJONIC_H_
