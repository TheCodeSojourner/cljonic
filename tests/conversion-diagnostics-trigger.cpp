#if defined(CLJONIC_USE_SINGLE_HEADER)
#include <cljonic.hpp>
#else
#include <cljonic-core.hpp>
#endif

void trigger_conversion_failure() noexcept {
    using cljonic::core::can_to_int;
    using cljonic::core::conversion_error;
    using cljonic::core::ConversionError;
    using cljonic::core::to_int;

    if (conversion_error() != ConversionError::none || can_to_int(1.5) || conversion_error() != ConversionError::none ||
        to_int(1) != 1 || conversion_error() != ConversionError::none) {
        return;
    }

    static_cast<void>(to_int(1.5));
}
