#include <cstdlib>
#include "catch.hpp"
#include "no-heap.hpp"
#include "cljonic_catch.hpp"
#include "cljonic-array.hpp"
#include "cljonic-iterator.hpp"
#include "cljonic-range.hpp"
#include "cljonic-repeat.hpp"
#include "cljonic-set.hpp"
#include "cljonic-shared.hpp"
#include "cljonic-string.hpp"

using namespace cljonic;

SCENARIO("Shared", "[CljonicShared]")
{
    EnableNoHeapMessagePrinting();

    // test AreEqualValues
    {
        CharType hello[10];
        hello[0] = 'H';
        hello[1] = 'e';
        hello[2] = 'l';
        hello[3] = 'l';
        hello[4] = 'o';
        hello[5] = '\0';

        CharType hellox[10];
        hellox[0] = 'H';
        hellox[1] = 'e';
        hellox[2] = 'l';
        hellox[3] = 'l';
        hellox[4] = 'o';
        hellox[4] = 'x';
        hellox[5] = '\0';

        // cljonic collection and not cljonic collection
        CHECK_CLJONIC_NOT(AreEqualValues(1, String{"Hello"}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"Hello"}, 1));

        // C-string and C-string
        CHECK_CLJONIC(AreEqualValues("Hello", "Hello"));
        CHECK_CLJONIC_NOT(AreEqualValues("Hello", "Hellox"));
        CHECK_CLJONIC(AreEqualValues(hello, "Hello"));
        CHECK_CLJONIC_NOT(AreEqualValues("Hello", hellox));
        CHECK_CLJONIC(AreEqualValues(hello, hello));
        CHECK_CLJONIC_NOT(AreEqualValues(hello, hellox));

        // C-string and cljonic-string
        CHECK_CLJONIC(AreEqualValues("Hello", String{"Hello"}));
        CHECK_CLJONIC_NOT(AreEqualValues("Hello", String{"Hellox"}));
        CHECK_CLJONIC(AreEqualValues(hello, String{"Hello"}));
        CHECK_CLJONIC_NOT(AreEqualValues(hello, String{"Hellox"}));
        CHECK_CLJONIC(AreEqualValues(hello, String{hello}));
        CHECK_CLJONIC_NOT(AreEqualValues(hello, String{hellox}));

        // cljonic-string and C-string
        CHECK_CLJONIC(AreEqualValues(String{"Hello"}, "Hello"));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"Hellox"}, "Hello"));
        CHECK_CLJONIC(AreEqualValues(String{"Hello"}, hello));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"Hellox"}, hello));
        CHECK_CLJONIC(AreEqualValues(String{hello}, hello));
        CHECK_CLJONIC_NOT(AreEqualValues(String{hello}, hellox));

        // cljonic-string and cljonic-string
        CHECK_CLJONIC(AreEqualValues(String{"Hello"}, String{"Hello"}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"Hellox"}, String{"Hello"}));
        CHECK_CLJONIC(AreEqualValues(String{hello}, String{hello}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{hello}, String{hellox}));

        // Array and Array, primitive types
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 2, 3}, Array{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 2, 3}, Array{1.0, 2.0, 3.0}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1.0, 2.0, 3.0}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1.0, 2.0, 3.0}, Array{1.0, 2.0, 3.0}));
        CHECK_CLJONIC(AreEqualValues(Array{1, 2, 3}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 2, 3}, Array{1, 2, 3, 4}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 2, 3}, Array{1, 2, 4}));
        CHECK_CLJONIC(AreEqualValues(Array{"1", "2", "3"}, Array{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{"1", "2", "3"}, Array{"1", "2", "3", "4"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{"1", "2", "3"}, Array{"1", "2", "4"}));
        CHECK_CLJONIC(AreEqualValues(Array{'1', '2', '3'}, Array{'1', '2', '3'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{'1', '2', '3'}, Array{'1', '2', '3', '4'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{'1', '2', '3'}, Array{'1', '2', '4'}));
        CHECK_CLJONIC(AreEqualValues(Array{'1', '2', '3'}, Array{49, 50, 51}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{'1', '2', '3'}, Array{49, 50, 51, 52}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{'1', '2', '3'}, Array{49, 50, 52}));

        // Array and Iterator, primitive types
        CHECK_CLJONIC(AreEqualValues(Array{1, 2, 3}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 2, 30}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 2, 3}, Iterator{[](const double d) { return d + 1.0; }, 1.0}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1.1, 2.1, 3.1}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC(AreEqualValues(
            Array{'1', '2', '3'},
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(
            Array{'1', '2', 'x'},
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));

        // Array and Range
        CHECK_CLJONIC(AreEqualValues(Array{1, 2, 3}, Range<1, 4>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 2, 3}, Range<1, 5>{}));

        // Array and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValues(Array{1, 1, 1}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValues(Array{1, 1, 1, 1}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 2, 3}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValues(Array{"1", "1", "1"}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValues(Array{"1", "1", "1", "1"}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{"1", "2", "3"}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValues(Array{'1', '1', '1'}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValues(Array{'1', '1', '1', '1'}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{'1', '2', '3'}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 1, 1}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{"1", "1", "1"}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 1, 1}, Repeat{1.1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1.1, 1.1, 1.1}, Repeat{1.1}));

        // Array and String, primitive types
        CHECK_CLJONIC(AreEqualValues(Array{'1', '1', '1'}, String{"111"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{'1', '1', '1', '1'}, String{"111"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{'1', '1', '1'}, String{"1111"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Array{1, 1, 1}, String{"111"}));

        // Iterator and Array, primitive types
        CHECK_CLJONIC(AreEqualValues(Iterator{[](const int i) { return i + 1; }, 1}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const int i) { return i + 1; }, 1}, Array{1, 2, 30}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const double d) { return d + 1.0; }, 1.0}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const int i) { return i + 1; }, 1}, Array{1.1, 2.1, 3.1}));
        CHECK_CLJONIC(AreEqualValues(
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            Array{'1', '2', '3'}));
        CHECK_CLJONIC_NOT(AreEqualValues(
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            Array{'1', '2', 'x'}));

        // Iterator and Iterator, primitive types
        CHECK_CLJONIC(AreEqualValues(Iterator{[](const int i) { return i + 1; }, 1},
                                     Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(
            Iterator{[](const int i) { return i + 1; }, 1},
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC(AreEqualValues(
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const double d) { return d + 1.1; }, 1.0},
                                         Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const int i) { return i + 1; }, 1},
                                         Iterator{[](const double d) { return d + 1.1; }, 1.0}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const double d) { return d + 1.1; }, 1.0},
                                         Iterator{[](const double d) { return d + 1.1; }, 1.0}));

        // Iterator and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValues(Iterator{[](const int i) { return i; }, 1}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValues(Iterator{[](const char c) { return c; }, '1'}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValues(Iterator{[](const char* cString) { return cString; }, "1"}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const int i) { return i + 1; }, 1}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValues(
            Iterator{[](const char c) { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const int i) { return i; }, 1}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const int i) { return i; }, 1}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const double d) { return d; }, 1.0}, Repeat{1.0}));

        // Iterator and String, primitive types
        CHECK_CLJONIC(AreEqualValues(
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            String{"123"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const char c) -> char { return c; }, '1'}, String{"123"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Iterator{[](const int i) { return i + 1; }, 1}, String{"123"}));

        // Range and Array, primitive types
        CHECK_CLJONIC(AreEqualValues(Range<1, 4>{}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 5>{}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 4>{}, Array{'1', '2', '3'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 4>{}, Array{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 4>{}, Array{1.0, 2.0, 3.0}));

        // Range and Iterator, primitive types
        CHECK_CLJONIC(AreEqualValues(Range{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(AreEqualValues(Range<1>{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(AreEqualValues(Range<10>{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(AreEqualValues(Range<1, 10>{}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC(AreEqualValues(Range<1, 100, 3>{}, Iterator{[](const int i) { return i + 3; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range{}, Iterator{[](const double d) { return d + 1.1; }, 0.0}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1>{}, Iterator{[](const double d) { return d + 1.1; }, 0.0}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<10>{}, Iterator{[](const double d) { return d + 1.1; }, 0.0}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 10>{}, Iterator{[](const double d) { return d + 1.1; }, 1.0}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 100, 3>{}, Iterator{[](const double d) { return d + 3.0; }, 1.0}));

        // Range and Range
        CHECK_CLJONIC(AreEqualValues(Range{}, Range{}));
        CHECK_CLJONIC(AreEqualValues(Range<5>{}, Range<5>{}));
        CHECK_CLJONIC(AreEqualValues(Range<1, 5>{}, Range<1, 5>{}));
        CHECK_CLJONIC(AreEqualValues(Range<1, 5, 2>{}, Range<1, 5, 2>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<5>{}, Range<3>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<3>{}, Range<5>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 5>{}, Range<5, 1>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<5, 1>{}, Range<1, 5>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 5, 2>{}, Range<1, 5, 3>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 5, 3>{}, Range<1, 5, 2>{}));

        // Range and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValues(Range<1, 2>{}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1>{}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 2>{}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 2>{}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<1, 2>{}, Repeat{1.1}));

        // Range and String
        CHECK_CLJONIC(AreEqualValues(Range<49, 52>{}, String{"123"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Range<49, 53>{}, String{"123"}));

        // Repeat and Array, primitive types
        CHECK_CLJONIC(AreEqualValues(Repeat{1}, Array{1, 1, 1}));
        CHECK_CLJONIC(AreEqualValues(Repeat{1}, Array{1, 1, 1, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1}, Array{1, 2, 3}));
        CHECK_CLJONIC(AreEqualValues(Repeat{"1"}, Array{"1", "1", "1"}));
        CHECK_CLJONIC(AreEqualValues(Repeat{"1"}, Array{"1", "1", "1", "1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{"1"}, Array{"1", "2", "3"}));
        CHECK_CLJONIC(AreEqualValues(Repeat{'1'}, Array{'1', '1', '1'}));
        CHECK_CLJONIC(AreEqualValues(Repeat{'1'}, Array{'1', '1', '1', '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{'1'}, Array{'1', '2', '3'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{"1"}, Array{1, 1, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1}, Array{"1", "1", "1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1.1}, Array{1, 1, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1.1}, Array{1.1, 1.1, 1.1}));

        // Repeat and Iterator, primitive types
        CHECK_CLJONIC(AreEqualValues(Repeat{1}, Iterator{[](const int i) { return i; }, 1}));
        CHECK_CLJONIC(AreEqualValues(Repeat{'1'}, Iterator{[](const char c) { return c; }, '1'}));
        CHECK_CLJONIC(AreEqualValues(Repeat{"1"}, Iterator{[](const char* cString) { return cString; }, "1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(
            Repeat{'1'}, Iterator{[](const char c) { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{'1'}, Iterator{[](const int i) { return i; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{"1"}, Iterator{[](const int i) { return i; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1.0}, Iterator{[](const double d) { return d; }, 1.0}));

        // Repeat and Range, primitive types
        CHECK_CLJONIC(AreEqualValues(Repeat{1}, Range<1, 2>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1}, Range<1>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{'1'}, Range<1, 2>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{"1"}, Range<1, 2>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1.1}, Range<1, 2>{}));

        // Repeat and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValues(Repeat{1}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValues(Repeat{'1'}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValues(Repeat{"1"}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{'1'}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{"1"}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1.0}, Repeat{1.0}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1.0}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1}, Repeat{1.0}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1}, Repeat<10, int>{1}));

        // Repeat and String, primitive types
        CHECK_CLJONIC(AreEqualValues(Repeat{"1"}, String{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1}, String{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{'1'}, String{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Repeat{1.0}, String{"1"}));

        // String and Array, primitive types
        CHECK_CLJONIC(AreEqualValues(String{"111"}, Array{'1', '1', '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"111"}, Array{'1', '1', '1', '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"1111"}, Array{'1', '1', '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"111"}, Array{1, 1, 1}));

        // String and Iterator, primitive types
        CHECK_CLJONIC(AreEqualValues(
            String{"123"},
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"123"}, Iterator{[](const char c) -> char { return c; }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"123"}, Iterator{[](const int i) { return i + 1; }, 1}));

        // String and Range
        CHECK_CLJONIC(AreEqualValues(String{"123"}, Range<49, 52>{}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"123"}, Range<49, 53>{}));

        // String and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValues(String{"1"}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"1"}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"1"}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValues(String{"1"}, Repeat{1.0}));

        // Set and Set, primitive types
        CHECK_CLJONIC(AreEqualValues(Set{1, 2, 3}, Set{1, 2, 3}));
        CHECK_CLJONIC(AreEqualValues(Set{'1', '2', '3'}, Set{'1', '2', '3'}));
        CHECK_CLJONIC(AreEqualValues(Set{"1", "2", "3"}, Set{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(AreEqualValues(Set{1, 2, 3}, Set{1, 2, 3, 4}));
        CHECK_CLJONIC_NOT(AreEqualValues(Set{'1', '2', '3'}, Set{'1', '2', '3', '4'}));
        CHECK_CLJONIC_NOT(AreEqualValues(Set{"1", "2", "3"}, Set{"1", "2", "3", "4"}));

        // just primitive types
        CHECK_CLJONIC(AreEqualValues(1, 1));
        CHECK_CLJONIC(AreEqualValues('1', '1'));
        CHECK_CLJONIC(AreEqualValues("1", "1"));
        CHECK_CLJONIC_NOT(AreEqualValues(1.1, 1));
        CHECK_CLJONIC_NOT(AreEqualValues(1, 1.1));
        CHECK_CLJONIC_NOT(AreEqualValues(1.1, 1.1));
        CHECK_CLJONIC_NOT(AreEqualValues(1, "1"));
    }

    // test AreEqualValuesBy
    {
        CharType hello[10];
        hello[0] = 'H';
        hello[1] = 'e';
        hello[2] = 'l';
        hello[3] = 'l';
        hello[4] = 'o';
        hello[5] = '\0';

        CharType hellox[10];
        hellox[0] = 'H';
        hellox[1] = 'e';
        hellox[2] = 'l';
        hellox[3] = 'l';
        hellox[4] = 'o';
        hellox[4] = 'x';
        hellox[5] = '\0';

        constexpr auto AreEqual = [](const auto& i, const auto& j) { return i == j; };

        constexpr auto EqualCStrings = [](const char* s1, const char* s2) { return 0 == std::strcmp(s1, s2); };

        constexpr auto EqualCStringAndCljonicString = [](const char* s1, const auto& s2)
        { return 0 == std::strcmp(s1, s2.c_str()); };

        constexpr auto EqualCljonicStringAndCString = [](const auto& s1, const char* s2)
        { return 0 == std::strcmp(s2, s1.c_str()); };

        constexpr auto EqualCljonicCharAndCljonicChar = [](const CharType c1, const CharType c2) { return c2 == c1; };

        constexpr auto IntEqualsCString = [](const int i, const char* s) { return i == std::atol(s); };

        constexpr auto IntEqualsCljonicChar = [](const int i, const CharType c) { return c == (i + 48); };

        constexpr auto CStringEqualsInt = [](const char* s, const int i) { return i == std::atol(s); };

        constexpr auto CljonicCharEqualsInt = [](const CharType c, const int i) { return c == (i + 48); };

        constexpr auto CljonicCharEqualsCString = [](const CharType c, const char* s)
        { return ((1 == std::strlen(s)) and (c == s[0])); };

        constexpr auto CljonicCharEqualsDouble = [](const CharType c, const double d) { return c == (d + 48); };

        // C-string and C-string
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, "Hello", "Hello"));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStrings, "Hello", "Hellox"));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, hello, "Hello"));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStrings, "Hello", hellox));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, hello, hello));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStrings, hello, hellox));

        // C-string and cljonic-string
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStringAndCljonicString, "Hello", String{"Hello"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStringAndCljonicString, "Hello", String{"Hellox"}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStringAndCljonicString, hello, String{"Hello"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStringAndCljonicString, hello, String{"Hellox"}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStringAndCljonicString, hello, String{hello}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStringAndCljonicString, hello, String{hellox}));

        // cljonic-string and C-string
        CHECK_CLJONIC(AreEqualValuesBy(EqualCljonicStringAndCString, String{"Hello"}, "Hello"));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCljonicStringAndCString, String{"Hellox"}, "Hello"));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCljonicStringAndCString, String{"Hello"}, hello));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCljonicStringAndCString, String{"Hellox"}, hello));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCljonicStringAndCString, String{hello}, hello));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCljonicStringAndCString, String{hello}, hellox));

        // cljonic-string and cljonic-string
        CHECK_CLJONIC(AreEqualValuesBy(EqualCljonicCharAndCljonicChar, String{"Hello"}, String{"Hello"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCljonicCharAndCljonicChar, String{"Hellox"}, String{"Hello"}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCljonicCharAndCljonicChar, String{hello}, String{hello}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCljonicCharAndCljonicChar, String{hello}, String{hellox}));

        // Array and Array, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1, 2, 3}, Array{1, 2, 3}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCString, Array{1, 2, 3}, Array{"1", "2", "3"}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1, 2, 3}, Array{1.0, 2.0, 3.0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1.0, 2.0, 3.0}, Array{1, 2, 3}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1.0, 2.0, 3.0}, Array{1.0, 2.0, 3.0}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{1, 2, 3}, Array{1, 2, 3, 4}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{1, 2, 3}, Array{1, 2, 4}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{"1", "2", "3"}, Array{"1", "2", "3"}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, Array{"1", "2", "3"}, Array{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{"1", "2", "3"}, Array{"1", "2", "3", "4"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStrings, Array{"1", "2", "3"}, Array{"1", "2", "3", "4"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{"1", "2", "3"}, Array{"1", "2", "4"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStrings, Array{"1", "2", "3"}, Array{"1", "2", "4"}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{'1', '2', '3'}, Array{'1', '2', '3'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{'1', '2', '3'}, Array{'1', '2', '3', '4'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{'1', '2', '3'}, Array{'1', '2', '4'}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{'1', '2', '3'}, Array{49, 50, 51}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{'1', '2', '3'}, Array{49, 50, 51, 52}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{'1', '2', '3'}, Array{49, 50, 52}));

        // Array and Iterator, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1, 2, 3}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{1, 2, 30}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC(
            AreEqualValuesBy(AreEqual, Array{1, 2, 3}, Iterator{[](const double d) { return d + 1.0; }, 1.0}));
        CHECK_CLJONIC_NOT(
            AreEqualValuesBy(AreEqual, Array{1.1, 2.1, 3.1}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(
            AreEqual,
            Array{'1', '2', '3'},
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(
            AreEqual,
            Array{'1', '2', 'x'},
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));

        // Array and Range
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1, 2, 3}, Range<1, 4>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{1, 2, 3}, Range<1, 5>{}));
        CHECK_CLJONIC(AreEqualValuesBy(CStringEqualsInt, Array{"1", "2", "3"}, Range<1, 4>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(CStringEqualsInt, Array{"1", "2", "3"}, Range<1, 5>{}));

        // Array and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1, 1, 1}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1, 1, 1, 1}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{1, 2, 3}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, Array{"1", "1", "1"}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, Array{"1", "1", "1", "1"}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStrings, Array{"1", "2", "3"}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{'1', '1', '1'}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{'1', '1', '1', '1'}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{'1', '2', '3'}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCString, Array{1, 1, 1}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(CStringEqualsInt, Array{"1", "1", "1"}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{1, 1, 1}, Repeat{1.1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1, 1, 1}, Repeat{1.0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{1.1, 1.1, 1.1}, Repeat{1.1}));

        // Array and String, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Array{'1', '1', '1'}, String{"111"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{'1', '1', '1', '1'}, String{"111"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{'1', '1', '1'}, String{"1111"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Array{1, 1, 1}, String{"111"}));

        // Iterator and Array, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Iterator{[](const int i) { return i + 1; }, 1}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Iterator{[](const int i) { return i + 1; }, 1}, Array{1, 2, 30}));
        CHECK_CLJONIC(
            AreEqualValuesBy(AreEqual, Iterator{[](const double d) { return d + 1.0; }, 1.0}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(
            AreEqualValuesBy(AreEqual, Iterator{[](const int i) { return i + 1; }, 1}, Array{1.1, 2.1, 3.1}));
        CHECK_CLJONIC(AreEqualValuesBy(
            AreEqual,
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            Array{'1', '2', '3'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(
            AreEqual,
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            Array{'1', '2', 'x'}));
        CHECK_CLJONIC(
            AreEqualValuesBy(IntEqualsCString, Iterator{[](const int i) { return i + 1; }, 1}, Array{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(
            AreEqualValuesBy(IntEqualsCString, Iterator{[](const int i) { return i + 1; }, 1}, Array{"1", "2", "30"}));

        // Iterator and Iterator, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(
            AreEqual, Iterator{[](const int i) { return i + 1; }, 1}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(
            AreEqual,
            Iterator{[](const int i) { return i + 1; }, 1},
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(
            AreEqual,
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(
            AreEqual,
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual,
                                       Iterator{[](const double d) { return d + 1.0; }, 1.0},
                                       Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual,
                                           Iterator{[](const double d) { return d + 1.1; }, 1.0},
                                           Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual,
                                       Iterator{[](const int i) { return i + 1; }, 1},
                                       Iterator{[](const double d) { return d + 1.0; }, 1.0}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual,
                                           Iterator{[](const int i) { return i + 1; }, 1},
                                           Iterator{[](const double d) { return d + 1.1; }, 1.0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual,
                                       Iterator{[](const double d) { return d + 1.1; }, 1.0},
                                       Iterator{[](const double d) { return d + 1.1; }, 1.0}));

        // Iterator and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Iterator{[](const int i) { return i; }, 1}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Iterator{[](const char c) { return c; }, '1'}, Repeat{'1'}));
        CHECK_CLJONIC(
            AreEqualValuesBy(EqualCStrings, Iterator{[](const char* cString) { return cString; }, "1"}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Iterator{[](const int i) { return i + 1; }, 1}, Repeat{1}));
        CHECK_CLJONIC(
            not AreEqualValuesBy(AreEqual,
                                 Iterator{[](const char c) { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
                                 Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Iterator{[](const int i) { return i; }, 1}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCString, Iterator{[](const int i) { return i; }, 1}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Iterator{[](const double d) { return d; }, 1.0}, Repeat{1.0}));

        // Iterator and String, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(
            AreEqual,
            Iterator{[](const char c) -> char { return static_cast<char>(static_cast<int>(c) + 1); }, '1'},
            String{"123"}));
        CHECK_CLJONIC_NOT(
            AreEqualValuesBy(AreEqual, Iterator{[](const char c) -> char { return c; }, '1'}, String{"123"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Iterator{[](const int i) { return i + 1; }, 1}, String{"123"}));

        // Range and Array, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1, 4>{}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<1, 5>{}, Array{1, 2, 3}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<1, 4>{}, Array{'1', '2', '3'}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCString, Range<1, 4>{}, Array{"1", "2", "3"}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1, 4>{}, Array{1.0, 2.0, 3.0}));

        // Range and Iterator, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1>{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<10>{}, Iterator{[](const int i) { return i + 1; }, 0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1, 10>{}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1, 100, 3>{}, Iterator{[](const int i) { return i + 3; }, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range{}, Iterator{[](const double d) { return d + 1.0; }, 0.0}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range{}, Iterator{[](const double d) { return d + 1.1; }, 0.0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1>{}, Iterator{[](const double d) { return d + 1.1; }, 0.0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<10>{}, Iterator{[](const double d) { return d + 1.0; }, 0.0}));
        CHECK_CLJONIC_NOT(
            AreEqualValuesBy(AreEqual, Range<10>{}, Iterator{[](const double d) { return d + 1.1; }, 0.0}));
        CHECK_CLJONIC(
            AreEqualValuesBy(AreEqual, Range<1, 10>{}, Iterator{[](const double d) { return d + 1.0; }, 1.0}));
        CHECK_CLJONIC_NOT(
            AreEqualValuesBy(AreEqual, Range<1, 10>{}, Iterator{[](const double d) { return d + 1.1; }, 1.0}));
        CHECK_CLJONIC(
            AreEqualValuesBy(AreEqual, Range<1, 100, 3>{}, Iterator{[](const double d) { return d + 3.0; }, 1.0}));

        // Range and Range
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range{}, Range{}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<5>{}, Range<5>{}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1, 5>{}, Range<1, 5>{}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1, 5, 2>{}, Range<1, 5, 2>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<5>{}, Range<3>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<3>{}, Range<5>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<1, 5>{}, Range<5, 1>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<5, 1>{}, Range<1, 5>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<1, 5, 2>{}, Range<1, 5, 3>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<1, 5, 3>{}, Range<1, 5, 2>{}));

        // Range and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1, 2>{}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<1>{}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<1, 2>{}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCString, Range<1, 2>{}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<1, 2>{}, Repeat{1.0}));

        // Range and String
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Range<49, 52>{}, String{"123"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Range<49, 53>{}, String{"123"}));

        // Repeat and Array, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1}, Array{1, 1, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1}, Array{1, 1, 1, 1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{1}, Array{1, 2, 3}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, Repeat{"1"}, Array{"1", "1", "1"}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, Repeat{"1"}, Array{"1", "1", "1", "1"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStrings, Repeat{"1"}, Array{"1", "2", "3"}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{'1'}, Array{'1', '1', '1'}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{'1'}, Array{'1', '1', '1', '1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{'1'}, Array{'1', '2', '3'}));
        CHECK_CLJONIC(AreEqualValuesBy(CStringEqualsInt, Repeat{"1"}, Array{1, 1, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCString, Repeat{1}, Array{"1", "1", "1"}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1.0}, Array{1, 1, 1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{1.1}, Array{1, 1, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1.1}, Array{1.1, 1.1, 1.1}));

        // Repeat and Iterator, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1}, Iterator{[](const int i) { return i; }, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{'1'}, Iterator{[](const char c) { return c; }, '1'}));
        CHECK_CLJONIC(
            AreEqualValuesBy(EqualCStrings, Repeat{"1"}, Iterator{[](const char* cString) { return cString; }, "1"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{1}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC_NOT(
            AreEqualValuesBy(AreEqual,
                             Repeat{'1'},
                             Iterator{[](const char c) { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{'1'}, Iterator{[](const int i) { return i; }, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(CStringEqualsInt, Repeat{"1"}, Iterator{[](const int i) { return i; }, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1.0}, Iterator{[](const double d) { return d; }, 1.0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1.0}, Iterator{[](const int i) { return i; }, 1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1}, Iterator{[](const double d) { return d; }, 1.0}));

        // Repeat and Range, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1}, Range<1, 2>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{1}, Range<1>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{'1'}, Range<1, 2>{}));
        CHECK_CLJONIC(AreEqualValuesBy(CStringEqualsInt, Repeat{"1"}, Range<1, 2>{}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1.0}, Range<1, 2>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{1.1}, Range<1, 2>{}));

        // Repeat and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{'1'}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, Repeat{"1"}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{1}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{'1'}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCString, Repeat{1}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(CStringEqualsInt, Repeat{"1"}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1.0}, Repeat{1.0}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1.0}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{1}, Repeat{1.0}));

        // Repeat and String, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{'1'}, String{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Repeat{'1'}, String{"111"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Repeat{'1'}, String{"12"}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCljonicChar, Repeat{1}, String{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCljonicChar, Repeat{1}, String{"11111"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(IntEqualsCljonicChar, Repeat{1}, String{"11131"}));

        // String and Array, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, String{"111"}, Array{'1', '1', '1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, String{"111"}, Array{'1', '1', '1', '1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, String{"1111"}, Array{'1', '1', '1'}));
        CHECK_CLJONIC(AreEqualValuesBy(CljonicCharEqualsInt, String{"111"}, Array{1, 1, 1}));

        // String and Iterator, primitive types
        CHECK_CLJONIC(
            AreEqualValuesBy(AreEqual,
                             String{"123"},
                             Iterator{[](const char c) { return static_cast<char>(static_cast<int>(c) + 1); }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, String{"123"}, Iterator{[](const char c) { return c; }, '1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, String{"123"}, Iterator{[](const int i) { return i + 1; }, 1}));
        CHECK_CLJONIC(
            AreEqualValuesBy(CljonicCharEqualsInt, String{"123"}, Iterator{[](const int i) { return i + 1; }, 1}));

        // String and Range
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, String{"123"}, Range<49, 52>{}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, String{"123"}, Range<49, 53>{}));

        // String and Repeat, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(CljonicCharEqualsCString, String{"1"}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(CljonicCharEqualsCString, String{"1111"}, Repeat{"1"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(CljonicCharEqualsCString, String{"1121"}, Repeat{"1"}));
        CHECK_CLJONIC(AreEqualValuesBy(CljonicCharEqualsInt, String{"1"}, Repeat{1}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(CljonicCharEqualsInt, String{"12"}, Repeat{1}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, String{"1"}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, String{"111"}, Repeat{'1'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, String{"1121"}, Repeat{'1'}));
        CHECK_CLJONIC(AreEqualValuesBy(CljonicCharEqualsDouble, String{"1"}, Repeat{1.0}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(CljonicCharEqualsDouble, String{"1121"}, Repeat{1.0}));

        // Set and Set, primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Set{1, 2, 3}, Set{1, 2, 3}));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, Set{'1', '2', '3'}, Set{'1', '2', '3'}));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, Set{"1", "2", "3"}, Set{"1", "2", "3"}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Set{1, 2, 3}, Set{1, 2, 3, 4}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, Set{'1', '2', '3'}, Set{'1', '2', '3', '4'}));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(EqualCStrings, Set{"1", "2", "3"}, Set{"1", "2", "3", "4"}));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCString, Set{1, 2, 3}, Set{"1", "2", "3"}));

        // just primitive types
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, 1, 1));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, '1', '1'));
        CHECK_CLJONIC(AreEqualValuesBy(EqualCStrings, "1", "1"));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, 1.0, 1));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, 1, 1.0));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, 1.1, 1));
        CHECK_CLJONIC_NOT(AreEqualValuesBy(AreEqual, 1, 1.1));
        CHECK_CLJONIC(AreEqualValuesBy(AreEqual, 1.1, 1.1));
        CHECK_CLJONIC(AreEqualValuesBy(IntEqualsCString, 1, "1"));
        CHECK_CLJONIC(AreEqualValuesBy(CStringEqualsInt, "1", 1));
        CHECK_CLJONIC_NOT(
            AreEqualValuesBy([]([[maybe_unused]] const int i, [[maybe_unused]] const char* s) { return true; }, 1, 1));
    }
    DisableNoHeapMessagePrinting();
}
