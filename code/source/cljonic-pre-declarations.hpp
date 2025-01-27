#ifndef CLJONIC_PRE_DECLARATIONSHPP
#define CLJONIC_PRE_DECLARATIONSHPP

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

constexpr auto Concat() noexcept;

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

template <typename T>
constexpr auto Inc(const T t) noexcept;

template <typename C, typename T>
constexpr auto IndexOf(const C& c, const T& t) noexcept;

template <typename F, typename C, typename T>
constexpr auto IndexOfBy(F&& f, const C& c, const T& t) noexcept;

constexpr auto Interleave() noexcept;

template <typename C, typename... Cs>
constexpr auto Interleave(const C& c, const Cs&... cs) noexcept;

template <typename T, typename C>
constexpr auto Interpose(const T& t, const C& c) noexcept;

template <typename T, typename... Ts>
constexpr auto IsDistinct(const T& t, const Ts&... ts) noexcept;

template <typename F, typename T, typename... Ts>
constexpr auto IsDistinctBy(F&& f, const T& t, const Ts&... ts) noexcept;

template <typename C>
constexpr auto IsEmpty(const C& c) noexcept;

template <typename C>
constexpr auto IsFull(const C& c) noexcept;

template <typename C>
constexpr auto Last(const C& c) noexcept;

template <typename C, typename T>
constexpr auto LastIndexOf(const C& c, const T& t) noexcept;

template <typename F, typename C, typename T>
constexpr auto LastIndexOfBy(F&& f, const C& c, const T& t) noexcept;

template <typename F, typename C, typename... Cs>
constexpr auto Map(F&& f, const C& c, const Cs&... cs) noexcept;

template <typename T, typename... Ts>
constexpr auto Max(const T& t, const Ts&... ts) noexcept;

template <typename F, typename T, typename... Ts>
constexpr auto MaxBy(F&& f, const T& t, const Ts&... ts) noexcept;

template <typename T, typename... Ts>
constexpr auto Min(const T& t, const Ts&... ts) noexcept;

template <typename F, typename T, typename... Ts>
constexpr auto MinBy(F&& f, const T& t, const Ts&... ts) noexcept;

template <typename F, typename C>
constexpr auto NotAny(F&& f, const C& c) noexcept;

template <typename F, typename C>
constexpr auto NotEvery(F&& f, const C& c) noexcept;

template <typename C>
constexpr auto Nth(const C& c, const SizeType index) noexcept;

template <typename C, typename T>
constexpr auto Nth(const C& c, const SizeType index, const T& t) noexcept;

template <typename F, typename... Args>
constexpr auto Partial(F&& f, Args&&... args) noexcept;

template <typename F, typename C>
constexpr auto Reduce(F&& f, const C& c) noexcept;

template <typename F, typename C>
constexpr auto Remove(F&& f, const C& c) noexcept;

template <typename C1, typename C2>
constexpr auto Replace(const C1& c1, const C2& c2) noexcept;

template <typename C>
constexpr auto Reverse(const C& c) noexcept;

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

template <typename C>
constexpr auto SplitAt(const SizeType count, const C& c) noexcept;

template <typename F, typename C>
constexpr auto SplitWith(F&& f, const C& c) noexcept;

template <typename C>
constexpr auto Subs(const C& c, const SizeType start, const SizeType end) noexcept;

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
