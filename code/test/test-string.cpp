#include <string>
#include "catch.hpp"
#include "cljonic-string.hpp"

using namespace cljonic;

SCENARIO("String", "[CljonicString]")
{
    char s[10];
    s[0] = 'H';
    s[1] = 'e';
    s[2] = 'l';
    s[3] = 'l';
    s[4] = 'o';
    s[5] = '\0';

    constexpr auto s0{String<10>{}};
    constexpr auto s1{String<10>{"Hello"}};
    constexpr auto s2{String<5>{"Hello"}};
    constexpr auto s3{String<4>{"Hello"}};
    constexpr auto s4{String{"Hello"}};
    constexpr auto s5{String<10>{'H', 'e', 'l', 'l', 'o'}};
    constexpr auto s6{String<5>{'H', 'e', 'l', 'l', 'o'}};
    constexpr auto s7{String<4>{'H', 'e', 'l', 'l'}};
    constexpr auto s8{String{'H', 'e', 'l', 'l', 'o'}};
    const auto s9{String{s}};

    CHECK(0 == s0.Count());
    CHECK(5 == s1.Count());
    CHECK(5 == s2.Count());
    CHECK(4 == s3.Count());
    CHECK(5 == s4.Count());
    CHECK(5 == s5.Count());
    CHECK(5 == s6.Count());
    CHECK(4 == s7.Count());
    CHECK(5 == s8.Count());
    CHECK(5 == s9.Count());

    CHECK('\0' == s0[0]);

    CHECK('H' == s1[0]);
    CHECK('e' == s1[1]);
    CHECK('l' == s1[2]);
    CHECK('l' == s1[3]);
    CHECK('o' == s1[4]);
    CHECK('\0' == s1[5]);

    CHECK('H' == s2[0]);
    CHECK('e' == s2[1]);
    CHECK('l' == s2[2]);
    CHECK('l' == s2[3]);
    CHECK('o' == s2[4]);
    CHECK('\0' == s2[5]);

    CHECK('H' == s3[0]);
    CHECK('e' == s3[1]);
    CHECK('l' == s3[2]);
    CHECK('l' == s3[3]);
    CHECK('\0' == s3[4]);

    CHECK('H' == s4[0]);
    CHECK('e' == s4[1]);
    CHECK('l' == s4[2]);
    CHECK('l' == s4[3]);
    CHECK('o' == s4[4]);
    CHECK('\0' == s4[5]);

    CHECK('H' == s5[0]);
    CHECK('e' == s5[1]);
    CHECK('l' == s5[2]);
    CHECK('l' == s5[3]);
    CHECK('o' == s5[4]);
    CHECK('\0' == s5[5]);

    CHECK('H' == s6[0]);
    CHECK('e' == s6[1]);
    CHECK('l' == s6[2]);
    CHECK('l' == s6[3]);
    CHECK('o' == s6[4]);
    CHECK('\0' == s6[5]);

    CHECK('H' == s7[0]);
    CHECK('e' == s7[1]);
    CHECK('l' == s7[2]);
    CHECK('l' == s7[3]);
    CHECK('\0' == s7[4]);

    CHECK('H' == s8[0]);
    CHECK('e' == s8[1]);
    CHECK('l' == s8[2]);
    CHECK('l' == s8[3]);
    CHECK('o' == s8[4]);
    CHECK('\0' == s8[5]);

    CHECK('H' == s9[0]);
    CHECK('e' == s9[1]);
    CHECK('l' == s9[2]);
    CHECK('l' == s9[3]);
    CHECK('o' == s9[4]);
    CHECK('\0' == s9[5]);

    {
        constexpr auto s{String{"1234"}};
        auto i{'1'};
        for (const auto& element : s)
        {
            CHECK(i == element);
            ++i;
        }
    }

    {
        // a String is a function of its index
        constexpr auto s{String{"1234"}};
        CHECK('1' == s(0));
        CHECK('2' == s(1));
        CHECK('3' == s(2));
        CHECK('4' == s(3));
        CHECK('\0' == s(4)); // index out-of-bounds so return default element, which is '\0' in this case
    }
}
