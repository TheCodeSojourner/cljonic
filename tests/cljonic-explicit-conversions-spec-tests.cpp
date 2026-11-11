#include <cfenv>
#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>

#include "cljonic-test-api.hpp"

template <typename Destination, typename Source>
concept HasGenericConversion =
    cljonic::core::conversion_detail::ValidConversion<Destination, Source, cljonic::core::conversion_detail::ExactTag>;

template <typename Destination, typename Source, typename Policy>
concept HasPolicyConversion = cljonic::core::conversion_detail::ValidConversion<Destination, Source, Policy>;

#define TRACE_ID(id_literal) INFO("trace-id: " id_literal)

TEST_CASE("Explicit conversions validate source values under named and generic contracts", "[conversions]") {
    using namespace cljonic::core;

    TRACE_ID("entity-fields.ExplicitConversions");
    TRACE_ID("invariant.ExplicitConversions.NamedNativeDestinationSurface");
    TRACE_ID("invariant.ExplicitConversions.EquivalentFormsShareContract");
    TRACE_ID("invariant.ExplicitConversions.CountIndexSizeAreBehavioralAliases");
    TRACE_ID("invariant.ExplicitConversions.OriginalSourceReachesValueValidation");
    TRACE_ID("invariant.ExplicitConversions.NumericPolicyMatrixAndExactness");
    TRACE_ID("invariant.ExplicitConversions.CharacterAndByteDomainsAreValidated");
    TRACE_ID("invariant.ExplicitConversions.PreflightMatchesConversionWithoutSideEffects");
    TRACE_ID("invariant.ExplicitConversions.NormalModeReturnsDocumentedFallback");
    TRACE_ID("invariant.ExplicitConversions.DiagnosticModeRecordsThenStops");
    TRACE_ID("invariant.ExplicitConversions.DiagnosticStateAndHandlerContract");
    TRACE_ID("invariant.ExplicitConversions.ConstantEvaluationFailureSemantics");
    TRACE_ID("invariant.ExplicitConversions.NoThrowNoHeapAndNoInvalidCast");
    TRACE_ID("invariant.ExplicitConversions.UnsupportedTypesAndPoliciesRejected");

    using namespace cljonic::core::conversion;

    STATIC_REQUIRE(HasGenericConversion<char, int>);
    STATIC_REQUIRE(HasGenericConversion<int, char>);
    STATIC_REQUIRE_FALSE(HasGenericConversion<float, char>);
    STATIC_REQUIRE_FALSE(HasGenericConversion<int, signed char>);
    STATIC_REQUIRE_FALSE(HasGenericConversion<int, unsigned char>);
    STATIC_REQUIRE_FALSE(HasGenericConversion<int, bool>);
    STATIC_REQUIRE_FALSE(HasGenericConversion<signed char, int>);
    STATIC_REQUIRE_FALSE(HasGenericConversion<char, double>);
    STATIC_REQUIRE_FALSE(HasGenericConversion<std::byte, double>);
    STATIC_REQUIRE(HasPolicyConversion<int, double, TruncateTag>);
    STATIC_REQUIRE_FALSE(HasPolicyConversion<int, double, NearestTag>);
    STATIC_REQUIRE_FALSE(HasPolicyConversion<float, int, TruncateTag>);
    STATIC_REQUIRE(HasPolicyConversion<float, int, NearestTag>);
    STATIC_REQUIRE_FALSE(HasPolicyConversion<int, int, TruncateTag>);
    STATIC_REQUIRE_FALSE(HasPolicyConversion<char, int, TruncateTag>);
    STATIC_REQUIRE_FALSE(HasPolicyConversion<int, char, TruncateTag>);
    STATIC_REQUIRE_FALSE(HasPolicyConversion<int, char, NearestTag>);

    STATIC_REQUIRE(std::same_as<decltype(to_short(short{2})), short>);
    STATIC_REQUIRE(std::same_as<decltype(to_ushort(static_cast<unsigned short>(2))), unsigned short>);
    STATIC_REQUIRE(std::same_as<decltype(to_int(2)), int>);
    STATIC_REQUIRE(std::same_as<decltype(to_uint(2U)), unsigned int>);
    STATIC_REQUIRE(std::same_as<decltype(to_long(2L)), long>);
    STATIC_REQUIRE(std::same_as<decltype(to_ulong(2UL)), unsigned long>);
    STATIC_REQUIRE(std::same_as<decltype(to_long_long(2LL)), long long>);
    STATIC_REQUIRE(std::same_as<decltype(to_ulong_long(2ULL)), unsigned long long>);
    STATIC_REQUIRE(std::same_as<decltype(to_float(2)), float>);
    STATIC_REQUIRE(std::same_as<decltype(to_double(2)), double>);
    STATIC_REQUIRE(std::same_as<decltype(to_long_double(2)), long double>);
    STATIC_REQUIRE(std::same_as<decltype(to_char(65)), char>);
    STATIC_REQUIRE(std::same_as<decltype(to_byte(65)), std::byte>);
    STATIC_REQUIRE(std::same_as<decltype(to_size(2)), std::size_t>);
    STATIC_REQUIRE(std::same_as<decltype(to_count(2)), std::size_t>);
    STATIC_REQUIRE(std::same_as<decltype(to_index(2)), std::size_t>);

    STATIC_REQUIRE(to_int(17) == to<int>(17));
    STATIC_REQUIRE(to_float(17) == to<float>(17));
    STATIC_REQUIRE(to_char(65) == to<char>(65));
    STATIC_REQUIRE(to_byte(65) == to<std::byte>(65));
    STATIC_REQUIRE(to_size(17) == to_count(17));
    STATIC_REQUIRE(to_size(17) == to_index(17));
    STATIC_REQUIRE(can_to_size(17) == can_to_count(17));
    STATIC_REQUIRE(can_to_size(17) == can_to_index(17));

    STATIC_REQUIRE(can_to_int(17));
    STATIC_REQUIRE(to_int(17) == 17);
    STATIC_REQUIRE(can_to<unsigned long long>(17));
    STATIC_REQUIRE(to<unsigned long long>(17) == 17ULL);
    STATIC_REQUIRE(can_to_int(19.0));
    STATIC_REQUIRE(to_int(19.0) == 19);
    STATIC_REQUIRE_FALSE(can_to_int(19.5));
    REQUIRE(to_int(19.5) == std::numeric_limits<int>::max());
    STATIC_REQUIRE(to_int(19.5, truncate) == 19);
    STATIC_REQUIRE(to<unsigned int>(-0.5, truncate) == 0U);
    STATIC_REQUIRE_FALSE(can_to<unsigned int>(-1));
    REQUIRE(to<unsigned int>(-1) == std::numeric_limits<unsigned int>::max());

    STATIC_REQUIRE_FALSE(can_to<float>(16777217));
    STATIC_REQUIRE(to<float>(16777217, nearest) == 16777216.0F);
    STATIC_REQUIRE(to<float>(16777219, nearest) == 16777220.0F);
    STATIC_REQUIRE(to<double>(std::numeric_limits<long long>::max(), nearest) == 9223372036854775808.0);
    STATIC_REQUIRE(to<double>(std::numeric_limits<unsigned long long>::max(), nearest) == 18446744073709551616.0);
    STATIC_REQUIRE(to<int>(2.5, truncate) == 2);
    STATIC_REQUIRE(to<int>(-2.5, truncate) == -2);
    STATIC_REQUIRE(to<float>(1.5, nearest) == 1.5F);
    STATIC_REQUIRE_FALSE(can_to<float>(std::numeric_limits<double>::infinity()));
    REQUIRE(to<double>(std::numeric_limits<double>::infinity()) == std::numeric_limits<double>::max());
    STATIC_REQUIRE_FALSE(can_to<double>(std::numeric_limits<double>::quiet_NaN()));
    STATIC_REQUIRE(to<double>(-0.0) == 0.0);
    STATIC_REQUIRE(std::signbit(to<double>(-0.0)));

    if constexpr (std::numeric_limits<float>::has_denorm == std::denorm_present) {
        constexpr double tiny = static_cast<double>(std::numeric_limits<float>::denorm_min()) / 4.0;
        STATIC_REQUIRE_FALSE(can_to<float>(tiny, nearest));
        REQUIRE(to<float>(tiny, nearest) == std::numeric_limits<float>::max());
    }

    const auto previous_rounding = std::fegetround();
    if (previous_rounding != -1 && std::fesetround(FE_UPWARD) == 0) {
        volatile int halfway = 16777219;
        const bool rounded_to_even = to<float>(halfway, nearest) == 16777220.0F;
        bool underflow_rejected = true;
        if constexpr (std::numeric_limits<float>::has_denorm == std::denorm_present) {
            volatile double tiny_source = static_cast<double>(std::numeric_limits<float>::denorm_min()) / 4.0;
            underflow_rejected = !can_to<float>(tiny_source, nearest) &&
                                 to_float(tiny_source, nearest) == std::numeric_limits<float>::max();
        }
        static_cast<void>(std::fesetround(previous_rounding));
        REQUIRE(rounded_to_even);
        REQUIRE(underflow_rejected);
    }

    STATIC_REQUIRE(to_char(65) == 'A');
    STATIC_REQUIRE(to_char('A') == 'A');
    STATIC_REQUIRE(to_char(std::byte{0x41}) == 'A');
    STATIC_REQUIRE(can_to_char(0));
    STATIC_REQUIRE(can_to_char(128) == (std::numeric_limits<char>::max() >= 128));
    REQUIRE(to_char(0) == '\0');
    STATIC_REQUIRE(to_byte(0x7f) == std::byte{0x7f});
    STATIC_REQUIRE(to_byte('A') == std::byte{0x41});
    STATIC_REQUIRE(to_byte(std::byte{0x41}) == std::byte{0x41});
    STATIC_REQUIRE_FALSE(can_to_byte(-1));
    STATIC_REQUIRE(can_to_byte(0x80));
    REQUIRE(to_byte(-1) == static_cast<std::byte>(UCHAR_MAX));
    STATIC_REQUIRE(to_byte(0x80) == std::byte{0x80});

    STATIC_REQUIRE(noexcept(to_int(1)));
    STATIC_REQUIRE(noexcept(can_to_int(1)));
    STATIC_REQUIRE(noexcept(conversion_error()));
    STATIC_REQUIRE(conversion_error() == ConversionError::none);
}

