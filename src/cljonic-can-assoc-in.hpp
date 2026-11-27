#pragma once

#include <cstddef>

#include <cljonic-assoc-in.hpp>
#include <cljonic-can-assoc.hpp>
#include <cljonic-concepts.hpp>

namespace cljonic::concepts_detail {

template <typename C, typename P, typename V, std::size_t PathCapacity>
constexpr auto can_assoc_in_at(const C& collection, const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                               const V& value) -> bool;

template <typename C, typename V>
constexpr auto can_assoc_in_map_final_at(const C& collection, const typename C::key_type& key, const V& value) -> bool {
    if constexpr (requires(const C& c, const C::key_type& k, const V& v) {
                      { c.can_assoc(k, v) } noexcept -> std::same_as<bool>;
                  }) {
        return collection.can_assoc(key, value);
    }
    return false; // LCOV_EXCL_LINE
}

template <typename C, typename V, typename P, std::size_t PathCapacity>
constexpr auto can_assoc_in_map_nested(const C& collection, const typename C::key_type& key) {
    using MappedType = C::mapped_type;
    if (collection.contains(key)) {
        return collection(key);
    }
    return MappedType{};
}

template <typename C, typename V, typename P, std::size_t PathCapacity>
constexpr auto can_assoc_in_map_intermediate_at(const C& collection, const typename C::key_type& key,
                                                const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                                                const V& value) -> bool {
    using MappedType = C::mapped_type;
    MappedType nested = can_assoc_in_map_nested<C, V, P, PathCapacity>(collection, key);
    if constexpr (collection_kind_of_v<MappedType> == collection_kind::map ||
                  collection_kind_of_v<MappedType> == collection_kind::vector) {
        if (!can_assoc_in_at<MappedType, P, V, PathCapacity>(nested, path, index + 1, value)) {
            return false;
        }
        const auto updated = assoc_in_at<MappedType, P, V, PathCapacity>(nested, path, index + 1, value);
        return collection.can_assoc(key, updated);
    }
    return false;
}

template <typename C, typename V, typename P, std::size_t PathCapacity>
constexpr auto can_assoc_in_vector_final_at(const C& collection, std::size_t position, const V& value) -> bool {
    if constexpr (requires(const C& c, const std::size_t& i, const V& v) {
                      { c.can_assoc(i, v) } noexcept -> std::same_as<bool>;
                  }) {
        return collection.can_assoc(position, value);
    }
    return false;
}

template <typename C, typename V, typename P, std::size_t PathCapacity>
constexpr auto can_assoc_in_vector_nested(const C& collection, std::size_t position) {
    using ElementType = C::value_type;
    if (position < collection.count()) {
        return collection(position);
    }
    return ElementType{};
}

template <typename C, typename V, typename P, std::size_t PathCapacity>
constexpr auto can_assoc_in_vector_intermediate_at(const C& collection, std::size_t position,
                                                   const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                                                   const V& value) -> bool {
    using ElementType = C::value_type;
    ElementType nested = can_assoc_in_vector_nested<C, V, P, PathCapacity>(collection, position);
    if constexpr (collection_kind_of_v<ElementType> == collection_kind::map ||
                  collection_kind_of_v<ElementType> == collection_kind::vector) {
        if (!can_assoc_in_at<ElementType, P, V, PathCapacity>(nested, path, index + 1, value)) {
            return false;
        }
        const auto updated = assoc_in_at<ElementType, P, V, PathCapacity>(nested, path, index + 1, value);
        return collection.can_assoc(position, updated);
    }
    return false;
}

template <typename C, typename P, typename V, std::size_t PathCapacity>
constexpr auto can_assoc_in_map_at(const C& collection, const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                                   const V& value) -> bool {
    static_assert(concepts::SafeImplicitConversion<typename C::key_type, P>,
                  "cljonic::can_assoc_in: a Map path element must safely convert to the Map key type.");
    typename C::key_type key = path(index);
    if (index + 1 == path.count()) {
        return can_assoc_in_map_final_at<C, V>(collection, key, value);
    }
    return can_assoc_in_map_intermediate_at<C, V, P, PathCapacity>(collection, key, path, index, value);
}

template <typename C, typename P, typename V, std::size_t PathCapacity>
constexpr auto can_assoc_in_vector_at(const C& collection, const cljonic::Vector<P, PathCapacity>& path,
                                      std::size_t index, const V& value) -> bool {
    static_assert(IntegerIndexType<P>,
                  "cljonic::can_assoc_in: a Vector path element must be a non-boolean, non-character integer.");
    const auto normalized = try_normalize_index(path(index));
    if (!normalized || *normalized > collection.count()) {
        return false;
    }
    if (index + 1 == path.count()) {
        return can_assoc_in_vector_final_at<C, V, P, PathCapacity>(collection, *normalized, value);
    }
    return can_assoc_in_vector_intermediate_at<C, V, P, PathCapacity>(collection, *normalized, path, index, value);
}

template <typename C, typename P, typename V, std::size_t PathCapacity>
constexpr auto can_assoc_in_at(const C& collection, const cljonic::Vector<P, PathCapacity>& path, std::size_t index,
                               const V& value) -> bool {
    if (index >= path.count()) {
        return false;
    }
    if constexpr (collection_kind_of_v<C> == collection_kind::map) {
        return can_assoc_in_map_at<C, P, V, PathCapacity>(collection, path, index, value);
    } else if constexpr (collection_kind_of_v<C> == collection_kind::vector) {
        return can_assoc_in_vector_at<C, P, V, PathCapacity>(collection, path, index, value);
    } else {
        static_assert(dependent_false<C>,
                      "cljonic::can_assoc_in: the root and every intermediate collection must be a Map or Vector.");
    }
}

} // namespace cljonic::concepts_detail

