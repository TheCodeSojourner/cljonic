#pragma once

#include <cstddef>

#include <cljonic-assoc.hpp>
#include <cljonic-collection-map.hpp>
#include <cljonic-collection-vector.hpp>
#include <cljonic-concepts.hpp>

namespace cljonic::concepts_detail {

template <typename C, typename P>
concept AssocInRootAndPathElementAdmissible =
    is_cljonic_collection_v<C> &&
    (collection_kind_of_v<C> == collection_kind::map || collection_kind_of_v<C> == collection_kind::vector) &&
    ((collection_kind_of_v<C> == collection_kind::map && concepts::SafeImplicitConversion<typename C::key_type, P>) ||
     (collection_kind_of_v<C> == collection_kind::vector && IntegerIndexType<P>));

template <typename C, typename V>
struct assoc_in_terminal_value_admissible : std::false_type {};

template <typename K, typename M, std::size_t N, typename V>
struct assoc_in_terminal_value_admissible<cljonic::Map<K, M, N>, V>
    : std::bool_constant<concepts::SafeImplicitConversion<M, V> ||
                         ((collection_kind_of_v<M> == collection_kind::map ||
                           collection_kind_of_v<M> == collection_kind::vector) &&
                          assoc_in_terminal_value_admissible<M, V>::value)> {};

template <typename T, std::size_t N, typename V>
struct assoc_in_terminal_value_admissible<cljonic::Vector<T, N>, V>
    : std::bool_constant<concepts::SafeImplicitConversion<T, V> ||
                         ((collection_kind_of_v<T> == collection_kind::map ||
                           collection_kind_of_v<T> == collection_kind::vector) &&
                          assoc_in_terminal_value_admissible<T, V>::value)> {};

template <typename C, typename V>
inline constexpr bool assoc_in_terminal_value_admissible_v =
    assoc_in_terminal_value_admissible<std::remove_cvref_t<C>, std::remove_cvref_t<V>>::value;

template <typename C, typename P, typename V, std::size_t PathCapacity>
constexpr auto assoc_in_at(const C& collection, const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                           const V& value) -> C;

template <typename C, typename V>
constexpr auto assoc_in_map_final_at(const C& collection, const typename C::key_type& key, const V& value) -> C {
    if constexpr (requires(const C& c, const C::key_type& k, const V& v) {
                      { c.assoc(k, v) } noexcept -> std::same_as<C>;
                  }) {
        return cljonic::core::assoc(collection, key, value);
    }
    return collection; // LCOV_EXCL_LINE
}

template <typename C, typename V, typename P, std::size_t PathCapacity>
constexpr auto assoc_in_map_intermediate_at(const C& collection, const typename C::key_type& key,
                                            const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                                            const V& value) -> C {
    using MappedType = C::mapped_type;
    MappedType nested{};
    if (collection.contains(key)) {
        nested = collection(key);
    }
    if constexpr (collection_kind_of_v<MappedType> == collection_kind::map ||
                  collection_kind_of_v<MappedType> == collection_kind::vector) {
        nested = assoc_in_at<MappedType, P, V, PathCapacity>(nested, path, index + 1, value);
        return cljonic::core::assoc(collection, key, nested);
    }
    return collection;
}

template <typename C, typename V, typename P, std::size_t PathCapacity>
constexpr auto assoc_in_vector_final_at(const C& collection, std::size_t position, const V& value) -> C {
    if constexpr (requires(const C& c, const std::size_t& i, const V& v) {
                      { c.assoc(i, v) } noexcept -> std::same_as<C>;
                  }) {
        return cljonic::core::assoc(collection, position, value);
    }
    return collection;
}

template <typename C, typename V, typename P, std::size_t PathCapacity>
constexpr auto assoc_in_vector_intermediate_at(const C& collection, std::size_t position,
                                               const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                                               const V& value) -> C {
    using ElementType = C::value_type;
    ElementType nested{};
    if (position < collection.count()) {
        nested = collection(position);
    }
    if constexpr (collection_kind_of_v<ElementType> == collection_kind::map ||
                  collection_kind_of_v<ElementType> == collection_kind::vector) {
        nested = assoc_in_at<ElementType, P, V, PathCapacity>(nested, path, index + 1, value);
        return cljonic::core::assoc(collection, position, nested);
    }
    return collection;
}

template <typename C, typename P, typename V, std::size_t PathCapacity>
constexpr auto assoc_in_map_at(const C& collection, const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                               const V& value) -> C {
    static_assert(concepts::SafeImplicitConversion<typename C::key_type, P>,
                  "cljonic::assoc_in: a Map path element must safely convert to the Map key type.");
    typename C::key_type key = path(index);
    if (index + 1 == path.count()) {
        return assoc_in_map_final_at<C, V>(collection, key, value);
    }
    return assoc_in_map_intermediate_at<C, V, P, PathCapacity>(collection, key, path, index, value);
}

template <typename C, typename P, typename V, std::size_t PathCapacity>
constexpr auto assoc_in_vector_at(const C& collection, const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                                  const V& value) -> C {
    static_assert(IntegerIndexType<P>,
                  "cljonic::assoc_in: a Vector path element must be a non-boolean, non-character integer.");
    const auto normalized = try_normalize_index(path(index));
    if (!normalized || *normalized > collection.count()) {
        return collection;
    }
    if (index + 1 == path.count()) {
        return assoc_in_vector_final_at<C, V, P, PathCapacity>(collection, *normalized, value);
    }
    return assoc_in_vector_intermediate_at<C, V, P, PathCapacity>(collection, *normalized, path, index, value);
}

template <typename C, typename P, typename V, std::size_t PathCapacity>
constexpr auto assoc_in_at(const C& collection, const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                           const V& value) -> C {
    if (index >= path.count()) {
        return collection;
    }
    if constexpr (collection_kind_of_v<C> == collection_kind::map) {
        return assoc_in_map_at<C, P, V, PathCapacity>(collection, path, index, value);
    } else if constexpr (collection_kind_of_v<C> == collection_kind::vector) {
        return assoc_in_vector_at<C, P, V, PathCapacity>(collection, path, index, value);
    } else {
        static_assert(dependent_false<C>,
                      "cljonic::assoc_in: the root and every intermediate collection must be a Map or Vector.");
    }
}

} // namespace cljonic::concepts_detail