TEST_CASE("Explicit character conversions preserve native values independently of String validity", "[conversions]") {
    using namespace cljonic::core;
    TRACE_ID("invariant.ExplicitConversions.NativeCharValuesPreservePlatformRangeAndSignedness");
    TRACE_ID("invariant.ExplicitConversions.LowLevelConversionsAreIndependentOfStringCharacterValidity");

    STATIC_REQUIRE(to_int('A') == static_cast<int>('A'));
    STATIC_REQUIRE(to<int>('7') == static_cast<int>('7'));
    STATIC_REQUIRE(to_int('\0') == 0);
    STATIC_REQUIRE(to_char(0) == '\0');
    STATIC_REQUIRE(to<char>(std::byte{0}) == '\0');
    STATIC_REQUIRE(to_byte('\0') == std::byte{0});
    STATIC_REQUIRE(to_char(std::numeric_limits<char>::min()) == std::numeric_limits<char>::min());
    STATIC_REQUIRE(to_char(std::numeric_limits<char>::max()) == std::numeric_limits<char>::max());
    STATIC_REQUIRE(to_char(std::numeric_limits<char>::lowest()) == std::numeric_limits<char>::lowest());
    STATIC_REQUIRE_FALSE(character_is_valid(to_char(0)));
    STATIC_REQUIRE_FALSE(can_to_char(std::numeric_limits<unsigned long long>::max()));
    STATIC_REQUIRE(can_to_char(-1) == std::is_signed_v<char>);
    STATIC_REQUIRE(can_to_char(std::byte{UCHAR_MAX}) == !std::is_signed_v<char>);
    STATIC_REQUIRE(can_to_uint(std::numeric_limits<char>::min()) == !std::is_signed_v<char>);

    for (int code = std::numeric_limits<char>::min(); code <= std::numeric_limits<char>::max(); ++code) {
        const auto character = static_cast<char>(code);
        CAPTURE(code);
        REQUIRE(can_to_int(character));
        REQUIRE(to_int(character) == code);
        REQUIRE(to_char(code) == character);
        REQUIRE(to_char(character) == character);
        REQUIRE(to<char>(code) == to_char(code));
        REQUIRE(can_to_uint(character) == (code >= 0));
        REQUIRE(can_to_byte(character) == (code >= 0));
        if (code >= 0) {
            REQUIRE(to_byte(character) == static_cast<std::byte>(code));
        } else {
            REQUIRE(to_uint(character) == std::numeric_limits<unsigned int>::max());
            REQUIRE(to_byte(character) == std::byte{UCHAR_MAX});
        }
    }
    const auto above = static_cast<unsigned long long>(std::numeric_limits<char>::max()) + 1ULL;
    REQUIRE_FALSE(can_to_char(above));
    REQUIRE(to_char(above) == '\0');
    REQUIRE_FALSE(can_to_char(std::numeric_limits<long long>::lowest()));
    REQUIRE(to_char(std::numeric_limits<long long>::lowest()) == '\0');
}

