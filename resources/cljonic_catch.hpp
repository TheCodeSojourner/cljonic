#ifndef CLJONIC_CATCH_HPP
#define CLJONIC_CATCH_HPP

#define CHECK_CLJONIC(expr)             \
    {                                   \
        auto x = (expr);                \
        DisableNoHeapMessagePrinting(); \
        CHECK(x);                       \
        EnableNoHeapMessagePrinting();  \
    }

#define CHECK_CLJONIC_NOT(expr)         \
    {                                   \
        auto x = (expr);                \
        DisableNoHeapMessagePrinting(); \
        CHECK(not x);                   \
        EnableNoHeapMessagePrinting();  \
    }

#endif // CLJONIC_CATCH_HPP
