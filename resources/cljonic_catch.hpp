#ifndef CLJONIC_CATCH_HPP
#define CLJONIC_CATCH_HPP

#define CHECK_CLJONIC(expr)             \
    {                                   \
        auto _x = (expr);               \
        DisableNoHeapMessagePrinting(); \
        CHECK(_x);                      \
        EnableNoHeapMessagePrinting();  \
    }

#define CHECK_CLJONIC_NOT(expr)         \
    {                                   \
        auto _x = (expr);               \
        DisableNoHeapMessagePrinting(); \
        CHECK(not _x);                  \
        EnableNoHeapMessagePrinting();  \
    }

#endif // CLJONIC_CATCH_HPP