TEST_CASE("Explicit conversions validate numeric and character values at runtime", "[conversions]") {
    using namespace cljonic::core;
    using namespace cljonic::core::conversion;

    REQUIRE(to_int(17L) == 17);
    REQUIRE(to_uint(17) == 17U);
    REQUIRE(to_int(19.0) == 19);
    REQUIRE_FALSE(can_to_int(std::numeric_limits<double>::infinity()));
    REQUIRE_FALSE(can_to_int(std::numeric_limits<double>::quiet_NaN()));
    REQUIRE_FALSE(can_to_int(std::numeric_limits<double>::max()));
    REQUIRE_FALSE(can_to_uint(-0.5));
    REQUIRE(to_uint(-0.5, truncate) == 0U);
    REQUIRE(to_int(-2.5, truncate) == -2);
    REQUIRE_FALSE(can_to_int(std::numeric_limits<double>::infinity(), truncate));
    REQUIRE_FALSE(can_to_int(std::numeric_limits<double>::max(), truncate));
    REQUIRE_FALSE(can_to_int(std::numeric_limits<double>::lowest(), truncate));
    REQUIRE_FALSE(can_to_uint(-1.0, truncate));
    REQUIRE(to_float(17) == 17.0F);
    REQUIRE_FALSE(can_to_float(16777217));
    REQUIRE(to_char(65) == 'A');
    REQUIRE(to_byte(128) == std::byte{128});
    REQUIRE_FALSE(can_to_byte(static_cast<unsigned long long>(UCHAR_MAX) + 1ULL));

    REQUIRE(to_float(1.5) == 1.5F);
    REQUIRE(to_double(1.5F) == 1.5);
    REQUIRE_FALSE(can_to_float(1.1));
    REQUIRE_FALSE(can_to_float(std::numeric_limits<double>::max()));
    REQUIRE_FALSE(can_to_float(std::numeric_limits<double>::max(), nearest));
    REQUIRE_FALSE(can_to_float(std::numeric_limits<double>::infinity(), nearest));
    REQUIRE_FALSE(can_to_float(std::numeric_limits<double>::quiet_NaN(), nearest));
    REQUIRE(std::signbit(to_float(-0.0)));
    REQUIRE_FALSE(std::signbit(to_float(0.0)));
    REQUIRE(std::signbit(to_float(-0.0, nearest)));
    REQUIRE_FALSE(std::signbit(to_float(0.0, nearest)));
}

