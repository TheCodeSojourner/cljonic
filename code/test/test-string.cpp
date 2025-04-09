#include <cstring>
#include <string>
#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-string.hpp"

using namespace cljonic;

SCENARIO("String", "[CljonicString]")
{
    EnableNoHeapMessagePrinting();

    TYPE_CLJONIC_CHAR hello[10];
    hello[0] = 'H';
    hello[1] = 'e';
    hello[2] = 'l';
    hello[3] = 'l';
    hello[4] = 'o';
    hello[5] = '\0';

    {
        constexpr auto s{String<10>{}};
        CHECK_CLJONIC(0 == s.Count());
        CHECK_CLJONIC('\0' == *s.begin());
    }

    {
        constexpr auto s{String<10>{"Hello"}};
        CHECK_CLJONIC(5 == s.Count());
        auto i{0};
        for (const auto& c : s)
            CHECK_CLJONIC(hello[i++] == c);
        CHECK_CLJONIC(5 == i);
        CHECK_CLJONIC(0 == std::strcmp(hello, s.c_str()));
    }

    {
        constexpr auto s{String<5>{"Hello"}};
        CHECK_CLJONIC(5 == s.Count());
        auto i{0};
        for (const auto& c : s)
            CHECK_CLJONIC(hello[i++] == c);
        CHECK_CLJONIC(5 == i);
        CHECK_CLJONIC(0 == std::strcmp(hello, s.c_str()));
    }

    {
        constexpr auto s{String<4>{"Hello"}};
        CHECK_CLJONIC(4 == s.Count());
        auto i{0};
        for (const auto& c : s)
            CHECK_CLJONIC(hello[i++] == c);
        CHECK_CLJONIC(4 == i);
        CHECK_CLJONIC(0 == std::strncmp(hello, s.c_str(), 4));
    }

    {
        constexpr auto s{String{"Hello"}};
        CHECK_CLJONIC(5 == s.Count());
        auto i{0};
        for (const auto& c : s)
            CHECK_CLJONIC(hello[i++] == c);
        CHECK_CLJONIC(5 == i);
        CHECK_CLJONIC(0 == std::strcmp(hello, s.c_str()));
    }

    {
        constexpr auto s{String<10>{'H', 'e', 'l', 'l', 'o'}};
        CHECK_CLJONIC(5 == s.Count());
        auto i{0};
        for (const auto& c : s)
            CHECK_CLJONIC(hello[i++] == c);
        CHECK_CLJONIC(5 == i);
        CHECK_CLJONIC(0 == std::strcmp(hello, s.c_str()));
    }

    {
        constexpr auto s{String<5>{'H', 'e', 'l', 'l', 'o'}};
        CHECK_CLJONIC(5 == s.Count());
        auto i{0};
        for (const auto& c : s)
            CHECK_CLJONIC(hello[i++] == c);
        CHECK_CLJONIC(5 == i);
        CHECK_CLJONIC(0 == std::strcmp(hello, s.c_str()));
    }

    {
        constexpr auto s{String<4>{'H', 'e', 'l', 'l', 'o'}};
        CHECK_CLJONIC(4 == s.Count());
        auto i{0};
        for (const auto& c : s)
            CHECK_CLJONIC(hello[i++] == c);
        CHECK_CLJONIC(4 == i);
        CHECK_CLJONIC(0 == std::strncmp(hello, s.c_str(), 4));
    }

    {
        constexpr auto s{String{'H', 'e', 'l', 'l', 'o'}};
        CHECK_CLJONIC(5 == s.Count());
        auto i{0};
        for (const auto& c : s)
            CHECK_CLJONIC(hello[i++] == c);
        CHECK_CLJONIC(5 == i);
        CHECK_CLJONIC(0 == std::strcmp(hello, s.c_str()));
    }

    {
        const auto s{String{hello}};
        CHECK_CLJONIC(5 == s.Count());
        auto i{0};
        for (const auto& c : s)
            CHECK_CLJONIC(hello[i++] == c);
        CHECK_CLJONIC(5 == i);
        CHECK_CLJONIC(0 == std::strcmp(hello, s.c_str()));
    }

    {
        // a String is a function of its index
        constexpr auto s{String{"1234"}};
        CHECK_CLJONIC('1' == s(0));
        CHECK_CLJONIC('2' == s(1));
        CHECK_CLJONIC('3' == s(2));
        CHECK_CLJONIC('4' == s(3));
        CHECK_CLJONIC('\0' == s(4)); // index out-of-bounds so return default element, which is '\0' in this case
    }
    DisableNoHeapMessagePrinting();
}
