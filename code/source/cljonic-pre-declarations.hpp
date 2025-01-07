#ifndef CLJONIC_PRE_DECLARATIONSHPP
#define CLJONIC_PRE_DECLARATIONSHPP

#include "cljonic-collection-maximum-element-count.hpp"

namespace cljonic
{

template <typename T, SizeType MaxElements>
class Array;

template <int... StartEndStep>
class Range;

template <SizeType MaxElements, typename T>
class Repeat;

template <typename T, SizeType MaxElements>
class Set;

template <SizeType MaxElements>
class String;

namespace core
{

template <typename F1, typename F2>
constexpr auto Compose(F1&& f1, F2&& f2) noexcept;

template <typename C, typename... Cs>
constexpr auto Concat(const C& c, const Cs&... cs) noexcept;

template <typename C, typename... Es>
constexpr auto Conj(const C& c, const Es&... es) noexcept;

template <typename C>
constexpr auto Count(const C& c) noexcept;

template <typename C>
constexpr auto Cycle(const C& c) noexcept;

template <typename C>
constexpr auto Dedupe(const C& c) noexcept;

template <typename F, typename C>
constexpr auto DedupeBy(F&& f, const C& c) noexcept;

template <typename C>
constexpr auto DefaultElement(const C& c) noexcept;

template <typename C>
constexpr auto Drop(const SizeType count, const C& c) noexcept;

template <typename C>
constexpr auto DropLast(const SizeType count, const C& c) noexcept;

template <typename F, typename C>
constexpr auto DropWhile(F&& f, const C& c) noexcept;

template <typename T, typename... Ts>
constexpr auto Equal(const T& t, const Ts&... ts) noexcept;

template <typename F, typename T, typename... Ts>
constexpr auto EqualBy(F&& f, const T& t, const Ts&... ts) noexcept;

template <typename F, typename C>
constexpr auto Every(F&& f, const C& c) noexcept;

template <typename F, typename C>
constexpr auto Filter(F&& f, const C& c) noexcept;

template <typename C>
constexpr auto First(const C& coll) noexcept;

template <typename T, typename... Ts>
constexpr auto Identical(const T& t, const Ts&... ts) noexcept;

template <typename T>
constexpr void* Identity(const T& t) noexcept;

template <typename F, typename C, typename... Cs>
constexpr auto Map(F&& f, const C& c, const Cs&... cs) noexcept;

template <typename F, typename... Args>
constexpr auto Partial(F&& f, Args&&... args) noexcept;

template <typename F, typename C>
constexpr auto Reduce(F&& f, const C& c) noexcept;

template <typename C>
constexpr auto Second(const C& c) noexcept;

template <typename C>
constexpr auto Seq(const C& c) noexcept;

template <typename C>
constexpr auto Size(const C& c) noexcept;

template <typename F, typename C>
constexpr auto Some(F&& f, const C& c) noexcept;

template <typename F, typename C>
constexpr auto Sort(F&& f, const C& c) noexcept;

template <typename F, typename C>
constexpr auto SortBy(F&& f, const C& c) noexcept;

template <typename F, typename C>
constexpr auto SplitBy(F&& f, const C& c) noexcept;

template <typename C>
constexpr auto Take(const SizeType count, const C& c) noexcept;

template <typename C>
constexpr auto TakeLast(const SizeType count, const C& c) noexcept;

template <typename C>
constexpr auto TakeNth(const SizeType nth, const C& c) noexcept;

template <typename F, typename C>
constexpr auto TakeWhile(F&& f, const C& c) noexcept;

} // namespace core

} // namespace cljonic

#endif // CLJONIC_PRE_DECLARATIONSHPP