TEST_CASE("Explicit nearest conversions round runtime boundaries and subnormals", "[conversions]") {
    using namespace cljonic::core;
    using namespace cljonic::core::conversion;

    REQUIRE(to_float(17, nearest) == 17.0F);
    REQUIRE(to_float(16777217, nearest) == 16777216.0F);
    REQUIRE(to_float(16777217U, nearest) == 16777216.0F);
    REQUIRE(to_float(16777219, nearest) == 16777220.0F);
    REQUIRE(to_float(-16777217, nearest) == -16777216.0F);
    REQUIRE(to_float(-16777219, nearest) == -16777220.0F);
    REQUIRE(to_double(std::numeric_limits<long long>::max(), nearest) == 9223372036854775808.0);
    REQUIRE(to_double(std::numeric_limits<unsigned long long>::max(), nearest) == 18446744073709551616.0);
    REQUIRE(to_double(std::numeric_limits<long long>::lowest() + 1, nearest) == -9223372036854775808.0);
    REQUIRE(to_float(33554433, nearest) == 33554432.0F);
    REQUIRE(to_float(33554435, nearest) == 33554436.0F);

    constexpr double step = static_cast<double>(std::numeric_limits<float>::epsilon());
    REQUIRE(to_float(1.0, nearest) == 1.0F);
    REQUIRE(to_float(1.0 + step / 4.0, nearest) == 1.0F);
    REQUIRE(to_float(1.0 + step * 3.0 / 4.0, nearest) == 1.0F + std::numeric_limits<float>::epsilon());
    REQUIRE(to_float(1.0 + step / 2.0, nearest) == 1.0F);
    REQUIRE(to_float(1.0 + step * 1.5, nearest) == 1.0F + 2.0F * std::numeric_limits<float>::epsilon());
    REQUIRE(to_float(-1.0 - step / 2.0, nearest) == -1.0F);
    REQUIRE(to_float(0.5 + step / 8.0, nearest) == 0.5F);

    if constexpr (std::numeric_limits<float>::has_denorm == std::denorm_present) {
        const auto smallest = std::numeric_limits<float>::denorm_min();
        const auto value = static_cast<double>(smallest);
        REQUIRE(to_float(value) == smallest);
        REQUIRE(to_float(value * 1.25, nearest) == smallest);
        REQUIRE(to_float(value * 1.75, nearest) == smallest * 2.0F);
        REQUIRE(to_float(-value * 1.25, nearest) == -smallest);
        REQUIRE_FALSE(can_to_float(value / 2.0, nearest));
        REQUIRE_FALSE(can_to_float(-value / 2.0, nearest));
    }
}

