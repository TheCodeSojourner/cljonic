#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Equal", "[CljonicCoreEqual]")
{
    EnableNoHeapMessagePrinting();

    {
        CHECK_CLJONIC(Equal('1'));
        CHECK_CLJONIC(Equal('1', '1', '1'));
        CHECK_CLJONIC_NOT(Equal('1', '2', '3'));
    }
    {
        CHECK_CLJONIC(Equal('1'));
        CHECK_CLJONIC(Equal('1', '1', '1'));
        CHECK_CLJONIC_NOT(Equal('1', '2', '3'));
    }

    {
        CHECK_CLJONIC(Equal(1));
        CHECK_CLJONIC(Equal(1, 1, 1));
        CHECK_CLJONIC_NOT(Equal(1, 2, 3));
    }

    {
        CHECK_CLJONIC(Equal(1L));
        CHECK_CLJONIC(Equal(1L, 1L, 1L));
        CHECK_CLJONIC_NOT(Equal(1L, 2L, 3L));
    }

    {
        CHECK_CLJONIC(Equal(1LL));
        CHECK_CLJONIC(Equal(1LL, 1LL, 1LL));
        CHECK_CLJONIC_NOT(Equal(1LL, 2LL, 3LL));
    }

    {
        CHECK_CLJONIC(Equal(1U));
        CHECK_CLJONIC(Equal(1U, 1U, 1U));
        CHECK_CLJONIC_NOT(Equal(1U, 2U, 3U));
    }

    {
        CHECK_CLJONIC(Equal(1UL));
        CHECK_CLJONIC(Equal(1UL, 1UL, 1UL));
        CHECK_CLJONIC_NOT(Equal(1UL, 2UL, 3UL));
    }

    {
        CHECK_CLJONIC(Equal(1ULL));
        CHECK_CLJONIC(Equal(1ULL, 1ULL, 1ULL));
        CHECK_CLJONIC_NOT(Equal(1ULL, 2ULL, 3ULL));
    }

    {
        auto c{1};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, 1, 1));
        CHECK_CLJONIC_NOT(Equal(c, 2, 3));
    }

    {
        auto c{1L};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, 1L, 1L));
        CHECK_CLJONIC_NOT(Equal(c, 2L, 3L));
    }

    {
        auto c{1LL};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, 1LL, 1LL));
        CHECK_CLJONIC_NOT(Equal(c, 2LL, 3LL));
    }

    {
        auto c{1U};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, 1U, 1U));
        CHECK_CLJONIC_NOT(Equal(c, 2U, 3U));
    }

    {
        auto c{1UL};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, 1UL, 1UL));
        CHECK_CLJONIC_NOT(Equal(c, 2UL, 3UL));
    }

    {
        auto c{1ULL};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, 1ULL, 1ULL));
        CHECK_CLJONIC_NOT(Equal(c, 2ULL, 3ULL));
    }

    {
        CHECK_CLJONIC(Equal(Array<char, 3>{'1', '2', '3'}));
        CHECK_CLJONIC(
            Equal(Array<char, 3>{'1', '2', '3'}, Array<char, 13>{'1', '2', '3'}, Array<char, 5>{'1', '2', '3'}));
        CHECK_CLJONIC_NOT(
            Equal(Array<char, 3>{'1', '2', '3'}, Array<char, 13>{'2', '2', '3'}, Array<char, 5>{'3', '2', '3'}));
    }

    {
        CHECK_CLJONIC(Equal(Array<int, 3>{1, 2, 3}));
        CHECK_CLJONIC(Equal(Array<int, 3>{1, 2, 3}, Array<int, 13>{1, 2, 3}, Array<int, 5>{1, 2, 3}));
        CHECK_CLJONIC_NOT(Equal(Array<int, 3>{1, 2, 3}, Array<int, 13>{2, 2, 3}, Array<int, 5>{3, 2, 3}));
    }

    {
        CHECK_CLJONIC(Equal(Array<long, 3>{1L, 2L, 3L}));
        CHECK_CLJONIC(true ==
                      Equal(Array<long, 3>{1L, 2L, 3L}, Array<long, 13>{1L, 2L, 3L}, Array<long, 5>{1L, 2L, 3L}));
        CHECK_CLJONIC(false ==
                      Equal(Array<long, 3>{1L, 2L, 3L}, Array<long, 13>{2L, 2L, 3L}, Array<long, 5>{3L, 2L, 3L}));
    }

    {
        CHECK_CLJONIC(Equal(Array<long long, 3>{1LL, 2LL, 3LL}));
        CHECK_CLJONIC(Equal(Array<long long, 3>{1LL, 2LL, 3LL},
                            Array<long long, 13>{1LL, 2LL, 3LL},
                            Array<long long, 5>{1LL, 2LL, 3LL}));
        CHECK_CLJONIC_NOT(Equal(Array<long long, 3>{1LL, 2LL, 3LL},
                                Array<long long, 13>{2LL, 2LL, 3LL},
                                Array<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        CHECK_CLJONIC(Equal(Array<unsigned, 3>{1U, 2U, 3U}));
        CHECK_CLJONIC(
            Equal(Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 13>{1U, 2U, 3U}, Array<unsigned, 5>{1U, 2U, 3U}));
        CHECK_CLJONIC_NOT(
            Equal(Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 13>{2U, 2U, 3U}, Array<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        CHECK_CLJONIC(Equal(Array<unsigned long, 3>{1UL, 2UL, 3UL}));
        CHECK_CLJONIC(Equal(Array<unsigned long, 3>{1UL, 2UL, 3UL},
                            Array<unsigned long, 13>{1UL, 2UL, 3UL},
                            Array<unsigned long, 5>{1UL, 2UL, 3UL}));
        CHECK_CLJONIC_NOT(Equal(Array<unsigned long, 3>{1UL, 2UL, 3UL},
                                Array<unsigned long, 13>{2UL, 2UL, 3UL},
                                Array<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        CHECK_CLJONIC(Equal(Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL}));
        CHECK_CLJONIC(Equal(Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                            Array<unsigned long long, 13>{1ULL, 2ULL, 3ULL},
                            Array<unsigned long long, 5>{1ULL, 2ULL, 3ULL}));
        CHECK_CLJONIC_NOT(Equal(Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                                Array<unsigned long long, 13>{2ULL, 2ULL, 3ULL},
                                Array<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        auto c{Array<int, 3>{1, 2, 3}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Array<int, 3>{1, 2, 3}, Array<int, 33>{1, 2, 3}));
        CHECK_CLJONIC_NOT(Equal(c, Array<int, 3>{2, 2, 3}, Array<int, 5>{3, 2, 3}));
    }

    {
        auto c{Array<long, 3>{1L, 2L, 3L}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Array<long, 3>{1L, 2L, 3L}, Array<long, 33>{1L, 2L, 3L}));
        CHECK_CLJONIC_NOT(Equal(c, Array<long, 3>{2L, 2L, 3L}, Array<long, 5>{3L, 2L, 3L}));
    }

    {
        auto c{Array<long long, 3>{1LL, 2LL, 3LL}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Array<long long, 3>{1LL, 2LL, 3LL}, Array<long long, 33>{1LL, 2LL, 3LL}));
        CHECK_CLJONIC_NOT(Equal(c, Array<long long, 3>{2LL, 2LL, 3LL}, Array<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        auto c{Array<unsigned, 3>{1U, 2U, 3U}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 33>{1U, 2U, 3U}));
        CHECK_CLJONIC_NOT(Equal(c, Array<unsigned, 3>{2U, 2U, 3U}, Array<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        auto c{Array<unsigned long, 3>{1UL, 2UL, 3UL}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(true ==
                      Equal(c, Array<unsigned long, 3>{1UL, 2UL, 3UL}, Array<unsigned long, 33>{1UL, 2UL, 3UL}));
        CHECK_CLJONIC(false ==
                      Equal(c, Array<unsigned long, 3>{2UL, 2UL, 3UL}, Array<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        auto c{Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(
            Equal(c, Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL}, Array<unsigned long long, 33>{1ULL, 2ULL, 3ULL}));
        CHECK_CLJONIC_NOT(
            Equal(c, Array<unsigned long long, 3>{2ULL, 2ULL, 3ULL}, Array<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        CHECK_CLJONIC(Equal(Array<const char*, 3>{"1", "2"}));
        CHECK_CLJONIC(Equal(Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"1", "2"}));
        CHECK_CLJONIC_NOT(Equal(Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"2", "2"}));
    }

    {
        auto c{Array<const char*, 3>{"1", "2"}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"1", "2"}));
        CHECK_CLJONIC_NOT(Equal(c, Array<const char*, 3>{"2", "2"}, Array<const char*, 3>{"3", "2"}));
    }

    {
        auto c{Array{"1", "2"}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Array{"1", "2"}, Array{"1", "2"}));
        CHECK_CLJONIC_NOT(Equal(c, Array{"2", "2"}, Array{"3", "2"}));
    }

    {
        CHECK_CLJONIC(Equal(Set<int, 3>{1, 2, 3}));
        CHECK_CLJONIC(Equal(Set<int, 3>{1, 2, 3}, Set<int, 3>{1, 2, 3}, Set<int, 3>{1, 2, 3}));
        CHECK_CLJONIC(Equal(Set<int, 3>{1, 2, 3, 1}, Set<int, 3>{1, 2, 3}, Set<int, 3>{1, 2, 3}));
        CHECK_CLJONIC_NOT(Equal(Set<int, 3>{1, 2, 3}, Set<int, 3>{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        auto c{Set<int, 3>{1, 2, 3}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Set<int, 3>{1, 2, 3}, Set<int, 33>{1, 2, 3}));
        CHECK_CLJONIC_NOT(Equal(c, Set<int, 3>{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        CHECK_CLJONIC(Equal(Set{1, 2, 3}));
        CHECK_CLJONIC(Equal(Set{1, 2, 3}, Set{1, 2, 3}, Set{1, 2, 3}));
        CHECK_CLJONIC_NOT(Equal(Set{1, 2, 3}, Set{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        auto c{Set{1, 2, 3}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Set{1, 2, 3}, Set<int, 33>{1, 2, 3}));
        CHECK_CLJONIC_NOT(Equal(c, Set{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        CHECK_CLJONIC(Equal(Set<const char*, 3>{"1", "2", "3"}));
        CHECK_CLJONIC(Equal(Set<const char*, 3>{"1", "2", "3"},
                            Set<const char*, 3>{"1", "2", "3"},
                            Set<const char*, 3>{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(Equal(Set<const char*, 3>{"1", "2", "3"},
                                Set<const char*, 3>{"2", "2", "3"},
                                Set<const char*, 3>{"3", "2", "3"}));
    }

    {
        auto c{Set<const char*, 3>{"1", "2", "3"}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Set<const char*, 3>{"1", "2", "3"}, Set<const char*, 33>{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(Equal(c, Set<const char*, 3>{"2", "2", "3"}, Set<const char*, 5>{"3", "2", "3"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK_CLJONIC(Equal(String<3>{"abc"}));
        CHECK_CLJONIC(Equal(String<3>{"abc"}, String<3>{"abc"}, String<3>{"abc"}));
        CHECK_CLJONIC(Equal(String<3>{"abc"}, String<3>{"abc"}, String<13>{"abc"}));
        CHECK_CLJONIC_NOT(Equal(String<3>{"abc"}, String<3>{"abc"}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, String<3>{"abc"}, String<13>{"abc"}));
        CHECK_CLJONIC_NOT(Equal(c, String<3>{"abc"}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{'a', 'b', 'c'}};
        CHECK_CLJONIC(Equal(String<3>{'a', 'b', 'c'}));
        CHECK_CLJONIC(Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}));
        CHECK_CLJONIC(Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK_CLJONIC_NOT(Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{'a', 'b', 'c'}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK_CLJONIC_NOT(Equal(c, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK_CLJONIC(Equal(String<3>{'a', 'b', 'c'}));
        CHECK_CLJONIC(Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}));
        CHECK_CLJONIC(Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK_CLJONIC_NOT(Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK_CLJONIC_NOT(Equal(c, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        CHECK_CLJONIC(Equal(Range<>{}));
        // CHECK_CLJONIC(Equal(Range<>{}, Range<>{}, Range<>{})); // this will take a LONG time to run
        CHECK_CLJONIC(Equal(Range<10>{}, Range<10>{}, Range<10>{}));
        CHECK_CLJONIC(Equal(Range<-5, 10>{}, Range<-5, 10>{}, Range<-5, 10>{}));
        CHECK_CLJONIC(Equal(Range<-5, 10, 3>{}, Range<-5, 10, 3>{}, Range<-5, 10, 3>{}));
        CHECK_CLJONIC(Equal(Range<5, -10, -2>{}, Range<5, -10, -2>{}, Range<5, -10, -2>{}));
        CHECK_CLJONIC(Equal(Range<5, 5, 0>{}, Range<5, 5, 0>{}, Range<5, 5, 0>{}));
        CHECK_CLJONIC_NOT(Equal(Range<5, 5, 0>{}, Range<5, 15, 0>{}, Range<5, 5, 0>{}));
    }

    {
        auto c{Range<10>{}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Range<10>{}, Range<10>{}));
        CHECK_CLJONIC_NOT(Equal(c, Range<10>{}, Range<100>{}));
    }

    {
        CHECK_CLJONIC(Equal(Repeat{1}));
        // CHECK_CLJONIC(Equal(Repeat{1}, Repeat{1}})); // this will take a LONG time to run
        CHECK_CLJONIC(Equal(Repeat<10, int>{1}, Repeat<10, int>{1}, Repeat<10, int>{1}));
        CHECK_CLJONIC_NOT(Equal(Repeat<10, int>{1}, Repeat<10, int>{2}, Repeat<10, int>{1}));
        CHECK_CLJONIC_NOT(Equal(Repeat<10, int>{1}, Repeat{1}, Repeat<10, int>{1}));
    }

    {
        auto c{Repeat<10, int>{1}};
        CHECK_CLJONIC(Equal(c));
        CHECK_CLJONIC(Equal(c, c, c));
        CHECK_CLJONIC(Equal(c, Repeat<10, int>{1}, Repeat<10, int>{1}));
        CHECK_CLJONIC_NOT(Equal(c, Repeat<10, int>{1}, Repeat<100, char>{'x'}));
    }

    {
        CHECK_CLJONIC(Equal(Range<5>{}, Array{0, 1, 2, 3, 4}));
        CHECK_CLJONIC_NOT(Equal(Range<10>{}, Array{0, 1, 2, 3, 4}));
        CHECK_CLJONIC(Equal(Array{0, 1, 2, 3, 4}, Range<0, 5, 1>{}, Array{0, 1, 2, 3, 4}));
        CHECK_CLJONIC(Equal(Array{5, 3, 1}, Range<5, 0, -2>{}));
        CHECK_CLJONIC(Equal(Range<1>{}, Array{0}, Repeat<1, int>{0}));
        CHECK_CLJONIC(Equal(Array{4, 4, 4, 4}, Repeat<4, int>{4}));
        CHECK_CLJONIC_NOT(Equal(Range<2>{}, Array{0}, Repeat<1, int>{0}));
        CHECK_CLJONIC_NOT(Equal(Range<1>{}, Array{0, 1}, Repeat<1, int>{0}));
        CHECK_CLJONIC_NOT(Equal(Range<1>{}, Array{0}, Repeat<2, int>{0}));
        CHECK_CLJONIC_NOT(Equal(Array{4, 4, 4}, Repeat<4, int>{4}));
        CHECK_CLJONIC_NOT(Equal(Array{4, 4, 4, 4}, Repeat<4, int>{5}));
    }

    {
        CHECK_CLJONIC_NOT(Equal(Array{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC_NOT(Equal(Array{1, 2, 3}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(
            Equal(Iterator{[](const int i) { return i + 1; }, 0}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC_NOT(
            Equal(Iterator{[](const int i) { return i + 1; }, 1}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(Equal(Range{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(Equal(Range<10, CljonicCollectionMaximumElementCount + 10>{},
                            Iterator{[](const int i) { return i + 1; }, 10}));
        CHECK_CLJONIC(Equal(Repeat{1}, Iterator{[]([[maybe_unused]] const int _) { return 1; }, 1}));
        CHECK_CLJONIC_NOT(Equal(String{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC_NOT(Equal(String{"Hello"}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC_NOT(Equal(String{"Hello"}, Iterator{[]([[maybe_unused]] const char _) { return 'a'; }, 'b'}));
    }

    DisableNoHeapMessagePrinting();
}
