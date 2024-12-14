#ifndef CLJONIC_CORE_HPP
#define CLJONIC_CORE_HPP

/** \mainpage
 * \b cljonic is a \b header-only library of C++ templates and classes designed to allow an \b embedded \b systems C++
 * developer to program in a \b Functional \b Programming \b Style using a set of functions \b similar in \b name and \b
 * function to \b Clojure. To address the embedded systems focus, \b cljonic \b DOES \b NOT use the heap or exceptions,
 * and uses \b Modern \b C++ features to \b protect , \b guide , and \b teach Modern C++ . Finally, \b cljonic is a
 * library that includes unit tests with nearly <b>100% coverage</b>, and is designed to be efficiently maintained.
 *
 * The motivation for \b cljonic comes from observations made about the pragmatic use of C++ for embedded systems
 * software development, the progression of the C++ language (e.g., C++11, C++14, C++17, C++20, C++23), and the counsel
 * of software development "experts", in many different programming languages, that <b>a functional programming style
 * should be eagerly embraced by all developers</b>.
 *
 * Embedded systems C++ developers, on the whole, are <b>not academic programming language experts</b> with in-depth
 * knowledge of every C++ feature, and they <b>do not have time</b> to follow the progression of the language.  Rather,
 * the average embedded systems C++ developer has a <b>strong hardware background</B>, and <B>knows a small domain
 * specific pragmatic subset of C++</B> that is sufficient to deliver reasonably good software in a predictable amount
 * of time, which is usually <b>not optimized for minimal long term cost of ownership</b>. Generally speaking, the C++
 * subset used is a small subset of the \b C programming language with a bit of C++'s <b>Object Oriented</b>
 * capabilities.  This begs a question:
 *
 * <b>Is there a pragmatic subset of Modern C++ that would allow embedded systems developers to produce even higher
 * quality code even more efficiently and predictably that is cheaper to maintain over time?</b>
 *
 * Changes to C++ have provided valuable capabilities that are time consuming to master, hence it is both valuable to
 * development organizations for their developers to use new C++ capabilities, and too expensive to train all of their
 * developers to be experts in their use.  This, too, begs another question:
 *
 * <b>Is there a cost-effective way to utilize a small subset of an organization's embedded systems software developers
 * (i.e., One or two) to make new C++ capabilities available to the entire team?</b>
 *
 * In this post:
 *
 *     https://www.gamedeveloper.com/programming/in-depth-functional-programming-in-c-
 *
 * the author makes the following statements:
 *
 * <b>"A large fraction of the flaws in software development are due to programmers not fully understanding all the
 *  possible states their code may execute in. In a multithreaded environment, the lack of understanding and the
 *  resulting problems are greatly amplified, almost to the point of panic if you are paying attention. Programming in a
 *  functional style makes the state presented to your code explicit, which makes it much easier to reason about
 *  [(i.e., Fewer mistakes, and less debugging)], and, in a completely pure system, makes thread race conditions
 *  impossible."
 *
 * "No matter what language you work in, programming in a functional style provides benefits. You should do it
 *  whenever it is convenient, and you should think hard about the decision when it isn't convenient."</b>
 *
 * \b cljonic is a \b header-only library that utilizes \b Modern \b C++ capabilities to provide a functional style C++
 * \b Clojure subset to \b embedded \b systems developers, promotes the \b predictable \b development of \b higher \b
 * quality code \b faster, and can be \b cost-effectively \b maintained over time.
 *
 * \anchor Core_Cheatsheet
 * ___
 * # Cheatsheet
 *
 * ## Aliases
 *
 * Namespace                      | "using" C++ Statement
 * ------------------------------ | ---------------------------------------------
 * \ref Namespace_Core "core"     | using **core** = cljonic::core;
 * \ref Namespace_Regex "regex"   | using **regex** = cljonic::regex;
 * \ref Namespace_Set "set"       | using **set** = cljonic::set;
 * \ref Namespace_String "string" | using **string** = cljonic::string;
 *
 * ## Collection Types
 *
 * - \ref Type_Array  "cljonic::Array"
 * - \ref Type_Range  "cljonic::Range"
 * - \ref Type_Repeat "cljonic::Repeat"
 * - \ref Type_Set    "cljonic::Set"
 * - \ref Type_String "cljonic::String"
 *
 * ## Core Functions
 *
 * ### Composition
 *
 * - \ref Core_Partial "Partial"
 *
 * ### Control
 *
 * - \ref Core_DoSeq "DoSeq", \ref Core_DoTimes "DoTimes", \ref Core_If "If", \ref Core_Unless "Unless", \ref Core_When
 * "When", \ref Core_WhenNot "WhenNot"
 *
 * ### Predicate
 *
 * - \ref Core_Compare "Compare"
 * - \ref Core_EQ "EQ"
 * - \ref Core_GE "GE", \ref Core_GT "GT"
 * - \ref Core_IsAlpha "IsAlpha", \ref Core_IsAlphanumeric "IsAlphanumeric", \ref Core_IsBlank "IsBlank",
 * \ref Core_IsControl "IsControl", \ref Core_IsDigit "IsDigit", \ref Core_IsEven "IsEven",
 * \ref Core_IsGraphical "IsGraphical", \ref Core_IsHexDigit "IsHexDigit", \ref Core_IsLowercase "IsLowercase",
 * \ref Core_IsNatural "IsNatural", \ref Core_IsNegative "IsNegative", \ref Core_IsOdd "IsOdd",
 * \ref Core_IsPositive "IsPositive", \ref Core_IsPrintable "IsPrintable", \ref Core_IsPunctuation "IsPunctuation",
 * \ref Core_IsSpace "IsSpace", \ref Core_IsUppercase "IsUppercase", \ref Core_IsZero "IsZero"
 * - \ref Core_LE "LE", \ref Core_LT "LT"
 * - \ref Core_NE "NE"
 *
 * ### Primitive
 *
 * - \ref Core_Abs "Abs", \ref Core_Add "Add"
 * - \ref Core_Char "Char", \ref Core_Combinations "Combinations", \ref Core_CString "CString"
 * - \ref Core_Dec "Dec", \ref Core_Div "Div"
 * - \ref Core_Factorial "Factorial"
 * - \ref Core_Inc "Inc"
 * - \ref Core_Lowercase "Lowercase"
 * - \ref Core_Max "Max", \ref Core_Min "Min", \ref Core_Mod "Mod", \ref Core_Mul "Mul"
 * - \ref Core_Ord "Ord"
 * - \ref Core_Permutations "Permutations"
 * - \ref Core_Sub "Sub",
 * - \ref Core_Uppercase "Uppercase"
 *
 * ### Array, Range, Repeat, Set, And String
 *
 * - \ref Core_Clone "Clone", \ref Core_Compose "Compose", \ref Core_Concat "Concat", \ref Core_Conj "Conj", \ref
 * Core_Conj_M "Conj_M", \ref Core_Count "Count", \ref Core_Count_M "Count_M", \ref Core_Cycle "Cycle"
 * - \ref Core_Dedupe "Dedupe", \ref Core_DedupeBy "DedupeBy", \ref Core_DefaultElement "DefaultElement",
 * \ref Core_DefaultElement_M "DefaultElement_M", \ref Core_Different "Different", \ref Core_Distinct "Distinct",
 * \ref Core_DistinctBy "DistinctBy", \ref Core_Drop "Drop", \ref Core_DropLast "DropLast",
 * \ref Core_DropWhile "DropWhile"
 * \ref Core_DropWhile "DropWhile"
 * - \ref Core_Empty_M "Empty_M", \ref Core_Equal "Equal", \ref Core_EqualBy "EqualBy", \ref Core_Every "Every"
 * - \ref Core_Filter "Filter", \ref Core_First "First", \ref Core_Flatten "Flatten",
 * \ref Core_FlattenSize "FlattenSize", \ref Core_Frequencies "Frequencies", \ref Core_FrequenciesBy "FrequenciesBy"
 * - \ref Core_Identity "Identity", \ref Core_IndexOf "IndexOf", \ref Core_IndexOfBy "IndexOfBy",
 * \ref Core_Interleave "Interleave", \ref Core_Interpose "Interpose", \ref Core_IsDistinct "IsDistinct",
 * \ref Core_IsDistinctBy "IsDistinctBy", \ref Core_IsFull "IsFull", \ref Core_Iterate "Iterate"
 * - \ref Core_Juxt "Juxt"
 * - \ref Core_Last "Last", \ref Core_LastIndexOf "LastIndexOf", \ref Core_LastIndexOfBy "LastIndexOfBy"
 * - \ref Core_Map "Map", \ref Core_Max "Max", \ref Core_MaxBy "MaxBy", \ref Core_Min "Min", \ref Core_MinBy "MinBy"
 * - \ref Core_NotAny "NotAny", \ref Core_NotEvery "NotEvery", \ref Core_Nth "Nth", \ref Core_Nth_M "Nth_M"
 * - \ref Core_Partition "Partition", \ref Core_PartitionAll "PartitionAll", \ref Core_PartitionBy "PartitionBy"
 * - \ref Core_Reduce "Reduce", \ref Core_Reductions "Reductions", \ref Core_Remove "Remove",
 * \ref Core_Replace "Replace", \ref Core_ReplaceBy "ReplaceBy", \ref Core_Reverse "Reverse"
 * - \ref Core_Same "Same", \ref Core_Second "Second", \ref Core_Some "Some", \ref Core_Sort "Sort",
 * \ref Core_SortBy "SortBy", \ref Core_SplitBy "SplitBy", \ref Core_SplitAt "SplitAt", \ref Core_Size "Size",
 * \ref Core_Subs "Subs"
 * - \ref Core_Take "Take", \ref Core_TakeLast "TakeLast", \ref Core_TakeNth "TakeNth", \ref Core_TakeWhile "TakeWhile"
 *
 * ## Regex Functions
 *
 * - \ref Regex_ReFind "ReFind", \ref Regex_ReSeq "ReSeq"
 *
 * ## Set Functions
 *
 * - \ref Set_Conj "Conj", \ref Set_ConjBy "ConjBy", \ref Set_Contains "Contains", \ref Set_ContainsBy "ContainsBy"
 * - \ref Set_Difference "Difference", \ref Set_DifferenceBy "DifferenceBy", \ref Set_Disj "Disj",
 * \ref Set_DisjBy "DisjBy"
 * - \ref Set_IndexOf "IndexOf", \ref Set_IndexOfBy "IndexOfBy", \ref Set_Intersection "Intersection",
 * \ref Set_IntersectionBy "IntersectionBy"
 * - \ref Set_ToSet "ToSet", \ref Set_ToSetBy "ToSetBy"
 * - \ref Set_Union "Union", \ref Set_UnionBy "UnionBy"
 *
 * ## String Functions
 *
 * - \ref String_Capitalize "Capitalize", \ref String_Compare "Compare", \ref String_Concat "Concat"
 * - \ref String_EndsWith "EndsWith"
 * - \ref String_FloatToString "FloatToString", \ref String_Format "Format"
 * - \ref String_Includes "Includes", \ref String_IndexOf "IndexOf", \ref String_Interpose "Interpose",
 * \ref String_IntToString "IntToString", \ref String_IntToStringHex "IntToStringHex", \ref String_IsBlank "IsBlank"
 * - \ref String_LastIndexOf "LastIndexOf", \ref String_Lowercase "Lowercase"
 * - \ref String_Partition "Partition", \ref String_PartitionAll "PartitionAll", \ref String_PartitionBy "PartitionBy"
 * - \ref String_Replace "Replace", \ref String_ReplaceFirst "ReplaceFirst"
 * - \ref String_Split "Split", \ref String_SplitCount "SplitCount", \ref String_SplitLine "SplitLine",
 * \ref String_StartsWith "StartsWith", \ref String_StrLen "StrLen", \ref String_StrOfChar "StrOfChar",
 * \ref String_StrToFloat "StrToFloat", \ref String_StrToIntDecimal "StrToIntDecimal",
 * \ref String_StrToIntHex "StrToIntHex", \ref String_Subs "Subs"
 * - \ref String_Trim "Trim", \ref String_TrimLeft "TrimLeft", \ref String_TrimNewLine "TrimNewLine",
 * \ref String_TrimRight "TrimRight"
 * - \ref String_Uppercase "Uppercase"
 */

