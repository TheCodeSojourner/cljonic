#include <string>
#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-set.hpp"

using namespace cljonic;

SCENARIO("Set", "[CljonicSet]")
{
    EnableNoHeapMessagePrinting();
    {
        constexpr auto s{Set<int, 10>{}};
        CHECK_CLJONIC(0 == s.Count());
        auto lastElement{1234};
        for ([[maybe_unused]] const auto& element : s)
        {
            CHECK_CLJONIC(false);
            lastElement = element;
        }
        CHECK_CLJONIC(1234 == lastElement);
        CHECK_CLJONIC_NOT(s.Contains(1));
    }

    {
        constexpr auto s{Set<int, 10>{1, 2, 3, 4}};
        CHECK_CLJONIC(4 == s.Count());
        auto iLast{1234};
        auto i{1};
        for ([[maybe_unused]] const auto& element : s)
        {
            CHECK_CLJONIC(s.Contains(i++));
            iLast = i;
        }
        CHECK_CLJONIC(5 == iLast);
        CHECK_CLJONIC_NOT(s.Contains(5));
    }

    {
        constexpr auto s{Set<int, 4>{1, 2, 3, 4}};
        CHECK_CLJONIC(4 == s.Count());
        auto iLast{1234};
        auto i{1};
        for ([[maybe_unused]] const auto& element : s)
        {
            CHECK_CLJONIC(s.Contains(i++));
            iLast = i;
        }
        CHECK_CLJONIC(5 == iLast);
        CHECK_CLJONIC_NOT(s.Contains(5));
    }

    {
        constexpr auto s{Set<int, 4>{1, 2, 3, 4, 5, 6}};
        CHECK_CLJONIC(4 == s.Count());
        auto iLast{1234};
        auto i{1};
        for ([[maybe_unused]] const auto& element : s)
        {
            CHECK_CLJONIC(s.Contains(i++));
            iLast = i;
        }
        CHECK_CLJONIC(5 == iLast);
        CHECK_CLJONIC_NOT(s.Contains(5));
    }

    {
        constexpr auto s{Set<int, 4>{1, 2, 1, 4, 3, 6}};
        CHECK_CLJONIC(4 == s.Count());
        auto iLast{1234};
        auto i{1};
        for ([[maybe_unused]] const auto& element : s)
        {
            CHECK_CLJONIC(s.Contains(i++));
            iLast = i;
        }
        CHECK_CLJONIC(5 == iLast);
        CHECK_CLJONIC_NOT(s.Contains(6));
    }

    {
        constexpr auto s{Set{1, 2, 3, 4}};
        CHECK_CLJONIC(4 == s.Count());
        auto iLast{1234};
        auto i{1};
        for ([[maybe_unused]] const auto& element : s)
        {
            CHECK_CLJONIC(s.Contains(i++));
            iLast = i;
        }
        CHECK_CLJONIC(5 == iLast);
        CHECK_CLJONIC_NOT(s.Contains(5));
    }

    {
        constexpr auto s{Set{1, 2, 1, 3}};
        CHECK_CLJONIC(3 == s.Count());
        auto iLast{1234};
        auto i{1};
        for ([[maybe_unused]] const auto& element : s)
        {
            CHECK_CLJONIC(s.Contains(i++));
            iLast = i;
        }
        CHECK_CLJONIC(4 == iLast);
        CHECK_CLJONIC_NOT(s.Contains(4));
        CHECK_CLJONIC_NOT(s.Contains(-4));
    }

    {
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

        {
            constexpr auto s{Set<const char*, 10>{}};
            CHECK_CLJONIC(0 == s.Count());
            CHECK_CLJONIC_NOT(s.Contains(buffer1));
            CHECK_CLJONIC_NOT(s.Contains(buffer2));
        }

        {
            constexpr auto s{Set<const char*, 10>{"1.1", "2.1", "3.1", "4.1"}};
            CHECK_CLJONIC(4 == s.Count());
            CHECK_CLJONIC(s.Contains(buffer1));
            CHECK_CLJONIC_NOT(s.Contains(buffer2));
            CHECK_CLJONIC(s.Contains("1.1"));
            CHECK_CLJONIC(s.Contains("2.1"));
            CHECK_CLJONIC(s.Contains("3.1"));
            CHECK_CLJONIC(s.Contains("4.1"));
            CHECK_CLJONIC_NOT(s.Contains("5.1"));
        }

        {
            constexpr auto s{Set<const char*, 4>{"1.1", "2.1", "3.1", "4.1"}};
            CHECK_CLJONIC(4 == s.Count());
            CHECK_CLJONIC(s.Contains(buffer1));
            CHECK_CLJONIC_NOT(s.Contains(buffer2));
            CHECK_CLJONIC(s.Contains("1.1"));
            CHECK_CLJONIC(s.Contains("2.1"));
            CHECK_CLJONIC(s.Contains("3.1"));
            CHECK_CLJONIC(s.Contains("4.1"));
            CHECK_CLJONIC_NOT(s.Contains("5.1"));
        }

        {
            constexpr auto s{Set<const char*, 4>{"1.1", "2.1", "3.1", "4.1", "5.1", "6.1"}};
            CHECK_CLJONIC(4 == s.Count());
            CHECK_CLJONIC(s.Contains(buffer1));
            CHECK_CLJONIC_NOT(s.Contains(buffer2));
            CHECK_CLJONIC(s.Contains("1.1"));
            CHECK_CLJONIC(s.Contains("2.1"));
            CHECK_CLJONIC(s.Contains("3.1"));
            CHECK_CLJONIC(s.Contains("4.1"));
            CHECK_CLJONIC_NOT(s.Contains("5.1"));
        }

        {
            constexpr auto s{Set<const char*, 4>{"1.1", "2.1", "1.1", "4.1", "5.1", "6.1"}};
            CHECK_CLJONIC(4 == s.Count());
            CHECK_CLJONIC(s.Contains(buffer1));
            CHECK_CLJONIC_NOT(s.Contains(buffer2));
            CHECK_CLJONIC(s.Contains("1.1"));
            CHECK_CLJONIC(s.Contains("2.1"));
            CHECK_CLJONIC_NOT(s.Contains("3.1"));
            CHECK_CLJONIC(s.Contains("4.1"));
            CHECK_CLJONIC(s.Contains("5.1"));
        }

        {
            constexpr auto s{Set{"1.1", "2.1", "3.1", "4.1"}};
            CHECK_CLJONIC(s.Contains(buffer1));
            CHECK_CLJONIC_NOT(s.Contains(buffer2));
            CHECK_CLJONIC(s.Contains("1.1"));
            CHECK_CLJONIC(s.Contains("2.1"));
            CHECK_CLJONIC(s.Contains("3.1"));
            CHECK_CLJONIC(s.Contains("4.1"));
            CHECK_CLJONIC_NOT(s.Contains("5.1"));
        }

        {
            constexpr auto s{Set{"1.1", "2.1", "1.1", "4.1"}};
            CHECK_CLJONIC(s.Contains(buffer1));
            CHECK_CLJONIC_NOT(s.Contains(buffer2));
            CHECK_CLJONIC(s.Contains("1.1"));
            CHECK_CLJONIC(s.Contains("2.1"));
            CHECK_CLJONIC_NOT(s.Contains("3.1"));
            CHECK_CLJONIC(s.Contains("4.1"));
        }
    }

    {
        // a Set is a function of its elements
        constexpr auto s{Set<int, 10>{1, 2, 3, 4}};
        CHECK_CLJONIC(1 == s(1));
        CHECK_CLJONIC(2 == s(2));
        CHECK_CLJONIC(3 == s(3));
        CHECK_CLJONIC(4 == s(4));
        CHECK_CLJONIC(0 == s(5)); // value is not in the Set so return default element, which is 0 in this case
    }
    DisableNoHeapMessagePrinting();
}