namespace cljonic::core {

/** \anchor CanAssocIn
 * The \b can_assoc_in free function reports whether one \ref AssocIn "assoc_in" path can be applied cumulatively. One
 * path element can preflight a root-level Map or Vector association. Additional path elements preflight nested exact
 * Map/Vector values. It checks every key, index, capacity, and final value against the state produced by earlier
 * levels. It returns false for an invalid runtime path or full capacity and never mutates the source.
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
using FullMap = Map<int, int, 1>;
using FullVector = Vector<int, 2>;

int main() {
  // A one-element Map path preflights the root Map.
  constexpr auto root_map = RootMap{};
  static_assert(can_assoc_in(root_map, RootMapPath{1}, 42));

  // Nested Map and nested Vector paths preflight their exact intermediate
  // shapes.
  constexpr auto nested_map_root = MapToInnerMap{};
  static_assert(can_assoc_in(nested_map_root, NestedMapPath{1, 2}, 42));

  constexpr auto nested_vector_root = MapToInnerVector{};
  static_assert(can_assoc_in(nested_vector_root, NestedMapPath{1, 0}, 42));

  // Existing Vector indexes can replace, and the logical-count index can
  // append.
  constexpr auto root_vector = RootVector{10, 20};
  static_assert(can_assoc_in(root_vector, VectorIndexPath{1}, 99));
  static_assert(can_assoc_in(root_vector, VectorIndexPath{2}, 30));

  // Runtime normal replacement and append preflights succeed.
  const auto runtime_map = assoc(RootMap{}, 1, 10);
  const auto runtime_map_ok = can_assoc_in(runtime_map, RootMapPath{1}, 42);
  const auto runtime_vector = RootVector{10, 20};
  const auto runtime_replace_ok =
      can_assoc_in(runtime_vector, VectorIndexPath{1}, 99);
  const auto runtime_append_ok =
      can_assoc_in(runtime_vector, VectorIndexPath{2}, 30);

  // Runtime invalid Map/Vector paths fail preflight.
  const auto full_map = assoc(FullMap{}, 1, 10);
  const auto full_vector = FullVector{10, 20};
  const auto invalid_full_map_key = can_assoc_in(full_map, RootMapPath{2}, 20);
  const auto invalid_negative_index =
      can_assoc_in(full_vector, VectorIndexPath{-1}, 99);
  const auto invalid_past_end =
      can_assoc_in(full_vector, VectorIndexPath{3}, 99);
  const auto invalid_full_append =
      can_assoc_in(full_vector, VectorIndexPath{2}, 99);

  return runtime_map_ok && runtime_replace_ok && runtime_append_ok &&
                 !invalid_full_map_key && !invalid_negative_index &&
                 !invalid_past_end && !invalid_full_append
             ? 0
             : 1;
}
~~~~~
 */
template <typename C, typename P, std::size_t PathCapacity, typename V>
    requires(cljonic::concepts_detail::AssocInRootAndPathElementAdmissible<C, P> &&
             cljonic::concepts_detail::assoc_in_terminal_value_admissible_v<C, V> && PathCapacity > 0)
[[nodiscard]] constexpr auto can_assoc_in(const C& collection, const cljonic::Vector<P, PathCapacity>& path,
                                          const V& value) noexcept -> bool {
    if (path.count() == 0) {
        return false;
    }
    return cljonic::concepts_detail::can_assoc_in_at<C, P, V, PathCapacity>(collection, path, 0, value);
}

template <typename C, typename P, std::size_t PathCapacity, typename V>
    requires(!cljonic::concepts_detail::AssocInRootAndPathElementAdmissible<C, P> ||
             !cljonic::concepts_detail::assoc_in_terminal_value_admissible_v<C, V> || PathCapacity == 0)
constexpr auto can_assoc_in([[maybe_unused]] const C& collection,
                            [[maybe_unused]] const cljonic::Vector<P, PathCapacity>& path,
                            [[maybe_unused]] const V& value) noexcept -> bool {
    static_assert(cljonic::concepts_detail::dependent_false<C, P, V>,
                  "cljonic::can_assoc_in: expected a Map or Vector root, a nonempty Vector path, exact path elements, "
                  "and a safely admissible final value.");
    return false;
}

template <typename C, typename P, typename V>
constexpr auto can_assoc_in([[maybe_unused]] const C& collection, [[maybe_unused]] const P& path,
                            [[maybe_unused]] const V& value) noexcept -> bool
    requires(!cljonic::concepts_detail::is_cljonic_collection_v<P>)
{
    static_assert(cljonic::concepts_detail::dependent_false<C, P, V>,
                  "cljonic::can_assoc_in: path must be exactly one cljonic::Vector.");
    return false;
}

} // namespace cljonic::core