#include "cljonic-concepts.hpp"
#include "cljonic-shared.hpp"

namespace cljonic
{

namespace core
{

/** \anchor Core_Equal
* The \b Equal function returns true if the values of its arguments are equal, else false.
~~~~~{.cpp}
#include <iostream>
#include "cljonic.hpp"

using namespace cljonic::core;

int main()
{
auto v1{Vector<int, 10>(1, 2, 3, 4)};
auto v2{Vector<int, 10>(1, 2, 3, 4)};
auto e0{Equal(v1)};                    // returns true whenever only one parameter is specified
auto e1{Equal(v1, v2)};                // returns true
auto e2{Equal(1, 2)};                  // returns false
// auto e3{Equal(1.1, 2.1)};           // compilation error: Floating point types should not be compared for equality
// auto e{Equal()};                    // compilation error: Must specify at least one parameter
// auto e{Equal(1.1, "Hello")};        // compilation error: Types are not comparable

return 0;
}
~~~~~
*/
template <typename T, typename... Ts>
auto Equal(const T& t, const Ts&... ts)
{
    /* An invocation of Equal with only one argument, like Equal(1) or Equal(myVector), returns true because
     * when only one argument is provided to "and" the single argument is trivially equal to itself,
     * and the result is true.
     */
    if constexpr (AllCljonicCollections<T, Ts...>)
    {
        static_assert(AllSameCljonicCollectionType<T, Ts...>,
                      "Cljonic collection types are not all the same (e.g., Vector, Set, or String)");
        static_assert(not AnyFloatingPointValueTypes<T, Ts...>,
                      "Cljonic floating point collection value types should not be compared for equality");
        static_assert(AllEqualityComparableValueTypes<T, Ts...>,
                      "Cljonic collection value types are not all equality comparable");
        if constexpr (AllCljonicSets<T, Ts...>)
        {
            constexpr auto EqualSets = [&](const auto& c1, const auto& c2)
            {
                using CountType = decltype(c1.Count());
                auto result{c1.Count() == c2.Count()};
                for (CountType i = 0; (result and (i < c1.Count())); ++i)
                    result = c2.Contains(c1[i]);
                return result;
            };
            return (EqualSets(t, ts) and ...);
        }
        else
        {
            constexpr auto EqualCollections = [&](const auto& c1, const auto& c2)
            {
                using CountType = decltype(c1.Count());
                auto result{c1.Count() == c2.Count()};
                for (CountType i = 0; (result and (i < c1.Count())); ++i)
                    result = AreEqual(c1[i], c2[i]);
                return result;
            };
            return (EqualCollections(t, ts) and ...);
        }
    }
    else
    {
        static_assert(not AnyFloatingPointTypes<T, Ts...>, "Floating point types should not be compared for equality");
        static_assert(AllEqualityComparableTypes<T, Ts...>, "Not all types are equality comparable");
        return (AreEqual(t, ts) and ...);
    }
}

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_HPP
