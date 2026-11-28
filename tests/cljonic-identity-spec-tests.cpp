#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <type_traits>

#include "cljonic-test-api.hpp"

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

TEST_CASE("Identity free function operations", "[identity]") {
    using namespace cljonic;
    using namespace cljonic::core;

    TRACE_ID("entity-fields.Identity");
    TRACE_ID("invariant.Identity.IdentityAcceptsExactlyOneArgument");
    TRACE_ID("invariant.Identity.IdentityReturnsExactTypeAndValue");
    TRACE_ID("invariant.Identity.IdentitySupportsCompileTimeEvaluation");
    TRACE_ID("invariant.Identity.IdentityIsNothrow");
    TRACE_ID("invariant.Identity.IdentityIsNonMutating");
    TRACE_ID("invariant.Identity.IdentityIsNonAllocating");
    TRACE_ID("invariant.Identity.IdentityHasNoRtti");
    TRACE_ID("invariant.Identity.IdentityHasNoExceptions");
    TRACE_ID("invariant.Identity.IdentityUsesSingleThreadedExecution");
    TRACE_ID("invariant.Identity.IdentityIsReferentiallyTransparent");
    TRACE_ID("invariant.Identity.IdentityRejectsZeroAndMultipleArguments");
    TRACE_ID("invariant.Identity.IdentityRejectsNothrowCopyConstructionViolations");

    constexpr auto integer_result = identity(42);
    constexpr auto vector_result = identity(vector(1, 2, 3));
    constexpr auto map_result = identity(hash_map(1, 100));

    STATIC_REQUIRE(std::same_as<decltype(integer_result), const int>);
    STATIC_REQUIRE(integer_result == 42);
    STATIC_REQUIRE(std::same_as<decltype(vector_result), const Vector<int, 3>>);
    STATIC_REQUIRE(equal(vector_result, vector(1, 2, 3)));
    STATIC_REQUIRE(std::same_as<decltype(map_result), const Map<int, int, 1>>);
    STATIC_REQUIRE(map_result(1) == 100);
    STATIC_REQUIRE(noexcept(identity(integer_result)));

    const auto runtime_value = 7;
    const auto runtime_result = identity(runtime_value);
    REQUIRE(runtime_result == 7);
    REQUIRE(runtime_value == 7);
}
