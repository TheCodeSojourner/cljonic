#include <cstdlib>

#if defined(CLJONIC_USE_SINGLE_HEADER)
#include <cljonic.hpp>
#else
#include <cljonic-core.hpp>
#endif

namespace cljonic::core {

[[noreturn]] void conversion_failure_handler() noexcept {
    std::_Exit(conversion_error() == ConversionError::precision_loss ? 0 : 1);
}

} // namespace cljonic::core
