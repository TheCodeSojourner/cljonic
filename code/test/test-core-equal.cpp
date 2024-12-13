#include "catch.hpp"
#include "cljonic-core.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-vector.hpp"

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
        CHECK(true == Equal(Vector<char, 3>{'1', '2', '3'}));
        CHECK(true ==
              Equal(Vector<char, 3>{'1', '2', '3'}, Vector<char, 13>{'1', '2', '3'}, Vector<char, 5>{'1', '2', '3'}));
        CHECK(false ==
              Equal(Vector<char, 3>{'1', '2', '3'}, Vector<char, 13>{'2', '2', '3'}, Vector<char, 5>{'3', '2', '3'}));
    }

    {
        CHECK(true == Equal(Vector<int, 3>{1, 2, 3}));
        CHECK(true == Equal(Vector<int, 3>{1, 2, 3}, Vector<int, 13>{1, 2, 3}, Vector<int, 5>{1, 2, 3}));
        CHECK(false == Equal(Vector<int, 3>{1, 2, 3}, Vector<int, 13>{2, 2, 3}, Vector<int, 5>{3, 2, 3}));
    }

    {
        CHECK(true == Equal(Vector<long, 3>{1L, 2L, 3L}));
        CHECK(true == Equal(Vector<long, 3>{1L, 2L, 3L}, Vector<long, 13>{1L, 2L, 3L}, Vector<long, 5>{1L, 2L, 3L}));
        CHECK(false == Equal(Vector<long, 3>{1L, 2L, 3L}, Vector<long, 13>{2L, 2L, 3L}, Vector<long, 5>{3L, 2L, 3L}));
    }

    {
        CHECK(true == Equal(Vector<long long, 3>{1LL, 2LL, 3LL}));
        CHECK(true == Equal(Vector<long long, 3>{1LL, 2LL, 3LL},
                            Vector<long long, 13>{1LL, 2LL, 3LL},
                            Vector<long long, 5>{1LL, 2LL, 3LL}));
        CHECK(false == Equal(Vector<long long, 3>{1LL, 2LL, 3LL},
                             Vector<long long, 13>{2LL, 2LL, 3LL},
                             Vector<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        CHECK(true == Equal(Vector<unsigned, 3>{1U, 2U, 3U}));
        CHECK(true == Equal(Vector<unsigned, 3>{1U, 2U, 3U},
                            Vector<unsigned, 13>{1U, 2U, 3U},
                            Vector<unsigned, 5>{1U, 2U, 3U}));
        CHECK(false == Equal(Vector<unsigned, 3>{1U, 2U, 3U},
                             Vector<unsigned, 13>{2U, 2U, 3U},
                             Vector<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        CHECK(true == Equal(Vector<unsigned long, 3>{1UL, 2UL, 3UL}));
        CHECK(true == Equal(Vector<unsigned long, 3>{1UL, 2UL, 3UL},
                            Vector<unsigned long, 13>{1UL, 2UL, 3UL},
                            Vector<unsigned long, 5>{1UL, 2UL, 3UL}));
        CHECK(false == Equal(Vector<unsigned long, 3>{1UL, 2UL, 3UL},
                             Vector<unsigned long, 13>{2UL, 2UL, 3UL},
                             Vector<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        CHECK(true == Equal(Vector<unsigned long long, 3>{1ULL, 2ULL, 3ULL}));
        CHECK(true == Equal(Vector<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                            Vector<unsigned long long, 13>{1ULL, 2ULL, 3ULL},
                            Vector<unsigned long long, 5>{1ULL, 2ULL, 3ULL}));
        CHECK(false == Equal(Vector<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                             Vector<unsigned long long, 13>{2ULL, 2ULL, 3ULL},
                             Vector<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        auto c{Vector<int, 3>{1, 2, 3}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Vector<int, 3>{1, 2, 3}, Vector<int, 33>{1, 2, 3}));
        CHECK(false == Equal(c, Vector<int, 3>{2, 2, 3}, Vector<int, 5>{3, 2, 3}));
    }

    {
        auto c{Vector<long, 3>{1L, 2L, 3L}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Vector<long, 3>{1L, 2L, 3L}, Vector<long, 33>{1L, 2L, 3L}));
        CHECK(false == Equal(c, Vector<long, 3>{2L, 2L, 3L}, Vector<long, 5>{3L, 2L, 3L}));
    }

    {
        auto c{Vector<long long, 3>{1LL, 2LL, 3LL}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Vector<long long, 3>{1LL, 2LL, 3LL}, Vector<long long, 33>{1LL, 2LL, 3LL}));
        CHECK(false == Equal(c, Vector<long long, 3>{2LL, 2LL, 3LL}, Vector<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        auto c{Vector<unsigned, 3>{1U, 2U, 3U}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Vector<unsigned, 3>{1U, 2U, 3U}, Vector<unsigned, 33>{1U, 2U, 3U}));
        CHECK(false == Equal(c, Vector<unsigned, 3>{2U, 2U, 3U}, Vector<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        auto c{Vector<unsigned long, 3>{1UL, 2UL, 3UL}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Vector<unsigned long, 3>{1UL, 2UL, 3UL}, Vector<unsigned long, 33>{1UL, 2UL, 3UL}));
        CHECK(false == Equal(c, Vector<unsigned long, 3>{2UL, 2UL, 3UL}, Vector<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        auto c{Vector<unsigned long long, 3>{1ULL, 2ULL, 3ULL}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c,
                            Vector<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                            Vector<unsigned long long, 33>{1ULL, 2ULL, 3ULL}));
        CHECK(false == Equal(c,
                             Vector<unsigned long long, 3>{2ULL, 2ULL, 3ULL},
                             Vector<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        CHECK(true == Equal(Vector<const char*, 3>{"1", "2"}));
        CHECK(true == Equal(Vector<const char*, 3>{"1", "2"}, Vector<const char*, 3>{"1", "2"}));
        CHECK(false == Equal(Vector<const char*, 3>{"1", "2"}, Vector<const char*, 3>{"2", "2"}));
    }

    {
        auto c{Vector<const char*, 3>{"1", "2"}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Vector<const char*, 3>{"1", "2"}, Vector<const char*, 3>{"1", "2"}));
        CHECK(false == Equal(c, Vector<const char*, 3>{"2", "2"}, Vector<const char*, 3>{"3", "2"}));
    }

    {
        auto c{Vector{"1", "2"}};
        CHECK(true == Equal(c));
        CHECK(true == Equal(c, c, c));
        CHECK(true == Equal(c, Vector{"1", "2"}, Vector{"1", "2"}));
        CHECK(false == Equal(c, Vector{"2", "2"}, Vector{"3", "2"}));
    }

    // all of the following tests are commented out because they will generate expected compiler errors
    // due to the fact that the Equal function knows that equality comparison of floating point types is not exact
    //{
    // CHECK(true == Equal(Vector<double, 3>{1.1, 2.1}));
    // CHECK(true == Equal(Vector<double, 3>{1.1, 2.1}, Vector<double, 3>{1.1, 2.1}, Vector<double, 3>{1.1, 2.1}));
    // CHECK(true == Equal(Vector<int, 3>{1, 2}, Vector<double, 3>{1.1, 2.1}, Vector<int, 3>{1, 2}));
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
}
