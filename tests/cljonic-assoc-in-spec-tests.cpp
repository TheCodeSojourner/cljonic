#include <catch2/catch_test_macros.hpp>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

TEST_CASE("AssocIn free function operations", "[assoc-in]") {
    using cljonic::Map;
    using cljonic::Vector;
    using cljonic::core::assoc;
    using cljonic::core::assoc_in;
    using cljonic::core::can_assoc_in;
    using cljonic::core::count;
    using cljonic::core::hash_map;
    using cljonic::core::vector;

    TRACE_ID("entity-fields.AssocIn");
    TRACE_ID("invariant.AssocIn.RootIsMapOrVector");
    TRACE_ID("invariant.AssocIn.PathIsVector");
    TRACE_ID("invariant.AssocIn.PathIsNonempty");
    TRACE_ID("invariant.AssocIn.PathElementIsExact");
    TRACE_ID("invariant.AssocIn.IntermediateValuesAreAssociative");
    TRACE_ID("invariant.AssocIn.MissingMapKeyCreatesDeclaredIntermediate");
    TRACE_ID("invariant.AssocIn.VectorExistingIndexReplaces");
    TRACE_ID("invariant.AssocIn.VectorAppendIndexAppendsWhenCapacityRemains");
    TRACE_ID("invariant.AssocIn.InvalidRuntimePathReturnsOriginalRoot");
    TRACE_ID("invariant.AssocIn.InvalidRuntimePathCanAssocInReturnsFalse");
    TRACE_ID("invariant.AssocIn.CompileTimeInvalidSurfaceHasTargetedDiagnostic");
    TRACE_ID("invariant.AssocIn.CanAssocInIsCumulative");
    TRACE_ID("invariant.AssocIn.ResultIsDistinctRootValue");
    TRACE_ID("invariant.AssocIn.SourceIsPreserved");
    TRACE_ID("invariant.AssocIn.ConstexprNoexceptNoHeapNoRtti");

    using InnerMap = Map<int, int, 4>;
    constexpr InnerMap inner{};
    constexpr auto nested_map = assoc_in(Map<int, InnerMap, 4>{}, vector<int, 2>(1, 2), 42);
    STATIC_REQUIRE(nested_map(1, InnerMap{})(2, 0) == 42);
    STATIC_REQUIRE(can_assoc_in(Map<int, InnerMap, 4>{}, vector<int, 2>(1, 2), 42));

    constexpr auto existing_inner = InnerMap{};
    constexpr auto existing_nested_root = assoc(Map<int, InnerMap, 4>{}, 1, existing_inner);
    constexpr auto existing_nested = assoc_in(existing_nested_root, vector<int, 2>(1, 2), 42);
    STATIC_REQUIRE(existing_nested(1, InnerMap{})(2, 0) == 42);
    STATIC_REQUIRE(can_assoc_in(existing_nested_root, vector<int, 2>(1, 2), 42));

    constexpr auto scalar_root = assoc(Map<int, int, 4>{}, 1, 10);
    constexpr auto scalar_intermediate = assoc_in(scalar_root, vector<int, 2>(1, 2), 42);
    STATIC_REQUIRE(scalar_intermediate(1) == 10);
    STATIC_REQUIRE_FALSE(can_assoc_in(scalar_root, vector<int, 2>(1, 2), 42));

    constexpr auto empty_path_result = assoc_in(scalar_root, Vector<int, 4>{}, 42);
    STATIC_REQUIRE(empty_path_result(1) == 10);
    STATIC_REQUIRE_FALSE(can_assoc_in(scalar_root, Vector<int, 4>{}, 42));

    volatile int runtime_nested_key_raw = 1;
    volatile int runtime_nested_value_raw = 42;
    const int runtime_nested_key = runtime_nested_key_raw;
    const int runtime_nested_value = runtime_nested_value_raw;
    const auto runtime_nested_root = assoc(Map<int, InnerMap, 4>{}, runtime_nested_key, existing_inner);
    const auto runtime_nested =
        assoc_in(runtime_nested_root, vector<int, 2>(runtime_nested_key, 2), runtime_nested_value);
    REQUIRE(runtime_nested(runtime_nested_key, InnerMap{})(2, 0) == 42);
    REQUIRE(can_assoc_in(runtime_nested_root, vector<int, 2>(runtime_nested_key, 2), runtime_nested_value));

    const auto runtime_scalar_root = assoc(Map<int, int, 4>{}, 1, 10);
    const auto runtime_scalar = assoc_in(runtime_scalar_root, vector<int, 2>(1, 2), runtime_nested_value);
    REQUIRE(runtime_scalar(1) == 10);
    REQUIRE_FALSE(can_assoc_in(runtime_scalar_root, vector<int, 2>(1, 2), runtime_nested_value));

    const auto runtime_empty = assoc_in(runtime_scalar_root, Vector<int, 4>{}, runtime_nested_value);
    REQUIRE(runtime_empty(1) == 10);
    REQUIRE_FALSE(can_assoc_in(runtime_scalar_root, Vector<int, 4>{}, runtime_nested_value));

    const auto internal_root = Map<int, InnerMap, 4>{};
    const auto internal_path = vector<int, 2>(1, 2);
    const auto internal_at_end =
        cljonic::concepts_detail::assoc_in_at(internal_root, internal_path, internal_path.count(), 42);
    REQUIRE(internal_at_end(1, InnerMap{}) == InnerMap{});

    const auto inadmissible_final = cljonic::concepts_detail::assoc_in_map_final_at(internal_root, 1, 42);
    REQUIRE(inadmissible_final(1, InnerMap{}) == InnerMap{});

    volatile int inadmissible_key_raw = 1;
    volatile int inadmissible_value_raw = 42;
    const int inadmissible_key = inadmissible_key_raw;
    const int inadmissible_value = inadmissible_value_raw;
    const auto inadmissible_can_final =
        cljonic::concepts_detail::can_assoc_in_map_final_at(internal_root, inadmissible_key, inadmissible_value);
    REQUIRE_FALSE(inadmissible_can_final);

    const auto full_inner = assoc(assoc(Map<int, int, 2>{}, 1, 10), 2, 20);
    const auto full_root = assoc(Map<int, Map<int, int, 2>, 4>{}, 1, full_inner);
    REQUIRE_FALSE(can_assoc_in(full_root, vector<int, 2>(1, 3), 42));

    const auto can_internal_at_end =
        cljonic::concepts_detail::can_assoc_in_at(internal_root, internal_path, internal_path.count(), 42);
    REQUIRE_FALSE(can_internal_at_end);

    using InnerVector = Vector<int, 4>;
    constexpr auto nested_vectors =
        assoc_in(Vector<InnerVector, 2>{InnerVector{1, 2}, InnerVector{3, 4}}, vector<int, 2>(1, 0), 42);
    STATIC_REQUIRE(nested_vectors(1, Vector<int, 4>{})(0, 0) == 42);
    STATIC_REQUIRE(nested_vectors(0, Vector<int, 4>{})(0, 0) == 1);

    constexpr auto invalid_negative = assoc_in(Vector<int, 2>{10, 20}, vector<int, 1>(-1), 99);
    STATIC_REQUIRE(invalid_negative(0) == 10);
    STATIC_REQUIRE(invalid_negative(1) == 20);
    STATIC_REQUIRE_FALSE(can_assoc_in(Vector<int, 2>{10, 20}, vector<int, 1>(-1), 99));

    constexpr auto invalid_past_end = assoc_in(Vector<int, 2>{10, 20}, vector<int, 1>(3), 99);
    STATIC_REQUIRE(invalid_past_end(0) == 10);
    STATIC_REQUIRE(invalid_past_end(1) == 20);
    STATIC_REQUIRE_FALSE(can_assoc_in(Vector<int, 2>{10, 20}, vector<int, 1>(3), 99));

    constexpr auto append = assoc_in(Vector<int, 4>{10, 20}, vector<int, 1>(2), 99);
    STATIC_REQUIRE(append(2) == 99);
    STATIC_REQUIRE(can_assoc_in(Vector<int, 4>{10, 20}, vector<int, 1>(2), 99));

    volatile int runtime_key_raw = 1;
    volatile int runtime_value_raw = 55;
    const int runtime_key = runtime_key_raw;
    const int runtime_value = runtime_value_raw;
    const auto runtime_root = Map<int, Map<int, int, 4>, 4>{};
    const auto runtime_path = vector<int, 2>(runtime_key, 2);
    const auto runtime_result = assoc_in(runtime_root, runtime_path, runtime_value);
    REQUIRE(runtime_result(runtime_key, Map<int, int, 4>{})(2, 0) == 55);
    REQUIRE(can_assoc_in(runtime_root, runtime_path, runtime_value));

    STATIC_REQUIRE(requires { assoc_in(Map<int, int, 4>{}, vector<int, 1>(1), 10); });
    STATIC_REQUIRE(requires { can_assoc_in(Map<int, int, 4>{}, vector<int, 1>(1), 10); });
}