namespace cljonic::core {

/** \anchor AssocIn
 * The \b assoc_in free function updates a \ref Map "Map" or \ref Vector "Vector" through a path without mutating the
 * source. Supply exactly one path \ref Vector "Vector" followed by the final value. One path element performs the
 * corresponding root-level association; multiple path elements update nested exact \ref Map "Map" or \ref Vector
 * "Vector" shapes.
 *
 * A Map path element is a key; a Vector path element is an integer index. Existing Vector indexes replace nested
 * values and the logical count appends when capacity remains. Missing intermediate Map keys create the declared Map or
 * Vector value type. A runtime-invalid key, index, or capacity leaves the original root unchanged.
 *
\b Examples
~~~~~{.cpp}
#include "cljonic.hpp"
using namespace cljonic;
using namespace cljonic::core;

using RootMap = Map<int, int, 4>;
using RootMapPath = Vector<int, 1>;
using InnerMap = Map<int, int, 4>;
using MapToInnerMap = Map<int, InnerMap, 4>;
using NestedMapPath = Vector<int, 2>;
using InnerVector = Vector<int, 4>;
using MapToInnerVector = Map<int, InnerVector, 4>;
using RootVector = Vector<int, 4>;
using VectorIndexPath = Vector<int, 1>;
using NestedVector = Vector<InnerVector, 2>;
using NestedVectorPath = Vector<int, 2>;
using FullMap = Map<int, int, 1>;
using FullVector = Vector<int, 2>;

int main() {
  // A one-element Map path associates the root Map.
  constexpr auto root_map = assoc_in(RootMap{}, RootMapPath{1}, 42);
  static_assert(root_map(1) == 42);

  // A one-element Vector path replaces an existing root index.
  constexpr auto root_vector =
      assoc_in(RootVector{10, 20}, VectorIndexPath{1}, 99);
  static_assert(root_vector(0) == 10);
  static_assert(root_vector(1) == 99);

  // The logical-count index appends when root Vector capacity remains.
  constexpr auto appended_vector =
      assoc_in(RootVector{10, 20}, VectorIndexPath{2}, 30);
  static_assert(appended_vector(2) == 30);

  // Nested Map updates descend through the exact declared InnerMap type.
  constexpr auto nested_map_root = MapToInnerMap{};
  constexpr auto nested_map =
      assoc_in(nested_map_root, NestedMapPath{1, 2}, 42);
  constexpr auto nested_inner_map = nested_map(1);
  static_assert(nested_inner_map(2) == 42);
  constexpr auto original_inner_map = nested_map_root(1);
  static_assert(original_inner_map(2) == 0);

  // A missing Map key can create a declared InnerMap.
  constexpr auto missing_inner_map_root = MapToInnerMap{};
  constexpr auto missing_inner_map =
      assoc_in(missing_inner_map_root, NestedMapPath{1, 2}, 42);
  constexpr auto missing_inner_map_value = missing_inner_map(1);
  static_assert(missing_inner_map_value(2) == 42);

  // A missing Map key can create a declared InnerVector.
  constexpr auto nested_vector_root = MapToInnerVector{};
  constexpr auto nested_vector =
      assoc_in(nested_vector_root, NestedMapPath{1, 0}, 42);
  constexpr auto nested_inner_vector = nested_vector(1);
  static_assert(nested_inner_vector(0) == 42);

  // Existing nested Vector indexes replace values.
  constexpr auto nested_vector_root_values =
      NestedVector{InnerVector{1, 2}, InnerVector{3, 4}};
  constexpr auto nested_vector_replaced =
      assoc_in(nested_vector_root_values, NestedVectorPath{1, 0}, 42);
  constexpr auto replaced_inner_vector = nested_vector_replaced(1);
  constexpr auto original_inner_vector = nested_vector_root_values(1);
  static_assert(replaced_inner_vector(0) == 42);
  static_assert(original_inner_vector(0) == 3);

  // Runtime Map and Vector associations use normal replacement and append.
  const auto runtime_map = assoc(RootMap{}, 1, 10);
  const auto runtime_map_updated = assoc_in(runtime_map, RootMapPath{1}, 42);
  const auto runtime_vector = RootVector{10, 20};
  const auto runtime_vector_replaced =
      assoc_in(runtime_vector, VectorIndexPath{1}, 99);
  const auto runtime_vector_appended =
      assoc_in(runtime_vector, VectorIndexPath{2}, 30);

  // Invalid Map keys and Vector indexes leave the source unchanged.
  const auto full_map = assoc(FullMap{}, 1, 10);
  const auto invalid_full_map_key = assoc_in(full_map, RootMapPath{2}, 20);
  const auto invalid_full_map_unchanged =
      invalid_full_map_key(2) == 0 && invalid_full_map_key(1) == 10;

  const auto full_vector = FullVector{10, 20};
  const auto invalid_negative_index =
      assoc_in(full_vector, VectorIndexPath{-1}, 99);
  const auto invalid_past_end = assoc_in(full_vector, VectorIndexPath{3}, 99);
  const auto invalid_full_append =
      assoc_in(full_vector, VectorIndexPath{2}, 99);

  return runtime_map_updated(1) == 42 && runtime_vector_replaced(1) == 99 &&
                 runtime_vector_appended(2) == 30 &&
                 invalid_full_map_unchanged &&
                 invalid_negative_index(0) == 10 &&
                 invalid_negative_index(1) == 20 && invalid_past_end(0) == 10 &&
                 invalid_past_end(1) == 20 && invalid_full_append(0) == 10 &&
                 invalid_full_append(1) == 20 && full_vector(0) == 10 &&
                 full_vector(1) == 20
             ? 0
             : 1;
}
~~~~~
 */
template <typename C, typename P, std::size_t PathCapacity, typename V>
    requires(cljonic::concepts_detail::AssocInRootAndPathElementAdmissible<C, P> &&
             cljonic::concepts_detail::assoc_in_terminal_value_admissible_v<C, V> && PathCapacity > 0)
[[nodiscard]] constexpr auto assoc_in(const C& collection, const cljonic::Vector<P, PathCapacity>& path,
                                      const V& value) noexcept -> C {
    if (path.count() == 0) {
        return collection;
    }
    return cljonic::concepts_detail::assoc_in_at<C, P, V, PathCapacity>(collection, path, 0, value);
}

template <typename C, typename P, std::size_t PathCapacity, typename V>
    requires(!cljonic::concepts_detail::AssocInRootAndPathElementAdmissible<C, P> ||
             !cljonic::concepts_detail::assoc_in_terminal_value_admissible_v<C, V> || PathCapacity == 0)
constexpr auto assoc_in([[maybe_unused]] const C& collection,
                        [[maybe_unused]] const cljonic::Vector<P, PathCapacity>& path,
                        [[maybe_unused]] const V& value) noexcept -> void {
    static_assert(cljonic::concepts_detail::dependent_false<C, P, V>,
                  "cljonic::assoc_in: expected a Map or Vector root, a nonempty Vector path, exact path elements, and "
                  "a safely admissible final value.");
}

template <typename C, typename P, typename V>
constexpr auto assoc_in([[maybe_unused]] const C& collection, [[maybe_unused]] const P& path,
                        [[maybe_unused]] const V& value) noexcept -> void
    requires(!cljonic::concepts_detail::is_cljonic_collection_v<P>)
{
    static_assert(cljonic::concepts_detail::dependent_false<C, P, V>,
                  "cljonic::assoc_in: path must be exactly one cljonic::Vector.");
}

} // namespace cljonic::core
