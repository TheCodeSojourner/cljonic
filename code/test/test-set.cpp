#include <string>
#include <variant>
#include "catch.hpp"
#include "cljonic-set.hpp"

using namespace cljonic;

SCENARIO("Set", "[CljonicSet]")
{
    {
        auto s0{Set<int, 10>{}};
        auto s1{Set<int, 10>{1, 2, 3, 4}};
        auto s2{Set<int, 4>{1, 2, 3, 4}};
        auto s3{Set<int, 4>{1, 2, 3, 4, 5, 6}};
        auto s4{Set<int, 4>{1, 2, 1, 4, 5, 6}};
        auto s5{Set{1, 2, 3, 4}};
        auto s6{Set{1, 2, 1, 4}};

        CHECK(0 == s0.Count());
        CHECK(4 == s1.Count());
        CHECK(4 == s2.Count());
        CHECK(4 == s3.Count());
        CHECK(4 == s4.Count());
        CHECK(4 == s5.Count());
        CHECK(3 == s6.Count());

        CHECK(not s0.Contains(1));

        CHECK(s1.Contains(1));
        CHECK(s1.Contains(2));
        CHECK(s1.Contains(3));
        CHECK(s1.Contains(4));
        CHECK(not s1.Contains(5));

        CHECK(s2.Contains(1));
        CHECK(s2.Contains(2));
        CHECK(s2.Contains(3));
        CHECK(s2.Contains(4));
        CHECK(not s2.Contains(5));

        CHECK(s3.Contains(1));
        CHECK(s3.Contains(2));
        CHECK(s3.Contains(3));
        CHECK(s3.Contains(4));
        CHECK(not s3.Contains(5));

        CHECK(s4.Contains(1));
        CHECK(s4.Contains(2));
        CHECK(not s4.Contains(3));
        CHECK(s4.Contains(4));
        CHECK(s4.Contains(5));

        CHECK(s5.Contains(1));
        CHECK(s5.Contains(2));
        CHECK(s5.Contains(3));
        CHECK(s5.Contains(4));
        CHECK(not s5.Contains(5));

        CHECK(s6.Contains(1));
        CHECK(s6.Contains(2));
        CHECK(not s6.Contains(3));
        CHECK(s6.Contains(4));
        CHECK(not s6.Contains(5));
    }

    {
        auto s0{Set<const char*, 10>{}};
        auto s1{Set<const char*, 10>{"1.1", "2.1", "3.1", "4.1"}};
        auto s2{Set<const char*, 4>{"1.1", "2.1", "3.1", "4.1"}};
        auto s3{Set<const char*, 4>{"1.1", "2.1", "3.1", "4.1", "5.1", "6.1"}};
        auto s4{Set<const char*, 4>{"1.1", "2.1", "1.1", "4.1", "5.1", "6.1"}};
        auto s5{Set{"1.1", "2.1", "3.1", "4.1"}};
        auto s6{Set{"1.1", "2.1", "1.1", "4.1"}};

        // use the strings in these buffers to test Contains with non-literal strings
        char buffer1[4];
        buffer1[0] = '1';
        buffer1[1] = '.';
        buffer1[2] = '1';
        buffer1[3] = '\0';

        char buffer2[4];
        buffer2[0] = 'x';
        buffer2[1] = 'y';
        buffer2[2] = 'z';
        buffer2[3] = '\0';

        CHECK(0 == s0.Count());
        CHECK(4 == s1.Count());
        CHECK(4 == s2.Count());
        CHECK(4 == s3.Count());
        CHECK(4 == s4.Count());

        CHECK(not s0.Contains(buffer1));
        CHECK(not s0.Contains(buffer2));

        CHECK(s1.Contains(buffer1));
        CHECK(not s1.Contains(buffer2));
        CHECK(s1.Contains("1.1"));
        CHECK(s1.Contains("2.1"));
        CHECK(s1.Contains("3.1"));
        CHECK(s1.Contains("4.1"));
        CHECK(not s1.Contains("5.1"));

        CHECK(s2.Contains(buffer1));
        CHECK(not s3.Contains(buffer2));
        CHECK(s2.Contains("1.1"));
        CHECK(s2.Contains("2.1"));
        CHECK(s2.Contains("3.1"));
        CHECK(s2.Contains("4.1"));
        CHECK(not s2.Contains("5.1"));

        CHECK(s3.Contains(buffer1));
        CHECK(not s3.Contains(buffer2));
        CHECK(s3.Contains("1.1"));
        CHECK(s3.Contains("2.1"));
        CHECK(s3.Contains("3.1"));
        CHECK(s3.Contains("4.1"));
        CHECK(not s3.Contains("5.1"));

        CHECK(s4.Contains(buffer1));
        CHECK(not s4.Contains(buffer2));
        CHECK(s4.Contains("1.1"));
        CHECK(s4.Contains("2.1"));
        CHECK(not s4.Contains("3.1"));
        CHECK(s4.Contains("4.1"));
        CHECK(s4.Contains("5.1"));

        CHECK(s5.Contains(buffer1));
        CHECK(not s5.Contains(buffer2));
        CHECK(s5.Contains("1.1"));
        CHECK(s5.Contains("2.1"));
        CHECK(s5.Contains("3.1"));
        CHECK(s5.Contains("4.1"));
        CHECK(not s5.Contains("5.1"));

        CHECK(s6.Contains(buffer1));
        CHECK(not s6.Contains(buffer2));
        CHECK(s6.Contains("1.1"));
        CHECK(s6.Contains("2.1"));
        CHECK(not s6.Contains("3.1"));
        CHECK(s6.Contains("4.1"));
    }

    {
        using T = std::variant<int, double, char, const char*>;

        auto s0{Set<T, 10>{}};
        auto s1{Set<T, 10>{1, 2.1, '3', "4"}};
        auto s2{Set<T, 4>{1, 2.1, '3', "4"}};
        auto s3{Set<T, 4>{1, 2.1, '3', "4", 5, 6.1}};
        auto s4{Set<T, 4>{1, 2.1, 1, "4", 5, 6.1}};
        auto s5{Set{T{1}, T{2.1}, T{'3'}, T{"4"}}};
        auto s6{Set{T{1}, T{2.1}, T{1}, T{"4"}}};

        CHECK(0 == s0.Count());
        CHECK(4 == s1.Count());
        CHECK(4 == s2.Count());
        CHECK(4 == s3.Count());
        CHECK(4 == s4.Count());

        CHECK(not s0.Contains(T{1}));

        CHECK(s1.Contains(T{1}));
        CHECK(s1.Contains(T{2.1}));
        CHECK(s1.Contains(T{'3'}));
        CHECK(s1.Contains(T{"4"}));
        CHECK(not s1.Contains(T{5.1}));

        CHECK(s2.Contains(T{1}));
        CHECK(s2.Contains(T{2.1}));
        CHECK(s2.Contains(T{'3'}));
        CHECK(s2.Contains(T{"4"}));
        CHECK(not s2.Contains(T{5.1}));

        CHECK(s3.Contains(T{1}));
        CHECK(s3.Contains(T{2.1}));
        CHECK(s3.Contains(T{'3'}));
        CHECK(s3.Contains(T{"4"}));
        CHECK(not s3.Contains(T{5.1}));

        CHECK(s4.Contains(T{1}));
        CHECK(s4.Contains(T{2.1}));
        CHECK(not s4.Contains(T{'3'}));
        CHECK(s4.Contains(T{"4"}));
        CHECK(s4.Contains(T{5}));

        CHECK(s5.Contains(T{1}));
        CHECK(s5.Contains(T{2.1}));
        CHECK(s5.Contains(T{'3'}));
        CHECK(s5.Contains(T{"4"}));
        CHECK(not s5.Contains("5.1"));

        CHECK(s6.Contains(T{1}));
        CHECK(s6.Contains(T{2.1}));
        CHECK(not s6.Contains(T{'3'}));
        CHECK(s6.Contains(T{"4"}));
    }

    {
        const auto s{Set<int, 10>{1, 2, 3, 4}};
        auto i{1};
        for (const auto& element : s)
        {
            CHECK(i == element);
            ++i;
        }
        CHECK(0 == s[4]);
        // *s.begin() = 1; // Error: assignment of read-only location '* s.Array<int, 10>::begin() const'
    }

    {
        // a Set is a function of its elements
        const auto s{Set<int, 10>{1, 2, 3, 4}};
        CHECK(1 == s(1));
        CHECK(2 == s(2));
        CHECK(3 == s(3));
        CHECK(4 == s(4));
        CHECK(0 == s(5)); // value is not in the Set so return default element, which is 0 in this case
    }
}
