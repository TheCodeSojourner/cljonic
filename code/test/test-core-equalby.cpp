#include <tuple>
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-string.hpp"
#include "cljonic-core-equalby.hpp"

using namespace cljonic;
using namespace cljonic::core;

template <typename T>
auto EBF()
{
    return [](const T a, const T b) { return a == b; };
}

const auto EBFc = EBF<char>();
const auto EBFccp = EBF<const char*>();
const auto EBFd = EBF<double>();
const auto EBFi = EBF<int>();
const auto EBFl = EBF<long>();
const auto EBFll = EBF<long long>();
const auto EBFu = EBF<unsigned>();
const auto EBFul = EBF<unsigned long>();
const auto EBFull = EBF<unsigned long long>();

SCENARIO("EqualBy", "[CljonicCoreEqualBy]")
{
    EnableNoHeapMessagePrinting();

    {
        CHECK_CLJONIC(EqualBy(EBFc, '1'));
        CHECK_CLJONIC(EqualBy(EBFc, '1', '1', '1'));
        CHECK_CLJONIC_NOT(EqualBy(EBFc, '1', '2', '3'));

        CHECK_CLJONIC(EqualBy(EBFi, '1'));
        CHECK_CLJONIC(EqualBy(EBFi, '1', '1', '1'));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, '1', '2', '3'));

        CHECK_CLJONIC(EqualBy(EBFl, '1'));
        CHECK_CLJONIC(EqualBy(EBFl, '1', '1', '1'));
        CHECK_CLJONIC_NOT(EqualBy(EBFl, '1', '2', '3'));

        CHECK_CLJONIC(EqualBy(EBFll, '1'));
        CHECK_CLJONIC(EqualBy(EBFll, '1', '1', '1'));
        CHECK_CLJONIC_NOT(EqualBy(EBFll, '1', '2', '3'));

        CHECK_CLJONIC(EqualBy(EBFu, '1'));
        CHECK_CLJONIC(EqualBy(EBFu, '1', '1', '1'));
        CHECK_CLJONIC_NOT(EqualBy(EBFu, '1', '2', '3'));

        CHECK_CLJONIC(EqualBy(EBFul, '1'));
        CHECK_CLJONIC(EqualBy(EBFul, '1', '1', '1'));
        CHECK_CLJONIC_NOT(EqualBy(EBFul, '1', '2', '3'));

        CHECK_CLJONIC(EqualBy(EBFull, '1'));
        CHECK_CLJONIC(EqualBy(EBFull, '1', '1', '1'));
        CHECK_CLJONIC_NOT(EqualBy(EBFull, '1', '2', '3'));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFc, 1));

        CHECK_CLJONIC(EqualBy(EBFi, 1));
        CHECK_CLJONIC(EqualBy(EBFi, 1, 1, 1));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, 1, 2, 3));

        CHECK_CLJONIC(EqualBy(EBFl, 1));
        CHECK_CLJONIC(EqualBy(EBFl, 1, 1, 1));
        CHECK_CLJONIC_NOT(EqualBy(EBFl, 1, 2, 3));

        CHECK_CLJONIC(EqualBy(EBFll, 1));
        CHECK_CLJONIC(EqualBy(EBFll, 1, 1, 1));
        CHECK_CLJONIC_NOT(EqualBy(EBFll, 1, 2, 3));

        CHECK_CLJONIC(EqualBy(EBFu, 1));
        CHECK_CLJONIC(EqualBy(EBFu, 1, 1, 1));
        CHECK_CLJONIC_NOT(EqualBy(EBFu, 1, 2, 3));

        CHECK_CLJONIC(EqualBy(EBFul, 1));
        CHECK_CLJONIC(EqualBy(EBFul, 1, 1, 1));
        CHECK_CLJONIC_NOT(EqualBy(EBFul, 1, 2, 3));

        CHECK_CLJONIC(EqualBy(EBFull, 1));
        CHECK_CLJONIC(EqualBy(EBFull, 1, 1, 1));
        CHECK_CLJONIC_NOT(EqualBy(EBFull, 1, 2, 3));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFc, 1L));
        CHECK_CLJONIC(EqualBy(EBFi, 1L));

        CHECK_CLJONIC(EqualBy(EBFl, 1L));
        CHECK_CLJONIC(EqualBy(EBFl, 1L, 1L, 1L));
        CHECK_CLJONIC_NOT(EqualBy(EBFl, 1L, 2L, 3L));

        CHECK_CLJONIC(EqualBy(EBFll, 1L));
        CHECK_CLJONIC(EqualBy(EBFll, 1L, 1L, 1L));
        CHECK_CLJONIC_NOT(EqualBy(EBFll, 1L, 2L, 3L));

        CHECK_CLJONIC(EqualBy(EBFu, 1L));

        CHECK_CLJONIC(EqualBy(EBFul, 1L));
        CHECK_CLJONIC(EqualBy(EBFul, 1L, 1L, 1L));
        CHECK_CLJONIC_NOT(EqualBy(EBFul, 1L, 2L, 3L));

        CHECK_CLJONIC(EqualBy(EBFull, 1L));
        CHECK_CLJONIC(EqualBy(EBFull, 1L, 1L, 1L));
        CHECK_CLJONIC_NOT(EqualBy(EBFull, 1L, 2L, 3L));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFc, 1LL));
        CHECK_CLJONIC(EqualBy(EBFi, 1LL));

        CHECK_CLJONIC(EqualBy(EBFl, 1LL));
        CHECK_CLJONIC(EqualBy(EBFl, 1LL, 1LL, 1LL));
        CHECK_CLJONIC_NOT(EqualBy(EBFl, 1LL, 2LL, 3LL));

        CHECK_CLJONIC(EqualBy(EBFll, 1LL));
        CHECK_CLJONIC(EqualBy(EBFll, 1LL, 1LL, 1LL));
        CHECK_CLJONIC_NOT(EqualBy(EBFll, 1LL, 2LL, 3LL));

        CHECK_CLJONIC(EqualBy(EBFu, 1LL));

        CHECK_CLJONIC(EqualBy(EBFul, 1LL));
        CHECK_CLJONIC(EqualBy(EBFul, 1LL, 1LL, 1LL));
        CHECK_CLJONIC_NOT(EqualBy(EBFul, 1LL, 2LL, 3LL));

        CHECK_CLJONIC(EqualBy(EBFull, 1LL));
        CHECK_CLJONIC(EqualBy(EBFull, 1LL, 1LL, 1LL));
        CHECK_CLJONIC_NOT(EqualBy(EBFull, 1LL, 2LL, 3LL));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFc, 1U));

        CHECK_CLJONIC(EqualBy(EBFi, 1U));
        CHECK_CLJONIC(EqualBy(EBFi, 1U, 1U, 1U));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, 1U, 2U, 3U));

        CHECK_CLJONIC(EqualBy(EBFl, 1U));
        CHECK_CLJONIC(EqualBy(EBFl, 1U, 1U, 1U));
        CHECK_CLJONIC_NOT(EqualBy(EBFl, 1U, 2U, 3U));

        CHECK_CLJONIC(EqualBy(EBFll, 1U));
        CHECK_CLJONIC(EqualBy(EBFll, 1U, 1U, 1U));
        CHECK_CLJONIC_NOT(EqualBy(EBFll, 1U, 2U, 3U));

        CHECK_CLJONIC(EqualBy(EBFu, 1U));
        CHECK_CLJONIC(EqualBy(EBFu, 1U, 1U, 1U));
        CHECK_CLJONIC_NOT(EqualBy(EBFu, 1U, 2U, 3U));

        CHECK_CLJONIC(EqualBy(EBFul, 1U));
        CHECK_CLJONIC(EqualBy(EBFul, 1U, 1U, 1U));
        CHECK_CLJONIC_NOT(EqualBy(EBFul, 1U, 2U, 3U));

        CHECK_CLJONIC(EqualBy(EBFull, 1U));
        CHECK_CLJONIC(EqualBy(EBFull, 1U, 1U, 1U));
        CHECK_CLJONIC_NOT(EqualBy(EBFull, 1U, 2U, 3U));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFc, 1UL));
        CHECK_CLJONIC(EqualBy(EBFi, 1UL));

        CHECK_CLJONIC(EqualBy(EBFl, 1UL));
        CHECK_CLJONIC(EqualBy(EBFl, 1UL, 1UL, 1UL));
        CHECK_CLJONIC_NOT(EqualBy(EBFl, 1UL, 2UL, 3UL));

        CHECK_CLJONIC(EqualBy(EBFll, 1UL));
        CHECK_CLJONIC(EqualBy(EBFll, 1UL, 1UL, 1UL));
        CHECK_CLJONIC_NOT(EqualBy(EBFll, 1UL, 2UL, 3UL));

        CHECK_CLJONIC(EqualBy(EBFu, 1UL));

        CHECK_CLJONIC(EqualBy(EBFul, 1UL));
        CHECK_CLJONIC(EqualBy(EBFul, 1UL, 1UL, 1UL));
        CHECK_CLJONIC_NOT(EqualBy(EBFul, 1UL, 2UL, 3UL));

        CHECK_CLJONIC(EqualBy(EBFull, 1UL));
        CHECK_CLJONIC(EqualBy(EBFull, 1UL, 1UL, 1UL));
        CHECK_CLJONIC_NOT(EqualBy(EBFull, 1UL, 2UL, 3UL));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFc, 1ULL));
        CHECK_CLJONIC(EqualBy(EBFi, 1ULL));

        CHECK_CLJONIC(EqualBy(EBFl, 1ULL));
        CHECK_CLJONIC(EqualBy(EBFl, 1ULL, 1ULL, 1ULL));
        CHECK_CLJONIC_NOT(EqualBy(EBFl, 1ULL, 2ULL, 3ULL));

        CHECK_CLJONIC(EqualBy(EBFll, 1ULL));
        CHECK_CLJONIC(EqualBy(EBFll, 1ULL, 1ULL, 1ULL));
        CHECK_CLJONIC_NOT(EqualBy(EBFll, 1ULL, 2ULL, 3ULL));

        CHECK_CLJONIC(EqualBy(EBFu, 1ULL));

        CHECK_CLJONIC(EqualBy(EBFul, 1ULL));
        CHECK_CLJONIC(EqualBy(EBFul, 1ULL, 1ULL, 1ULL));
        CHECK_CLJONIC_NOT(EqualBy(EBFul, 1ULL, 2ULL, 3ULL));

        CHECK_CLJONIC(EqualBy(EBFull, 1ULL));
        CHECK_CLJONIC(EqualBy(EBFull, 1ULL, 1ULL, 1ULL));
        CHECK_CLJONIC_NOT(EqualBy(EBFull, 1ULL, 2ULL, 3ULL));
    }

    {
        auto c{1};
        CHECK_CLJONIC(EqualBy(EBFi, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, 1, 1));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, c, 2, 3));
    }

    {
        auto c{1L};
        CHECK_CLJONIC(EqualBy(EBFl, c));
        CHECK_CLJONIC(EqualBy(EBFl, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFl, c, 1L, 1L));
        CHECK_CLJONIC_NOT(EqualBy(EBFl, c, 2L, 3L));
    }

    {
        auto c{1LL};
        CHECK_CLJONIC(EqualBy(EBFll, c));
        CHECK_CLJONIC(EqualBy(EBFll, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFll, c, 1LL, 1LL));
        CHECK_CLJONIC_NOT(EqualBy(EBFll, c, 2LL, 3LL));
    }

    {
        auto c{1U};
        CHECK_CLJONIC(EqualBy(EBFu, c));
        CHECK_CLJONIC(EqualBy(EBFu, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFu, c, 1U, 1U));
        CHECK_CLJONIC_NOT(EqualBy(EBFu, c, 2U, 3U));
    }

    {
        auto c{1UL};
        CHECK_CLJONIC(EqualBy(EBFul, c));
        CHECK_CLJONIC(EqualBy(EBFul, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFul, c, 1UL, 1UL));
        CHECK_CLJONIC_NOT(EqualBy(EBFul, c, 2UL, 3UL));
    }

    {
        auto c{1ULL};
        CHECK_CLJONIC(EqualBy(EBFull, c));
        CHECK_CLJONIC(EqualBy(EBFull, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFull, c, 1ULL, 1ULL));
        CHECK_CLJONIC_NOT(EqualBy(EBFull, c, 2ULL, 3ULL));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFi, Array<int, 3>{1, 2, 3}));
        CHECK_CLJONIC(EqualBy(EBFi, Array<int, 3>{1, 2, 3}, Array<int, 13>{1, 2, 3}, Array<int, 5>{1, 2, 3}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Array<int, 3>{1, 2, 3}, Array<int, 13>{2, 2, 3}, Array<int, 5>{3, 2, 3}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFl, Array<long, 3>{1L, 2L, 3L}));
        CHECK_CLJONIC(
            true == EqualBy(EBFl, Array<long, 3>{1L, 2L, 3L}, Array<long, 13>{1L, 2L, 3L}, Array<long, 5>{1L, 2L, 3L}));
        CHECK_CLJONIC(
            false ==
            EqualBy(EBFl, Array<long, 3>{1L, 2L, 3L}, Array<long, 13>{2L, 2L, 3L}, Array<long, 5>{3L, 2L, 3L}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFll, Array<long long, 3>{1LL, 2LL, 3LL}));
        CHECK_CLJONIC(EqualBy(EBFll,
                              Array<long long, 3>{1LL, 2LL, 3LL},
                              Array<long long, 13>{1LL, 2LL, 3LL},
                              Array<long long, 5>{1LL, 2LL, 3LL}));
        CHECK_CLJONIC_NOT(EqualBy(EBFll,
                                  Array<long long, 3>{1LL, 2LL, 3LL},
                                  Array<long long, 13>{2LL, 2LL, 3LL},
                                  Array<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFu, Array<unsigned, 3>{1U, 2U, 3U}));
        CHECK_CLJONIC(EqualBy(
            EBFu, Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 13>{1U, 2U, 3U}, Array<unsigned, 5>{1U, 2U, 3U}));
        CHECK_CLJONIC_NOT(EqualBy(
            EBFu, Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 13>{2U, 2U, 3U}, Array<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFul, Array<unsigned long, 3>{1UL, 2UL, 3UL}));
        CHECK_CLJONIC(EqualBy(EBFul,
                              Array<unsigned long, 3>{1UL, 2UL, 3UL},
                              Array<unsigned long, 13>{1UL, 2UL, 3UL},
                              Array<unsigned long, 5>{1UL, 2UL, 3UL}));
        CHECK_CLJONIC_NOT(EqualBy(EBFul,
                                  Array<unsigned long, 3>{1UL, 2UL, 3UL},
                                  Array<unsigned long, 13>{2UL, 2UL, 3UL},
                                  Array<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFull, Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL}));
        CHECK_CLJONIC(EqualBy(EBFull,
                              Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                              Array<unsigned long long, 13>{1ULL, 2ULL, 3ULL},
                              Array<unsigned long long, 5>{1ULL, 2ULL, 3ULL}));
        CHECK_CLJONIC_NOT(EqualBy(EBFull,
                                  Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                                  Array<unsigned long long, 13>{2ULL, 2ULL, 3ULL},
                                  Array<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        auto c{Array<int, 3>{1, 2, 3}};
        CHECK_CLJONIC(EqualBy(EBFi, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, Array<int, 3>{1, 2, 3}, Array<int, 33>{1, 2, 3}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, c, Array<int, 3>{2, 2, 3}, Array<int, 5>{3, 2, 3}));
    }

    {
        auto c{Array<long, 3>{1L, 2L, 3L}};
        CHECK_CLJONIC(EqualBy(EBFl, c));
        CHECK_CLJONIC(EqualBy(EBFl, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFl, c, Array<long, 3>{1L, 2L, 3L}, Array<long, 33>{1L, 2L, 3L}));
        CHECK_CLJONIC_NOT(EqualBy(EBFl, c, Array<long, 3>{2L, 2L, 3L}, Array<long, 5>{3L, 2L, 3L}));
    }

    {
        auto c{Array<long long, 3>{1LL, 2LL, 3LL}};
        CHECK_CLJONIC(EqualBy(EBFll, c));
        CHECK_CLJONIC(EqualBy(EBFll, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFll, c, Array<long long, 3>{1LL, 2LL, 3LL}, Array<long long, 33>{1LL, 2LL, 3LL}));
        CHECK_CLJONIC_NOT(EqualBy(EBFll, c, Array<long long, 3>{2LL, 2LL, 3LL}, Array<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        auto c{Array<unsigned, 3>{1U, 2U, 3U}};
        CHECK_CLJONIC(EqualBy(EBFu, c));
        CHECK_CLJONIC(EqualBy(EBFu, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFu, c, Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 33>{1U, 2U, 3U}));
        CHECK_CLJONIC_NOT(EqualBy(EBFu, c, Array<unsigned, 3>{2U, 2U, 3U}, Array<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        auto c{Array<unsigned long, 3>{1UL, 2UL, 3UL}};
        CHECK_CLJONIC(EqualBy(EBFul, c));
        CHECK_CLJONIC(EqualBy(EBFul, c, c, c));
        CHECK_CLJONIC(
            true == EqualBy(EBFul, c, Array<unsigned long, 3>{1UL, 2UL, 3UL}, Array<unsigned long, 33>{1UL, 2UL, 3UL}));
        CHECK_CLJONIC(
            false == EqualBy(EBFul, c, Array<unsigned long, 3>{2UL, 2UL, 3UL}, Array<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        auto c{Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL}};
        CHECK_CLJONIC(EqualBy(EBFull, c));
        CHECK_CLJONIC(EqualBy(EBFull, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFull,
                              c,
                              Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                              Array<unsigned long long, 33>{1ULL, 2ULL, 3ULL}));
        CHECK_CLJONIC_NOT(EqualBy(
            EBFull, c, Array<unsigned long long, 3>{2ULL, 2ULL, 3ULL}, Array<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFccp, Array<const char*, 3>{"1", "2"}));
        CHECK_CLJONIC(EqualBy(EBFccp, Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"1", "2"}));
        CHECK_CLJONIC_NOT(EqualBy(EBFccp, Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"2", "2"}));
    }

    {
        auto c{Array<const char*, 3>{"1", "2"}};
        CHECK_CLJONIC(EqualBy(EBFccp, c));
        CHECK_CLJONIC(EqualBy(EBFccp, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFccp, c, Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"1", "2"}));
        CHECK_CLJONIC_NOT(EqualBy(EBFccp, c, Array<const char*, 3>{"2", "2"}, Array<const char*, 3>{"3", "2"}));
    }

    {
        auto c{Array{"1", "2"}};
        CHECK_CLJONIC(EqualBy(EBFccp, c));
        CHECK_CLJONIC(EqualBy(EBFccp, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFccp, c, Array{"1", "2"}, Array{"1", "2"}));
        CHECK_CLJONIC_NOT(EqualBy(EBFccp, c, Array{"2", "2"}, Array{"3", "2"}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFi, Set<int, 3>{1, 2, 3}));
        CHECK_CLJONIC(EqualBy(EBFi, Set<int, 3>{1, 2, 3}, Set<int, 3>{1, 2, 3}, Set<int, 3>{1, 2, 3}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Set<int, 3>{1, 2, 3}, Set<int, 3>{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        auto c{Set<int, 3>{1, 2, 3}};
        CHECK_CLJONIC(EqualBy(EBFi, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, Set<int, 3>{1, 2, 3}, Set<int, 33>{1, 2, 3}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, c, Set<int, 3>{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFi, Set{1, 2, 3}));
        CHECK_CLJONIC(EqualBy(EBFi, Set{1, 2, 3}, Set{1, 2, 3}, Set{1, 2, 3}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Set{1, 2, 3}, Set{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        auto c{Set{1, 2, 3}};
        CHECK_CLJONIC(EqualBy(EBFi, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, Set{1, 2, 3}, Set<int, 33>{1, 2, 3}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, c, Set{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFccp, Set<const char*, 3>{"1", "2", "3"}));
        CHECK_CLJONIC(EqualBy(EBFccp,
                              Set<const char*, 3>{"1", "2", "3"},
                              Set<const char*, 3>{"1", "2", "3"},
                              Set<const char*, 3>{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(EqualBy(EBFccp,
                                  Set<const char*, 3>{"1", "2", "3"},
                                  Set<const char*, 3>{"2", "2", "3"},
                                  Set<const char*, 3>{"3", "2", "3"}));
    }

    {
        auto c{Set<const char*, 3>{"1", "2", "3"}};
        CHECK_CLJONIC(EqualBy(EBFccp, c));
        CHECK_CLJONIC(EqualBy(EBFccp, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFccp, c, Set<const char*, 3>{"1", "2", "3"}, Set<const char*, 33>{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(EqualBy(EBFccp, c, Set<const char*, 3>{"2", "2", "3"}, Set<const char*, 5>{"3", "2", "3"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK_CLJONIC(EqualBy(EBFc, String<3>{"abc"}));
        CHECK_CLJONIC(EqualBy(EBFc, String<3>{"abc"}, String<3>{"abc"}, String<3>{"abc"}));
        CHECK_CLJONIC(EqualBy(EBFc, String<3>{"abc"}, String<3>{"abc"}, String<13>{"abc"}));
        CHECK_CLJONIC_NOT(EqualBy(EBFc, String<3>{"abc"}, String<3>{"abc"}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK_CLJONIC(EqualBy(EBFc, c));
        CHECK_CLJONIC(EqualBy(EBFc, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFc, c, String<3>{"abc"}, String<13>{"abc"}));
        CHECK_CLJONIC_NOT(EqualBy(EBFc, c, String<3>{"abc"}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{'a', 'b', 'c'}};
        CHECK_CLJONIC(EqualBy(EBFc, String<3>{'a', 'b', 'c'}));
        CHECK_CLJONIC(EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}));
        CHECK_CLJONIC(EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK_CLJONIC_NOT(EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{'a', 'b', 'c'}};
        CHECK_CLJONIC(EqualBy(EBFc, c));
        CHECK_CLJONIC(EqualBy(EBFc, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFc, c, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK_CLJONIC_NOT(EqualBy(EBFc, c, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK_CLJONIC(EqualBy(EBFc, String<3>{'a', 'b', 'c'}));
        CHECK_CLJONIC(EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}));
        CHECK_CLJONIC(EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK_CLJONIC_NOT(EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK_CLJONIC(EqualBy(EBFc, c));
        CHECK_CLJONIC(EqualBy(EBFc, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFc, c, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK_CLJONIC_NOT(EqualBy(EBFc, c, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFi, Range<>{}));
        CHECK_CLJONIC(EqualBy(EBFi, Range<>{}, Range<>{}, Range<>{}));
        CHECK_CLJONIC(EqualBy(EBFi, Range<10>{}, Range<10>{}, Range<10>{}));
        CHECK_CLJONIC(EqualBy(EBFi, Range<-5, 10>{}, Range<-5, 10>{}, Range<-5, 10>{}));
        CHECK_CLJONIC(EqualBy(EBFi, Range<-5, 10, 3>{}, Range<-5, 10, 3>{}, Range<-5, 10, 3>{}));
        CHECK_CLJONIC(EqualBy(EBFi, Range<5, -10, -2>{}, Range<5, -10, -2>{}, Range<5, -10, -2>{}));
        CHECK_CLJONIC(EqualBy(EBFi, Range<5, 5, 0>{}, Range<5, 5, 0>{}, Range<5, 5, 0>{}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Range<5, 5, 0>{}, Range<5, 15, 0>{}, Range<5, 5, 0>{}));
    }

    {
        auto c{Range<10>{}};
        CHECK_CLJONIC(EqualBy(EBFi, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, Range<10>{}, Range<10>{}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, c, Range<10>{}, Range<100>{}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFi, Repeat{1}));
        CHECK_CLJONIC(EqualBy(EBFi, Repeat{1}, Repeat{1}));
        CHECK_CLJONIC(EqualBy(EBFi, Repeat<10, int>{1}, Repeat<10, int>{1}, Repeat<10, int>{1}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Repeat<10, int>{1}, Repeat<10, int>{2}, Repeat<10, int>{1}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Repeat<10, int>{1}, Repeat{1}, Repeat<10, int>{1}));
    }

    {
        auto c{Repeat<10, int>{1}};
        CHECK_CLJONIC(EqualBy(EBFi, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, c, c));
        CHECK_CLJONIC(EqualBy(EBFi, c, Repeat<10, int>{1}, Repeat<10, int>{1}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, c, Repeat<10, int>{1}, Repeat<100, int>{'x'}));
    }

    {
        CHECK_CLJONIC(EqualBy(EBFi, Range<5>{}, Array{0, 1, 2, 3, 4}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Range<10>{}, Array{0, 1, 2, 3, 4}));
        CHECK_CLJONIC(EqualBy(EBFi, Array{0, 1, 2, 3, 4}, Range<0, 5, 1>{}, Array{0, 1, 2, 3, 4}));
        CHECK_CLJONIC(EqualBy(EBFi, Array{5, 3, 1}, Range<5, 0, -2>{}));
        CHECK_CLJONIC(EqualBy(EBFi, Range<1>{}, Array{0}, Repeat<1, int>{0}));
        CHECK_CLJONIC(EqualBy(EBFi, Array{4, 4, 4, 4}, Repeat<4, int>{4}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Range<2>{}, Array{0}, Repeat<1, int>{0}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Range<1>{}, Array{0, 1}, Repeat<1, int>{0}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Range<1>{}, Array{0}, Repeat<2, int>{0}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Array{4, 4, 4}, Repeat<4, int>{4}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Array{4, 4, 4, 4}, Repeat<4, int>{5}));
    }

    {
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Array{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC_NOT(EqualBy(EBFi, Array{1, 2, 3}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(EqualBy(
            EBFi, Iterator{[](const int i) { return i + 1; }, 0}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC_NOT(EqualBy(
            EBFi, Iterator{[](const int i) { return i + 1; }, 1}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(EqualBy(EBFi, Range{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(EqualBy(EBFi,
                              Range<10, CljonicCollectionMaximumElementCount + 10>{},
                              Iterator{[](const int i) { return i + 1; }, 10}));
        CHECK_CLJONIC(EqualBy(EBFi, Repeat{1}, Iterator{[]([[maybe_unused]] const int _) { return 1; }, 1}));
        CHECK_CLJONIC_NOT(EqualBy(EBFc, String{}, Iterator{[]([[maybe_unused]] const char _) { return 'a'; }, 'b'}));
        CHECK_CLJONIC_NOT(
            EqualBy(EBFc, String{"Hello"}, Iterator{[]([[maybe_unused]] const char _) { return 'a'; }, 'b'}));
    }

    DisableNoHeapMessagePrinting();
}
