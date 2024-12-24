#include <tuple>
#include "catch.hpp"
#include "cljonic-array.hpp"
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

template <typename T>
auto EBFInvalidReturnType()
{
    return [](const T a, const T b) { return std::make_tuple(a, b); };
}

const auto EBFInvalidReturnTypei = EBFInvalidReturnType<int>();

template <typename T>
auto EBFInvalidParameterType()
{
    return [](const T a, const std::tuple<int, int>& b)
    {
        auto [x, y]{b};
        return a == x == y;
    };
}

const auto EBFInvalidParameterTypei = EBFInvalidParameterType<int>();

SCENARIO("EqualBy", "[CljonicCoreEqualBy]")
{
    // all of the following tests are commented out because they will generate expected compiler errors
    // because the functions passed to EqualBy are not valid
    // {
    //     CHECK(true == EqualBy(EBFInvalidReturnTypei, '1', '1'));
    //     CHECK(true == EqualBy(EBFInvalidParameterTypei, '1', '1'));
    // }

    // the following test is commented out because it will generate expected compiler error
    // because the type of the single parameter involves floating point
    // {
    //     CHECK(true == EqualBy(EBFc, 1.1f));
    //     CHECK(true == EqualBy(EBFc, 1.1));
    // }

    {
        CHECK(true == EqualBy(EBFc, '1'));
        CHECK(true == EqualBy(EBFc, '1', '1', '1'));
        CHECK(false == EqualBy(EBFc, '1', '2', '3'));

        CHECK(true == EqualBy(EBFi, '1'));
        CHECK(true == EqualBy(EBFi, '1', '1', '1'));
        CHECK(false == EqualBy(EBFi, '1', '2', '3'));

        CHECK(true == EqualBy(EBFl, '1'));
        CHECK(true == EqualBy(EBFl, '1', '1', '1'));
        CHECK(false == EqualBy(EBFl, '1', '2', '3'));

        CHECK(true == EqualBy(EBFll, '1'));
        CHECK(true == EqualBy(EBFll, '1', '1', '1'));
        CHECK(false == EqualBy(EBFll, '1', '2', '3'));

        CHECK(true == EqualBy(EBFu, '1'));
        CHECK(true == EqualBy(EBFu, '1', '1', '1'));
        CHECK(false == EqualBy(EBFu, '1', '2', '3'));

        CHECK(true == EqualBy(EBFul, '1'));
        CHECK(true == EqualBy(EBFul, '1', '1', '1'));
        CHECK(false == EqualBy(EBFul, '1', '2', '3'));

        CHECK(true == EqualBy(EBFull, '1'));
        CHECK(true == EqualBy(EBFull, '1', '1', '1'));
        CHECK(false == EqualBy(EBFull, '1', '2', '3'));
    }

    {
        CHECK(true == EqualBy(EBFc, 1)); // EqualBy returns true because there's only one element to compare

        CHECK(true == EqualBy(EBFi, 1));
        CHECK(true == EqualBy(EBFi, 1, 1, 1));
        CHECK(false == EqualBy(EBFi, 1, 2, 3));

        CHECK(true == EqualBy(EBFl, 1));
        CHECK(true == EqualBy(EBFl, 1, 1, 1));
        CHECK(false == EqualBy(EBFl, 1, 2, 3));

        CHECK(true == EqualBy(EBFll, 1));
        CHECK(true == EqualBy(EBFll, 1, 1, 1));
        CHECK(false == EqualBy(EBFll, 1, 2, 3));

        CHECK(true == EqualBy(EBFu, 1));
        CHECK(true == EqualBy(EBFu, 1, 1, 1));
        CHECK(false == EqualBy(EBFu, 1, 2, 3));

        CHECK(true == EqualBy(EBFul, 1));
        CHECK(true == EqualBy(EBFul, 1, 1, 1));
        CHECK(false == EqualBy(EBFul, 1, 2, 3));

        CHECK(true == EqualBy(EBFull, 1));
        CHECK(true == EqualBy(EBFull, 1, 1, 1));
        CHECK(false == EqualBy(EBFull, 1, 2, 3));
    }

    {
        CHECK(true == EqualBy(EBFc, 1L)); // EqualBy returns true because there's only one element to compare
        CHECK(true == EqualBy(EBFi, 1L));

        CHECK(true == EqualBy(EBFl, 1L));
        CHECK(true == EqualBy(EBFl, 1L, 1L, 1L));
        CHECK(false == EqualBy(EBFl, 1L, 2L, 3L));

        CHECK(true == EqualBy(EBFll, 1L));
        CHECK(true == EqualBy(EBFll, 1L, 1L, 1L));
        CHECK(false == EqualBy(EBFll, 1L, 2L, 3L));

        CHECK(true == EqualBy(EBFu, 1L));

        CHECK(true == EqualBy(EBFul, 1L));
        CHECK(true == EqualBy(EBFul, 1L, 1L, 1L));
        CHECK(false == EqualBy(EBFul, 1L, 2L, 3L));

        CHECK(true == EqualBy(EBFull, 1L));
        CHECK(true == EqualBy(EBFull, 1L, 1L, 1L));
        CHECK(false == EqualBy(EBFull, 1L, 2L, 3L));
    }

    {
        CHECK(true == EqualBy(EBFc, 1LL)); // EqualBy returns true because there's only one element to compare
        CHECK(true == EqualBy(EBFi, 1LL));

        CHECK(true == EqualBy(EBFl, 1LL));
        CHECK(true == EqualBy(EBFl, 1LL, 1LL, 1LL));
        CHECK(false == EqualBy(EBFl, 1LL, 2LL, 3LL));

        CHECK(true == EqualBy(EBFll, 1LL));
        CHECK(true == EqualBy(EBFll, 1LL, 1LL, 1LL));
        CHECK(false == EqualBy(EBFll, 1LL, 2LL, 3LL));

        CHECK(true == EqualBy(EBFu, 1LL));

        CHECK(true == EqualBy(EBFul, 1LL));
        CHECK(true == EqualBy(EBFul, 1LL, 1LL, 1LL));
        CHECK(false == EqualBy(EBFul, 1LL, 2LL, 3LL));

        CHECK(true == EqualBy(EBFull, 1LL));
        CHECK(true == EqualBy(EBFull, 1LL, 1LL, 1LL));
        CHECK(false == EqualBy(EBFull, 1LL, 2LL, 3LL));
    }

    {
        CHECK(true == EqualBy(EBFc, 1U)); // EqualBy returns true because there's only one element to compare

        CHECK(true == EqualBy(EBFi, 1U));
        CHECK(true == EqualBy(EBFi, 1U, 1U, 1U));
        CHECK(false == EqualBy(EBFi, 1U, 2U, 3U));

        CHECK(true == EqualBy(EBFl, 1U));
        CHECK(true == EqualBy(EBFl, 1U, 1U, 1U));
        CHECK(false == EqualBy(EBFl, 1U, 2U, 3U));

        CHECK(true == EqualBy(EBFll, 1U));
        CHECK(true == EqualBy(EBFll, 1U, 1U, 1U));
        CHECK(false == EqualBy(EBFll, 1U, 2U, 3U));

        CHECK(true == EqualBy(EBFu, 1U));
        CHECK(true == EqualBy(EBFu, 1U, 1U, 1U));
        CHECK(false == EqualBy(EBFu, 1U, 2U, 3U));

        CHECK(true == EqualBy(EBFul, 1U));
        CHECK(true == EqualBy(EBFul, 1U, 1U, 1U));
        CHECK(false == EqualBy(EBFul, 1U, 2U, 3U));

        CHECK(true == EqualBy(EBFull, 1U));
        CHECK(true == EqualBy(EBFull, 1U, 1U, 1U));
        CHECK(false == EqualBy(EBFull, 1U, 2U, 3U));
    }

    {
        CHECK(true == EqualBy(EBFc, 1UL)); // EqualBy returns true because there's only one element to compare
        CHECK(true == EqualBy(EBFi, 1UL));

        CHECK(true == EqualBy(EBFl, 1UL));
        CHECK(true == EqualBy(EBFl, 1UL, 1UL, 1UL));
        CHECK(false == EqualBy(EBFl, 1UL, 2UL, 3UL));

        CHECK(true == EqualBy(EBFll, 1UL));
        CHECK(true == EqualBy(EBFll, 1UL, 1UL, 1UL));
        CHECK(false == EqualBy(EBFll, 1UL, 2UL, 3UL));

        CHECK(true == EqualBy(EBFu, 1UL));

        CHECK(true == EqualBy(EBFul, 1UL));
        CHECK(true == EqualBy(EBFul, 1UL, 1UL, 1UL));
        CHECK(false == EqualBy(EBFul, 1UL, 2UL, 3UL));

        CHECK(true == EqualBy(EBFull, 1UL));
        CHECK(true == EqualBy(EBFull, 1UL, 1UL, 1UL));
        CHECK(false == EqualBy(EBFull, 1UL, 2UL, 3UL));
    }

    {
        CHECK(true == EqualBy(EBFc, 1ULL)); // EqualBy returns true because there's only one element to compare
        CHECK(true == EqualBy(EBFi, 1ULL));

        CHECK(true == EqualBy(EBFl, 1ULL));
        CHECK(true == EqualBy(EBFl, 1ULL, 1ULL, 1ULL));
        CHECK(false == EqualBy(EBFl, 1ULL, 2ULL, 3ULL));

        CHECK(true == EqualBy(EBFll, 1ULL));
        CHECK(true == EqualBy(EBFll, 1ULL, 1ULL, 1ULL));
        CHECK(false == EqualBy(EBFll, 1ULL, 2ULL, 3ULL));

        CHECK(true == EqualBy(EBFu, 1ULL));

        CHECK(true == EqualBy(EBFul, 1ULL));
        CHECK(true == EqualBy(EBFul, 1ULL, 1ULL, 1ULL));
        CHECK(false == EqualBy(EBFul, 1ULL, 2ULL, 3ULL));

        CHECK(true == EqualBy(EBFull, 1ULL));
        CHECK(true == EqualBy(EBFull, 1ULL, 1ULL, 1ULL));
        CHECK(false == EqualBy(EBFull, 1ULL, 2ULL, 3ULL));
    }

    {
        auto c{1};
        CHECK(true == EqualBy(EBFi, c));
        CHECK(true == EqualBy(EBFi, c, c, c));
        CHECK(true == EqualBy(EBFi, c, 1, 1));
        CHECK(false == EqualBy(EBFi, c, 2, 3));
    }

    {
        auto c{1L};
        CHECK(true == EqualBy(EBFl, c));
        CHECK(true == EqualBy(EBFl, c, c, c));
        CHECK(true == EqualBy(EBFl, c, 1L, 1L));
        CHECK(false == EqualBy(EBFl, c, 2L, 3L));
    }

    {
        auto c{1LL};
        CHECK(true == EqualBy(EBFll, c));
        CHECK(true == EqualBy(EBFll, c, c, c));
        CHECK(true == EqualBy(EBFll, c, 1LL, 1LL));
        CHECK(false == EqualBy(EBFll, c, 2LL, 3LL));
    }

    {
        auto c{1U};
        CHECK(true == EqualBy(EBFu, c));
        CHECK(true == EqualBy(EBFu, c, c, c));
        CHECK(true == EqualBy(EBFu, c, 1U, 1U));
        CHECK(false == EqualBy(EBFu, c, 2U, 3U));
    }

    {
        auto c{1UL};
        CHECK(true == EqualBy(EBFul, c));
        CHECK(true == EqualBy(EBFul, c, c, c));
        CHECK(true == EqualBy(EBFul, c, 1UL, 1UL));
        CHECK(false == EqualBy(EBFul, c, 2UL, 3UL));
    }

    {
        auto c{1ULL};
        CHECK(true == EqualBy(EBFull, c));
        CHECK(true == EqualBy(EBFull, c, c, c));
        CHECK(true == EqualBy(EBFull, c, 1ULL, 1ULL));
        CHECK(false == EqualBy(EBFull, c, 2ULL, 3ULL));
    }

    // all of the following tests are commented out because they will generate expected compiler errors
    // due to the fact that the EqualBy function knows that equality comparison of floating point types is not exact
    // {
    //     constexpr auto EBFd = [](const double a, const double b) { return a == b; };
    //     CHECK(true == EqualBy(EBFd, 1.1));
    //     CHECK(true == EqualBy(EBFd, 1.1, 1.1, 1.1));
    //     CHECK(false == EqualBy(EBFd, 1, 1.0, 1));
    // }

    // {
    //     CHECK(true == EqualBy(Array<char, 3>{'1', '2', '3'}));
    //     CHECK(true ==
    //           EqualBy(Array<char, 3>{'1', '2', '3'}, Array<char, 13>{'1', '2', '3'}, Array<char, 5>{'1', '2', '3'}));
    //     CHECK(false ==
    //           EqualBy(Array<char, 3>{'1', '2', '3'}, Array<char, 13>{'2', '2', '3'}, Array<char, 5>{'3', '2', '3'}));
    // }

    {
        CHECK(true == EqualBy(EBFi, Array<int, 3>{1, 2, 3}));
        CHECK(true == EqualBy(EBFi, Array<int, 3>{1, 2, 3}, Array<int, 13>{1, 2, 3}, Array<int, 5>{1, 2, 3}));
        CHECK(false == EqualBy(EBFi, Array<int, 3>{1, 2, 3}, Array<int, 13>{2, 2, 3}, Array<int, 5>{3, 2, 3}));
    }

    {
        CHECK(true == EqualBy(EBFl, Array<long, 3>{1L, 2L, 3L}));
        CHECK(true ==
              EqualBy(EBFl, Array<long, 3>{1L, 2L, 3L}, Array<long, 13>{1L, 2L, 3L}, Array<long, 5>{1L, 2L, 3L}));
        CHECK(false ==
              EqualBy(EBFl, Array<long, 3>{1L, 2L, 3L}, Array<long, 13>{2L, 2L, 3L}, Array<long, 5>{3L, 2L, 3L}));
    }

    {
        CHECK(true == EqualBy(EBFll, Array<long long, 3>{1LL, 2LL, 3LL}));
        CHECK(true == EqualBy(EBFll,
                              Array<long long, 3>{1LL, 2LL, 3LL},
                              Array<long long, 13>{1LL, 2LL, 3LL},
                              Array<long long, 5>{1LL, 2LL, 3LL}));
        CHECK(false == EqualBy(EBFll,
                               Array<long long, 3>{1LL, 2LL, 3LL},
                               Array<long long, 13>{2LL, 2LL, 3LL},
                               Array<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        CHECK(true == EqualBy(EBFu, Array<unsigned, 3>{1U, 2U, 3U}));
        CHECK(true == EqualBy(EBFu,
                              Array<unsigned, 3>{1U, 2U, 3U},
                              Array<unsigned, 13>{1U, 2U, 3U},
                              Array<unsigned, 5>{1U, 2U, 3U}));
        CHECK(false == EqualBy(EBFu,
                               Array<unsigned, 3>{1U, 2U, 3U},
                               Array<unsigned, 13>{2U, 2U, 3U},
                               Array<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        CHECK(true == EqualBy(EBFul, Array<unsigned long, 3>{1UL, 2UL, 3UL}));
        CHECK(true == EqualBy(EBFul,
                              Array<unsigned long, 3>{1UL, 2UL, 3UL},
                              Array<unsigned long, 13>{1UL, 2UL, 3UL},
                              Array<unsigned long, 5>{1UL, 2UL, 3UL}));
        CHECK(false == EqualBy(EBFul,
                               Array<unsigned long, 3>{1UL, 2UL, 3UL},
                               Array<unsigned long, 13>{2UL, 2UL, 3UL},
                               Array<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        CHECK(true == EqualBy(EBFull, Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL}));
        CHECK(true == EqualBy(EBFull,
                              Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                              Array<unsigned long long, 13>{1ULL, 2ULL, 3ULL},
                              Array<unsigned long long, 5>{1ULL, 2ULL, 3ULL}));
        CHECK(false == EqualBy(EBFull,
                               Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                               Array<unsigned long long, 13>{2ULL, 2ULL, 3ULL},
                               Array<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        auto c{Array<int, 3>{1, 2, 3}};
        CHECK(true == EqualBy(EBFi, c));
        CHECK(true == EqualBy(EBFi, c, c, c));
        CHECK(true == EqualBy(EBFi, c, Array<int, 3>{1, 2, 3}, Array<int, 33>{1, 2, 3}));
        CHECK(false == EqualBy(EBFi, c, Array<int, 3>{2, 2, 3}, Array<int, 5>{3, 2, 3}));
    }

    {
        auto c{Array<long, 3>{1L, 2L, 3L}};
        CHECK(true == EqualBy(EBFl, c));
        CHECK(true == EqualBy(EBFl, c, c, c));
        CHECK(true == EqualBy(EBFl, c, Array<long, 3>{1L, 2L, 3L}, Array<long, 33>{1L, 2L, 3L}));
        CHECK(false == EqualBy(EBFl, c, Array<long, 3>{2L, 2L, 3L}, Array<long, 5>{3L, 2L, 3L}));
    }

    {
        auto c{Array<long long, 3>{1LL, 2LL, 3LL}};
        CHECK(true == EqualBy(EBFll, c));
        CHECK(true == EqualBy(EBFll, c, c, c));
        CHECK(true == EqualBy(EBFll, c, Array<long long, 3>{1LL, 2LL, 3LL}, Array<long long, 33>{1LL, 2LL, 3LL}));
        CHECK(false == EqualBy(EBFll, c, Array<long long, 3>{2LL, 2LL, 3LL}, Array<long long, 5>{3LL, 2LL, 3LL}));
    }

    {
        auto c{Array<unsigned, 3>{1U, 2U, 3U}};
        CHECK(true == EqualBy(EBFu, c));
        CHECK(true == EqualBy(EBFu, c, c, c));
        CHECK(true == EqualBy(EBFu, c, Array<unsigned, 3>{1U, 2U, 3U}, Array<unsigned, 33>{1U, 2U, 3U}));
        CHECK(false == EqualBy(EBFu, c, Array<unsigned, 3>{2U, 2U, 3U}, Array<unsigned, 5>{3U, 2U, 3U}));
    }

    {
        auto c{Array<unsigned long, 3>{1UL, 2UL, 3UL}};
        CHECK(true == EqualBy(EBFul, c));
        CHECK(true == EqualBy(EBFul, c, c, c));
        CHECK(true ==
              EqualBy(EBFul, c, Array<unsigned long, 3>{1UL, 2UL, 3UL}, Array<unsigned long, 33>{1UL, 2UL, 3UL}));
        CHECK(false ==
              EqualBy(EBFul, c, Array<unsigned long, 3>{2UL, 2UL, 3UL}, Array<unsigned long, 5>{3UL, 2UL, 3UL}));
    }

    {
        auto c{Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL}};
        CHECK(true == EqualBy(EBFull, c));
        CHECK(true == EqualBy(EBFull, c, c, c));
        CHECK(true == EqualBy(EBFull,
                              c,
                              Array<unsigned long long, 3>{1ULL, 2ULL, 3ULL},
                              Array<unsigned long long, 33>{1ULL, 2ULL, 3ULL}));
        CHECK(false == EqualBy(EBFull,
                               c,
                               Array<unsigned long long, 3>{2ULL, 2ULL, 3ULL},
                               Array<unsigned long long, 5>{3ULL, 2ULL, 3ULL}));
    }

    {
        CHECK(true == EqualBy(EBFccp, Array<const char*, 3>{"1", "2"}));
        CHECK(true == EqualBy(EBFccp, Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"1", "2"}));
        CHECK(false == EqualBy(EBFccp, Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"2", "2"}));
    }

    {
        auto c{Array<const char*, 3>{"1", "2"}};
        CHECK(true == EqualBy(EBFccp, c));
        CHECK(true == EqualBy(EBFccp, c, c, c));
        CHECK(true == EqualBy(EBFccp, c, Array<const char*, 3>{"1", "2"}, Array<const char*, 3>{"1", "2"}));
        CHECK(false == EqualBy(EBFccp, c, Array<const char*, 3>{"2", "2"}, Array<const char*, 3>{"3", "2"}));
    }

    {
        auto c{Array{"1", "2"}};
        CHECK(true == EqualBy(EBFccp, c));
        CHECK(true == EqualBy(EBFccp, c, c, c));
        CHECK(true == EqualBy(EBFccp, c, Array{"1", "2"}, Array{"1", "2"}));
        CHECK(false == EqualBy(EBFccp, c, Array{"2", "2"}, Array{"3", "2"}));
    }

    // all of the following tests are commented out because they will generate expected compiler errors
    // due to the fact that the Equal function knows that equality comparison of floating point types is not exact
    // {
    //     CHECK(true == EqualBy(EBFccp, Array<double, 3>{1.1, 2.1}));
    //     CHECK(true ==
    //           EqualBy(EBFd, Array<double, 3>{1.1, 2.1}, Array<double, 3>{1.1, 2.1}, Array<double, 3>{1.1, 2.1}));
    //     CHECK(true == EqualBy(EBFd, Array<int, 3>{1, 2}, Array<double, 3>{1.1, 2.1}, Array<int, 3>{1, 2}));
    //     CHECK(true == EqualBy(EBFi, Array<int, 3>{1, 2}, Array<double, 3>{1.1, 2.1}, Array<int, 3>{1, 2}));
    // }

    {
        CHECK(true == EqualBy(EBFi, Set<int, 3>{1, 2, 3}));
        CHECK(true == EqualBy(EBFi, Set<int, 3>{1, 2, 3}, Set<int, 3>{1, 2, 3}, Set<int, 3>{1, 2, 3}));
        CHECK(false == EqualBy(EBFi, Set<int, 3>{1, 2, 3}, Set<int, 3>{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        auto c{Set<int, 3>{1, 2, 3}};
        CHECK(true == EqualBy(EBFi, c));
        CHECK(true == EqualBy(EBFi, c, c, c));
        CHECK(true == EqualBy(EBFi, c, Set<int, 3>{1, 2, 3}, Set<int, 33>{1, 2, 3}));
        CHECK(false == EqualBy(EBFi, c, Set<int, 3>{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        CHECK(true == EqualBy(EBFi, Set{1, 2, 3}));
        CHECK(true == EqualBy(EBFi, Set{1, 2, 3}, Set{1, 2, 3}, Set{1, 2, 3}));
        CHECK(false == EqualBy(EBFi, Set{1, 2, 3}, Set{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        auto c{Set{1, 2, 3}};
        CHECK(true == EqualBy(EBFi, c));
        CHECK(true == EqualBy(EBFi, c, c, c));
        CHECK(true == EqualBy(EBFi, c, Set{1, 2, 3}, Set<int, 33>{1, 2, 3}));
        CHECK(false == EqualBy(EBFi, c, Set{2, 2, 3}, Set<int, 5>{3, 2, 3}));
    }

    {
        CHECK(true == EqualBy(EBFccp, Set<const char*, 3>{"1", "2", "3"}));
        CHECK(true == EqualBy(EBFccp,
                              Set<const char*, 3>{"1", "2", "3"},
                              Set<const char*, 3>{"1", "2", "3"},
                              Set<const char*, 3>{"1", "2", "3"}));
        CHECK(false == EqualBy(EBFccp,
                               Set<const char*, 3>{"1", "2", "3"},
                               Set<const char*, 3>{"2", "2", "3"},
                               Set<const char*, 3>{"3", "2", "3"}));
    }

    {
        auto c{Set<const char*, 3>{"1", "2", "3"}};
        CHECK(true == EqualBy(EBFccp, c));
        CHECK(true == EqualBy(EBFccp, c, c, c));
        CHECK(true == EqualBy(EBFccp, c, Set<const char*, 3>{"1", "2", "3"}, Set<const char*, 33>{"1", "2", "3"}));
        CHECK(false == EqualBy(EBFccp, c, Set<const char*, 3>{"2", "2", "3"}, Set<const char*, 5>{"3", "2", "3"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK(true == EqualBy(EBFc, String<3>{"abc"}));
        CHECK(true == EqualBy(EBFc, String<3>{"abc"}, String<3>{"abc"}, String<3>{"abc"}));
        CHECK(true == EqualBy(EBFc, String<3>{"abc"}, String<3>{"abc"}, String<13>{"abc"}));
        CHECK(false == EqualBy(EBFc, String<3>{"abc"}, String<3>{"abc"}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK(true == EqualBy(EBFc, c));
        CHECK(true == EqualBy(EBFc, c, c, c));
        CHECK(true == EqualBy(EBFc, c, String<3>{"abc"}, String<13>{"abc"}));
        CHECK(false == EqualBy(EBFc, c, String<3>{"abc"}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{'a', 'b', 'c'}};
        CHECK(true == EqualBy(EBFc, String<3>{'a', 'b', 'c'}));
        CHECK(true == EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}));
        CHECK(true == EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK(false == EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{'a', 'b', 'c'}};
        CHECK(true == EqualBy(EBFc, c));
        CHECK(true == EqualBy(EBFc, c, c, c));
        CHECK(true == EqualBy(EBFc, c, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK(false == EqualBy(EBFc, c, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK(true == EqualBy(EBFc, String<3>{'a', 'b', 'c'}));
        CHECK(true == EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}));
        CHECK(true == EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK(false == EqualBy(EBFc, String<3>{'a', 'b', 'c'}, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        auto c{String<3>{"abc"}};
        CHECK(true == EqualBy(EBFc, c));
        CHECK(true == EqualBy(EBFc, c, c, c));
        CHECK(true == EqualBy(EBFc, c, String<3>{'a', 'b', 'c'}, String<13>{'a', 'b', 'c'}));
        CHECK(false == EqualBy(EBFc, c, String<3>{'a', 'b', 'c'}, String<22>{"xbc"}));
    }

    {
        CHECK(true == EqualBy(EBFi, Range<>{}));
        // CHECK(true == EqualBy(EBFi, Range<>{}, Range<>{}, Range<>{})); // this will take a LONG time to run
        CHECK(true == EqualBy(EBFi, Range<10>{}, Range<10>{}, Range<10>{}));
        CHECK(true == EqualBy(EBFi, Range<-5, 10>{}, Range<-5, 10>{}, Range<-5, 10>{}));
        CHECK(true == EqualBy(EBFi, Range<-5, 10, 3>{}, Range<-5, 10, 3>{}, Range<-5, 10, 3>{}));
        CHECK(true == EqualBy(EBFi, Range<5, -10, -2>{}, Range<5, -10, -2>{}, Range<5, -10, -2>{}));
        CHECK(true == EqualBy(EBFi, Range<5, 5, 0>{}, Range<5, 5, 0>{}, Range<5, 5, 0>{}));
        CHECK(false == EqualBy(EBFi, Range<5, 5, 0>{}, Range<5, 15, 0>{}, Range<5, 5, 0>{}));
    }

    {
        auto c{Range<10>{}};
        CHECK(true == EqualBy(EBFi, c));
        CHECK(true == EqualBy(EBFi, c, c, c));
        CHECK(true == EqualBy(EBFi, c, Range<10>{}, Range<10>{}));
        CHECK(false == EqualBy(EBFi, c, Range<10>{}, Range<100>{}));
    }

    {
        using T = std::variant<int, double, char, const char*>;
        const auto EBFv = EBF<T>();

        CHECK(true == EqualBy(EBFi, Repeat(1)));
        // CHECK(true == EqualBy(EBFi, Repeat(1), Repeat(1))); // this will take a LONG time to run
        CHECK(true == EqualBy(EBFi, Repeat<10, int>{1}, Repeat<10, int>{1}, Repeat<10, int>{1}));
        CHECK(false == EqualBy(EBFi, Repeat<10, int>{1}, Repeat<10, int>{2}, Repeat<10, int>{1}));
        CHECK(false == EqualBy(EBFi, Repeat<10, int>{1}, Repeat(1), Repeat<10, int>{1}));
        CHECK(true == EqualBy(EBFv, Repeat(T{'x'})));
        CHECK(true == EqualBy(EBFv, Repeat<10, T>{T{'x'}}, Repeat<10, T>{T{'x'}}, Repeat<10, T>{T{'x'}}));
        CHECK(false == EqualBy(EBFv, Repeat<10, T>{T{'x'}}, Repeat<10, T>{T{'y'}}, Repeat<10, T>{T{'x'}}));
        CHECK(false == EqualBy(EBFv, Repeat<10, T>{T{'x'}}, Repeat(T{'x'}), Repeat<10, T>{T{'x'}}));
    }

    {
        auto c{Repeat<10, int>{1}};
        CHECK(true == EqualBy(EBFi, c));
        CHECK(true == EqualBy(EBFi, c, c, c));
        CHECK(true == EqualBy(EBFi, c, Repeat<10, int>{1}, Repeat<10, int>{1}));
        CHECK(false == EqualBy(EBFi, c, Repeat<10, int>{1}, Repeat<100, int>{'x'}));
    }

    {
        CHECK(true == EqualBy(EBFi, Range<5>{}, Array{0, 1, 2, 3, 4}));
        CHECK(false == EqualBy(EBFi, Range<10>{}, Array{0, 1, 2, 3, 4}));
        CHECK(true == EqualBy(EBFi, Array{0, 1, 2, 3, 4}, Range<0, 5, 1>{}, Array{0, 1, 2, 3, 4}));
        CHECK(true == EqualBy(EBFi, Array{5, 3, 1}, Range<5, 0, -2>{}));
        CHECK(true == EqualBy(EBFi, Range<1>{}, Array{0}, Repeat<1, int>{0}));
        CHECK(true == EqualBy(EBFi, Array{4, 4, 4, 4}, Repeat<4, int>{4}));
        CHECK(false == EqualBy(EBFi, Range<2>{}, Array{0}, Repeat<1, int>{0}));
        CHECK(false == EqualBy(EBFi, Range<1>{}, Array{0, 1}, Repeat<1, int>{0}));
        CHECK(false == EqualBy(EBFi, Range<1>{}, Array{0}, Repeat<2, int>{0}));
        CHECK(false == EqualBy(EBFi, Array{4, 4, 4}, Repeat<4, int>{4}));
        CHECK(false == EqualBy(EBFi, Array{4, 4, 4, 4}, Repeat<4, int>{5}));
    }
}