TEST_CASE("Conversion arithmetic helpers handle range sentinels and adjacent values", "[conversions]") {
    using namespace cljonic::core::conversion_detail;

    const auto maximum = std::numeric_limits<std::uintmax_t>::max();
    REQUIRE(unsigned_distance_from_bound(0, std::numeric_limits<std::uintmax_t>::digits) == maximum);
    REQUIRE(unsigned_distance_from_bound(maximum, std::numeric_limits<std::uintmax_t>::digits) == 1);
    REQUIRE(unsigned_distance_from_bound(7, 4) == 9);
    REQUIRE(distance_to_upper_bound(-1) == maximum);
    REQUIRE(distance_between_integers(-2, 3) == 5);
    REQUIRE(integer_distance(0, -1.0) == 1);
    REQUIRE(integer_distance(0U, -1.0) == maximum);
    REQUIRE(integer_distance(0, -std::numeric_limits<double>::max()) == maximum);
    REQUIRE(next_down(0.0F) == -std::numeric_limits<float>::denorm_min());
    REQUIRE(next_down(-1.0F) == std::nextafter(-1.0F, -2.0F));
    REQUIRE(next_up(-1.0F) == std::nextafter(-1.0F, 0.0F));
    REQUIRE(next_up(std::numeric_limits<float>::max()) == std::numeric_limits<float>::infinity());
    REQUIRE_FALSE(even_significand(std::numeric_limits<float>::max()));
    REQUIRE(nearest_integer_float(5, 4.0F, 8.0F) == 4.0F);
    REQUIRE(nearest_integer_float(7, 4.0F, 8.0F) == 8.0F);
}
