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

// This file was generated Wed Dec 18 11:29:40 AM MST 2024

namespace cljonic {

enum class CljonicCollectionType {
    Array,
    Range,
    Repeat,
    Set,
    String
};

}

#include <concepts>
#include <limits>

namespace cljonic {

template <typename T>
concept IsCljonicCollection = std::same_as<typename T::cljonic_collection, std::true_type>;

template <typename T>
concept IsCljonicArray = std::same_as<typename T::cljonic_collection_type,
                                      std::integral_constant<CljonicCollectionType, CljonicCollectionType::Array>>;

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

template <typename T, typename... Ts>
concept AllCljonicArrayRangeOrRepeat = (IsCljonicArrayRangeOrRepeat<T> and ... and IsCljonicArrayRangeOrRepeat<Ts>);

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

#include <cstring>

namespace cljonic {

template <typename T, typename U>
auto AreEqual(const T& t, const U& u) {
if constexpr(CString<T> and CString<U>)
return (std::strcmp(t, u) == 0);
else {
static_assert((not std::floating_point<T>) and (not std::floating_point<U>),
              "Floating point types should not be equality compared");
static_assert(std::equality_comparable_with<T, U>, "Types are not equality comparable");
return (t == u);
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
T m_elementDefault;
T m_elements[MaxElements];

public:
using cljonic_collection = std::true_type;
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Array>;
using size_type = MaxElementsType;
using value_type = T;

Array() noexcept : m_elementCount(0), m_elementDefault(T{}) {
}

explicit Array(const std::initializer_list<const T> elements) noexcept : m_elementCount(0), m_elementDefault(T{}) {
for(const auto& element : elements) {
if(m_elementCount == MaxElements)
break;
m_elements[m_elementCount++] = element;
}
}

Array(const Array& other) = default;
Array(Array&& other) = default;

const T* begin() const noexcept {
return m_elements;
}

const T* end() const noexcept {
return m_elements + m_elementCount;
}

const T& operator[](const MaxElementsType index) const noexcept {
return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
}

[[nodiscard]] MaxElementsType Count() const noexcept {
return m_elementCount;
}
};

template <typename... Args>
Array(Args...) -> Array<std::common_type_t<Args...>, sizeof...(Args)>;

} // namespace cljonic

#include <type_traits>

namespace cljonic {

class Range {
std::size_t m_elementCount;
int m_elementDefault;
int m_elementStart;
int m_elementStep;

class Iterator {
const Range& m_range;
std::size_t m_index;

public:
Iterator(const Range& range, const std::size_t index) : m_range(range), m_index(index) {
}

int operator*() const {
return m_range[m_index];
}

Iterator& operator++() {
++m_index;
return *this;
}

bool operator!=(const Iterator& other) const {
return m_index != other.m_index;
}
};

constexpr int Count(const int start, const int end, const int step) noexcept {
return ((end - start) / step) + ((((end - start) % step) == 0) ? 0 : 1);
}

constexpr void InitializeMembers(const int count, const int start, const int step) noexcept {
m_elementCount = static_cast<std::size_t>(count);
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
const int count{Count(start, end, step)};
InitializeMembers(count, start, step);
}
}

constexpr void InitializeStartEndStepWithPositiveStep(const int start, const int end, const int step) noexcept {
if(end <= start)
InitializeMembers(0, 0, step);
else {
const int count{Count(start, end, step)};
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
using cljonic_collection = std::true_type;
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Range>;
using size_type = std::size_t;
using value_type = int;

Range() noexcept
    : m_elementCount{static_cast<std::size_t>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementStep{0} {
Initialize();
}

explicit Range(const int end) noexcept
    : m_elementCount{static_cast<std::size_t>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementStep{0} {
InitializeEnd(end);
}

explicit Range(const int start, const int end) noexcept
    : m_elementCount{static_cast<std::size_t>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementStep{0} {
InitializeStartEnd(start, end);
}

explicit Range(const int start, const int end, const int step) noexcept
    : m_elementCount{static_cast<std::size_t>(0)}, m_elementDefault{0}, m_elementStart{0}, m_elementStep{0} {
InitializeStartEndStep(start, end, step);
}

Range(const Range& other) = default;
Range(Range&& other) = default;

[[nodiscard]] Iterator begin() const {
return Iterator{*this, 0};
}

[[nodiscard]] Iterator end() const {
return Iterator{*this, m_elementCount};
}

int operator[](const size_type index) const noexcept {
return ((0 == m_elementCount) or (index >= m_elementCount))
           ? m_elementDefault
           : (m_elementStart + (static_cast<int>(index) * m_elementStep));
}

[[nodiscard]] size_type Count() const noexcept {
return m_elementCount;
}
};

} // namespace cljonic

#include <type_traits>

namespace cljonic {

template <typename T>
class Repeat {
std::size_t m_elementCount;
T m_elementDefault;
T m_elementValue;

class Iterator {
const Repeat& m_repeat;
std::size_t m_index;

public:
Iterator(const Repeat& repeat, const std::size_t index) : m_repeat(repeat), m_index(index) {
}

int operator*() const {
return m_repeat[m_index];
}

Iterator& operator++() {
++m_index;
return *this;
}

bool operator!=(const Iterator& other) const {
return m_index != other.m_index;
}
};

public:
using cljonic_collection = std::true_type;
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Repeat>;
using size_type = std::size_t;
using value_type = T;

explicit Repeat(const T& t) noexcept
    : m_elementCount{std::numeric_limits<std::size_t>::max()}, m_elementDefault{T{}}, m_elementValue{t} {
}

explicit Repeat(const size_type count, const T& t) noexcept
    : m_elementCount{count}, m_elementDefault{T{}}, m_elementValue{t} {
}

Repeat(const Repeat& other) = default;
Repeat(Repeat&& other) = default;

[[nodiscard]] Iterator begin() const {
return Iterator{*this, 0};
}

[[nodiscard]] Iterator end() const {
return Iterator{*this, m_elementCount};
}

const T& operator[](const size_type index) const noexcept {
return ((m_elementCount <= 0) or (index >= m_elementCount)) ? m_elementDefault : m_elementValue;
}

[[nodiscard]] size_type Count() const noexcept {
return m_elementCount;
}
};

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
T m_elementDefault;
T m_elements[MaxElements];

bool IsUniqueElement(const T& element) const noexcept {
auto result{true};
for(MaxElementsType i = 0; (result and (i < m_elementCount)); ++i)
result = not AreEqual(element, m_elements[i]);
return result;
}

public:
using cljonic_collection = std::true_type;
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::Set>;
using size_type = MaxElementsType;
using value_type = T;

Set() noexcept : m_elementCount(0), m_elementDefault(T{}) {
}

explicit Set(const std::initializer_list<const T> elements) noexcept : m_elementCount(0), m_elementDefault(T{}) {
for(const auto& element : elements) {
if(m_elementCount == MaxElements)
break;
if(IsUniqueElement(element))
m_elements[m_elementCount++] = element;
}
}

Set(const Set& other) = default;
Set(Set&& other) = default;

const T* begin() const noexcept {
return m_elements;
}

const T* end() const noexcept {
return m_elements + m_elementCount;
}

const T& operator[](const MaxElementsType index) const noexcept {
return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
}

[[nodiscard]] MaxElementsType Count() const noexcept {
return m_elementCount;
}

bool Contains(const T& element) const noexcept {
return not IsUniqueElement(element);
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
using MaxElementsType = decltype(MaxElements);

MaxElementsType m_elementCount;
char m_elementDefault;
char m_elements[MaxElements + 1];

class Iterator {
const String& m_string;
std::size_t m_index;

public:
Iterator(const String& string, const std::size_t index) : m_string(string), m_index(index) {
}

char operator*() const {
return m_string[m_index];
}

Iterator& operator++() {
++m_index;
return *this;
}

bool operator!=(const Iterator& other) const {
return m_index != other.m_index;
}
};

public:
using cljonic_collection = std::true_type;
using cljonic_collection_type = std::integral_constant<CljonicCollectionType, CljonicCollectionType::String>;
using size_type = MaxElementsType;
using value_type = char;

String() noexcept : m_elementCount(0), m_elementDefault('\0') {
m_elements[0] = '\0';
}

explicit String(const std::initializer_list<const char> elements) noexcept
    : m_elementCount(0), m_elementDefault('\0') {
for(const auto& element : elements) {
if(m_elementCount == MaxElements)
break;
m_elements[m_elementCount++] = element;
}
m_elements[m_elementCount] = '\0';
}

explicit String(const char* c_str) noexcept : m_elementCount(0), m_elementDefault('\0') {
while((m_elementCount < MaxElements) and ('\0' != c_str[m_elementCount])) {
m_elements[m_elementCount] = c_str[m_elementCount];
m_elementCount += 1;
}
m_elements[m_elementCount] = '\0';
}

String(const String& other) = default;
String(String&& other) = default;

[[nodiscard]] Iterator begin() const {
return Iterator{*this, 0};
}

[[nodiscard]] Iterator end() const {
return Iterator{*this, m_elementCount};
}

char operator[](const MaxElementsType index) const noexcept {
return (index < m_elementCount) ? m_elements[index] : m_elementDefault;
}

[[nodiscard]] MaxElementsType Count() const noexcept {
return m_elementCount;
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

namespace cljonic {

namespace core {
template <typename T, typename... Ts>
auto Equal(const T& t, const Ts&... ts) {

if constexpr(AllCljonicCollections<T, Ts...>) {
static_assert(AllCljonicArrayRangeOrRepeat<T, Ts...> or AllSameCljonicCollectionType<T, Ts...>,
              "Cljonic collection types are not all the same, or all Array, Range or Repeat types");
static_assert(not AnyFloatingPointValueTypes<T, Ts...>,
              "Cljonic floating point collection value types should not be compared for equality");
static_assert(AllEqualityComparableValueTypes<T, Ts...>,
              "Cljonic collection value types are not all equality comparable");
if constexpr(AllCljonicSets<T, Ts...>) {
constexpr auto EqualSets = [&](const auto& c1, const auto& c2) {
using CountType = decltype(c1.Count());
auto result{c1.Count() == c2.Count()};
for(CountType i = 0; (result and (i < c1.Count())); ++i)
result = c2.Contains(c1[i]);
return result;
};
return (EqualSets(t, ts) and ...);
} else {
constexpr auto EqualCollections = [&](const auto& c1, const auto& c2) {
using CountType = decltype(c1.Count());
auto result{c1.Count() == c2.Count()};
for(CountType i = 0; (result and (i < c1.Count())); ++i)
result = AreEqual(c1[i], c2[i]);
return result;
};
return (EqualCollections(t, ts) and ...);
}
} else {
static_assert(not AnyFloatingPointTypes<T, Ts...>, "Floating point types should not be compared for equality");
static_assert(AllEqualityComparableTypes<T, Ts...>, "Not all types are equality comparable");
return (AreEqual(t, ts) and ...);
}
}

}

} // namespace cljonic::core

#endif // CLJONIC_H_
