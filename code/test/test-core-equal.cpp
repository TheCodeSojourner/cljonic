#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equal.hpp"

using namespace cljonic;
using namespace cljonic::core;

SCENARIO("Equal", "[CljonicCoreEqual]")
{
    {
        CHECK(true == Equal('1'));
        CHECK(true == Equal('1', '1', '1'));
        CHECK(false == Equal('1', '2', '3'));
    }

    {
        CHECK(true == Equal(1));
        CHECK(true == Equal(1, 1, 1));
        CHECK(false == Equal(1, 2, 3));
    }

    {
        CHECK(true == Equal(1L));
        CHECK(true == Equal(1L, 1L, 1L));
        CHECK(false == Equal(1L, 2L, 3L));
    }

    {
        CHECK(true == Equal(1LL));
        CHECK(true == Equal(1LL, 1LL, 1LL));
        CHECK(false == Equal(1LL, 2LL, 3LL));
    }

    {
        CHECK(true == Equal(1U));
        CHECK(true == Equal(1U, 1U, 1U));
        CHECK(false == Equal(1U, 2U, 3U));
    }

    {
        CHECK(true == Equal(1UL));
        CHECK(true == Equal(1UL, 1UL, 1UL));
        CHECK(false == Equal(1UL, 2UL, 3UL));
    }

    {
        CHECK(true == Equal(1ULL));
        CHECK(true == Equal(1ULL, 1ULL, 1ULL));
        CHECK(false == Equal(1ULL, 2ULL, 3ULL));
    }

    {
        auto c{1};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, 1, 1));
        CHECK(false == Equal(c, 2, 3));
    }

    {
        auto c{1L};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, 1L, 1L));
        CHECK(false == Equal(c, 2L, 3L));
    }

    {
        auto c{1LL};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, 1LL, 1LL));
        CHECK(false == Equal(c, 2LL, 3LL));
    }

    {
        auto c{1U};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, 1U, 1U));
        CHECK(false == Equal(c, 2U, 3U));
    }

    {
        auto c{1UL};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, 1UL, 1UL));
        CHECK(false == Equal(c, 2UL, 3UL));
    }

    {
        auto c{1ULL};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, 1ULL, 1ULL));
        CHECK(false == Equal(c, 2ULL, 3ULL));
    }

    // all of the following tests are commented out because they will generate expected compiler errors
    // due to the fact that the Equal function knows that equality comparison of floating point types is not exact
    // {
    //     CHECK(true == Equal(1.1));
    //     CHECK(true == Equal(1.1, 1.1, 1.1));
    //     CHECK(false == Equal(1, 1.0, 1));
    // }

    {
        CHECK(true == Equal(Array<char, 3>{'1', '2', '3'}));
        CHECK(true ==
              Equal(Array<char, 3>{'1', '2', '3'}, Array<char, 13>{'1', '2', '3'}, Array<char, 5>{'1', '2', '3'}));
        CHECK(false ==
              Equal(Array<char, 3>{'1', '2', '3'}, Array<char, 13>{'2', '2', '3'}, Array<char, 5>{'3', '2', '3'}));
    }

    {
        CHECK(true == Equal(Array<int, 3>{1, 2, 3}));
        CHECK(true == Equal(Array<int, 3>{1, 2, 3}, Array<int, 13>{1, 2, 3}, Array<int, 5>{1, 2, 3}));
        CHECK(false == Equal(Array<int, 3>{1, 2, 3}, Array<int, 13>{2, 2, 3}, Array<int, 5>{3, 2, 3}));
    }

    {
        CHECK(true == Equal(Array<long, 3>{1L, 2L, 3L}));
        CHECK(true == Equal(Array<long, 3>{1L, 2L, 3L}, Array<long, 13>{1L, 2L, 3L}, Array<long, 5>{1L, 2L, 3L}));
        CHECK(false == Equal(Array<long, 3>{1L, 2L, 3L}, Array<long, 13>{2L, 2L, 3L}, Array<long, 5>{3L, 2L, 3L}));
    }

    {
        CHECK(true == Equal(Array<long long, 3>{1LL, 2LL, 3LL}));
        CHECK(true == Equal(Array<long long, 3>{1LL, 2LL, 3LL},
                            Array<long long, 13>{1LL, 2LL, 3LL},
                            Array<long long, 5>{1LL, 2LL, 3LL}));
        CHECK(false == Equal(Array<long long, 3>{1LL, 2LL, 3LL},
                             Array<long long, 13>{2LL, 2LL, 3LL},
                             Array<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        CHECK(true == Equal(Array<unsigned, 3>{1U, 2U, 3U}));
        CHECK(true ==
              Equal(Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 13>{1U, 2U, 3U}, Array<unsigned, 5>{1U, 2U, 3U}));
        CHECK(false ==
              Equal(Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 13>{2U, 2U, 3U}, Array<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        CHECK(true == Equal(Array<unsigned long, 3>{1UL, 2UL, 3UL}));
        CHECK(true == Equal(Array<unsigned long, 3>{1UL, 2UL, 3UL},
                            Array<unsigned long, 13>{1UL, 2UL, 3UL},
                            Array<unsigned long, 5>{1UL, 2UL, 3UL}));
        CHECK(false == Equal(Array<unsigned long, 3>{1UL, 2UL, 3UL},
                             Array<unsigned long, 13>{2UL, 2UL, 3UL},
                             Array<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        CHECK(true == Equal(Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL}));
        CHECK(true == Equal(Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                            Array<unsigned long long, 13>{1ULL, 2ULL, 3ULL},
                            Array<unsigned long long, 5>{1ULL, 2ULL, 3ULL}));
        CHECK(false == Equal(Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                             Array<unsigned long long, 13>{2ULL, 2ULL, 3ULL},
                             Array<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        auto c{Array<int, 3>{1, 2, 3}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Array<int, 3>{1, 2, 3}, Array<int, 33>{1, 2, 3}));
        CHECK(false == Equal(c, Array<int, 3>{2, 2, 3}, Array<int, 5>{3, 2, 3}));
    }

    {
        auto c{Array<long, 3>{1L, 2L, 3L}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Array<long, 3>{1L, 2L, 3L}, Array<long, 33>{1L, 2L, 3L}));
        CHECK(false == Equal(c, Array<long, 3>{2L, 2L, 3L}, Array<long, 5>{3L, 2L, 3L}));
    }

    {
        auto c{Array<long long, 3>{1LL, 2LL, 3LL}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Array<long long, 3>{1LL, 2LL, 3LL}, Array<long long, 33>{1LL, 2LL, 3LL}));
        CHECK(false == Equal(c, Array<long long, 3>{2LL, 2LL, 3LL}, Array<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        auto c{Array<unsigned, 3>{1U, 2U, 3U}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 33>{1U, 2U, 3U}));
        CHECK(false == Equal(c, Array<unsigned, 3>{2U, 2U, 3U}, Array<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        auto c{Array<unsigned long, 3>{1UL, 2UL, 3UL}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Array<unsigned long, 3>{1UL, 2UL, 3UL}, Array<unsigned long, 33>{1UL, 2UL, 3UL}));
        CHECK(false == Equal(c, Array<unsigned long, 3>{2UL, 2UL, 3UL}, Array<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        auto c{Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c,
                            Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                            Array<unsigned long long, 33>{1ULL, 2ULL, 3ULL}));
        CHECK(false ==
              Equal(c, Array<unsigned long long, 3>{2ULL, 2ULL, 3ULL}, Array<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        CHECK(true == Equal(Array<const char*, 3>{"1", "2"}));
        CHECK(true == Equal(Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"1", "2"}));
        CHECK(false == Equal(Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"2", "2"}));
    }

    {
        auto c{Array<const char*, 3>{"1", "2"}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"1", "2"}));
        CHECK(false == Equal(c, Array<const char*, 3>{"2", "2"}, Array<const char*, 3>{"3", "2"}));
    }

    {
        auto c{Array{"1", "2"}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Array{"1", "2"}, Array{"1", "2"}));
        CHECK(false == Equal(c, Array{"2", "2"}, Array{"3", "2"}));
    }

    // all of the following tests are commented out because they will generate expected compiler errors
    // due to the fact that the Equal function knows that equality comparison of floating point types is not exact
    //{
    // CHECK(true == Equal(Array<double, 3>{1.1, 2.1}));
    // CHECK(true == Equal(Array<double, 3>{1.1, 2.1}, Array<double, 3>{1.1, 2.1}, Array<double, 3>{1.1, 2.1}));
    // CHECK(true == Equal(Array<int, 3>{1, 2}, Array<double, 3>{1.1, 2.1}, Array<int, 3>{1, 2}));
    // }

    {
        CHECK(true == Equal(Set<int, 3>{1, 2, 3}));
        CHECK(true == Equal(Set<int, 3>{1, 2, 3}, Set<int, 3>{1, 2, 3}, Set<int, 3>{1, 2, 3}));
        CHECK(false == Equal(Set<int, 3>{1, 2, 3}, Set<int, 3>{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        auto c{Set<int, 3>{1, 2, 3}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Set<int, 3>{1, 2, 3}, Set<int, 33>{1, 2, 3}));
        CHECK(false == Equal(c, Set<int, 3>{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        CHECK(true == Equal(Set{1, 2, 3}));
        CHECK(true == Equal(Set{1, 2, 3}, Set{1, 2, 3}, Set{1, 2, 3}));
        CHECK(false == Equal(Set{1, 2, 3}, Set{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        auto c{Set{1, 2, 3}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Set{1, 2, 3}, Set<int, 33>{1, 2, 3}));
        CHECK(false == Equal(c, Set{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        CHECK(true == Equal(Set<const char*, 3>{"1", "2", "3"}));
        CHECK(true == Equal(Set<const char*, 3>{"1", "2", "3"},
                            Set<const char*, 3>{"1", "2", "3"},
                            Set<const char*, 3>{"1", "2", "3"}));
        CHECK(false == Equal(Set<const char*, 3>{"1", "2", "3"},
                             Set<const char*, 3>{"2", "2", "3"},
                             Set<const char*, 3>{"3", "2", "3"}));
    }

    {
        auto c{Set<const char*, 3>{"1", "2", "3"}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Set<const char*, 3>{"1", "2", "3"}, Set<const char*, 33>{"1", "2", "3"}));
        CHECK(false == Equal(c, Set<const char*, 3>{"2", "2", "3"}, Set<const char*, 5>{"3", "2", "3"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK(true == Equal(String<3>{"abc"}));
        CHECK(true == Equal(String<3>{"abc"}, String<3>{"abc"}, String<3>{"abc"}));
        CHECK(true == Equal(String<3>{"abc"}, String<3>{"abc"}, String<13>{"abc"}));
        CHECK(false == Equal(String<3>{"abc"}, String<3>{"abc"}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, String<3>{"abc"}, String<13>{"abc"}));
        CHECK(false == Equal(c, String<3>{"abc"}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{'a', 'b', 'c'}};
        CHECK(true == Equal(String<3>{'a', 'b', 'c'}));
        CHECK(true == Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}));
        CHECK(true == Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK(false == Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{'a', 'b', 'c'}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK(false == Equal(c, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK(true == Equal(String<3>{'a', 'b', 'c'}));
        CHECK(true == Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}));
        CHECK(true == Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK(false == Equal(String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK(false == Equal(c, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        CHECK(true == Equal(Range<>{}));
        // CHECK(true == Equal(Range<>{}, Range<>{}, Range<>{})); // this will take a LONG time to run
        CHECK(true == Equal(Range<10>{}, Range<10>{}, Range<10>{}));
        CHECK(true == Equal(Range<-5, 10>{}, Range<-5, 10>{}, Range<-5, 10>{}));
        CHECK(true == Equal(Range<-5, 10, 3>{}, Range<-5, 10, 3>{}, Range<-5, 10, 3>{}));
        CHECK(true == Equal(Range<5, -10, -2>{}, Range<5, -10, -2>{}, Range<5, -10, -2>{}));
        CHECK(true == Equal(Range<5, 5, 0>{}, Range<5, 5, 0>{}, Range<5, 5, 0>{}));
        CHECK(false == Equal(Range<5, 5, 0>{}, Range<5, 15, 0>{}, Range<5, 5, 0>{}));
    }

    {
        auto c{Range<10>{}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Range<10>{}, Range<10>{}));
        CHECK(false == Equal(c, Range<10>{}, Range<100>{}));
    }

    {
        CHECK(true == Equal(Repeat{1}));
        // CHECK(true == Equal(Repeat{1}, Repeat{1}})); // this will take a LONG time to run
        CHECK(true == Equal(Repeat<10, int>{1}, Repeat<10, int>{1}, Repeat<10, int>{1}));
        CHECK(false == Equal(Repeat<10, int>{1}, Repeat<10, int>{2}, Repeat<10, int>{1}));
        CHECK(false == Equal(Repeat<10, int>{1}, Repeat{1}, Repeat<10, int>{1}));
    }

    {
        auto c{Repeat<10, int>{1}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Repeat<10, int>{1}, Repeat<10, int>{1}));
        CHECK(false == Equal(c, Repeat<10, int>{1}, Repeat<100, char>{'x'}));
    }

    {
        CHECK(true == Equal(Range<5>{}, Array{0, 1, 2, 3, 4}));
        CHECK(false == Equal(Range<10>{}, Array{0, 1, 2, 3, 4}));
        CHECK(true == Equal(Array{0, 1, 2, 3, 4}, Range<0, 5, 1>{}, Array{0, 1, 2, 3, 4}));
        CHECK(true == Equal(Array{5, 3, 1}, Range<5, 0, -2>{}));
        CHECK(true == Equal(Range<1>{}, Array{0}, Repeat<1, int>{0}));
        CHECK(true == Equal(Array{4, 4, 4, 4}, Repeat<4, int>{4}));
        CHECK(false == Equal(Range<2>{}, Array{0}, Repeat<1, int>{0}));
        CHECK(false == Equal(Range<1>{}, Array{0, 1}, Repeat<1, int>{0}));
        CHECK(false == Equal(Range<1>{}, Array{0}, Repeat<2, int>{0}));
        CHECK(false == Equal(Array{4, 4, 4}, Repeat<4, int>{4}));
        CHECK(false == Equal(Array{4, 4, 4, 4}, Repeat<4, int>{5}));
    }
}
