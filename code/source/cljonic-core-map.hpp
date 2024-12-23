#ifndef CLJONIC_CORE_MAP_HPP
#define CLJONIC_CORE_MAP_HPP

namespace cljonic
{

namespace core
{

/** \anchor Core_Map
* The \b Map function returns a \b cljonic \b Array of the values returned by calling the function \b f on consecutive
* elements of one or more \b cljonic \b collections.
*
* Three overloads of Map are implemented.
*
* <b>NOTE: THE NUMBER OF VALUES RETURNED WILL ALWAYS BE EQUAL TO THE MINIMUM OF SOURCE COUNTS.</b>
*
* The first overload is a \b Unary \b Map function, which takes a unary map function \b uf and one \b src. The
* following shows examples of a lambda \b uf, and a named \b uf.
*
*     auto src{Array(int, 3){1, 2, 3}};
*     Map(MapUnaryFn(double, int, i, { return 2.2 * i; }), src);
*
* A unary function named \b uf can be defined as follows:
*
*     MapUnaryFunctionDef(uf, double, int, i)
*     {
*         return 2.2 * i;
*     }
*
* and used as follows:
*
*     auto src{Array(int, 3){1, 2, 3}};
*     Map(MapUnaryFunction(uf), src);
*
* In both cases the Map function returns [2.2, 4.4].
*
* The second overload is a \b Binary \b Map function, which takes a binary map function \b bf and two \b sources.
* The following shows examples of a lambda \b bf, and a named \b bf.
*
*     auto src1{Array(int, 3){1, 2, 3}};
*     Array(double, 3) src2{1.1, 2.2, 3.3};
*     Map(MapBinaryFn(double, int, i, double, d, { return d * i; }), src1, src2);
*
* A binary function named \b bf can be defined as follows:
*
*     MapBinaryFunctionDef(bf, double, int, i, double, d)
*     {
*         return d * i;
*     }
*
* and used as follows:
*
*     auto src1{Array(int, 3){1, 2, 3}};
*     Array(double, 3) src2{1.1, 2.2, 3.3};
*     Map(MapBinaryFunction(bf), src1, src2);
*
* In both cases the Map function will return [1.1, 4.4].
*
* The third overload is a \b Ternary \b Map function, which takes a ternary map function \b tf and three \b sources.
* The following shows examples of a lambda \b tf, and a named \b tf.
*
*     auto src1{Array(int, 3){1, 2, 3}};
*     auto src2{Array(double, 3){1.1, 2.2, 3.3}};
*     auto src3{Array(double, 4){10.1, 20.2, 30.3, 40.4}};
*     Map(MapTernaryFn(double, int, i, double, d1, double, d2, { return d2 + (d1 * i); }),
*         src1,
*         src2,
*         src3);
*
* A ternary function named \b tf can be defined as follows:
*
*     MapTernaryFunctionDef(tf, double, int, i, double, d1, double, d2)
*     {
*         return d2 + (d1 * i);
*     }
*
* and used as follows:
*
*     auto src1{Array(int, 3){1, 2, 3}};
*     auto src2{Array(double, 3){1.1, 2.2, 3.3}};
*     auto src3{Array(double, 4){10.1, 20.2, 30.3, 40.4}};
*     Map(MapTernaryFunction(tf), src1, src2, src3);
*
* In both cases the Map function will return [11.2, 24.6].
*
~~~~~{.cpp}
#include <iostream>
#include <string>
#include "curt.hpp"

using namespace curt::core;

template <typename ElementType, TCount ElementSize>
auto ArrayToString(const Array(ElementType, ElementSize) & array) noexcept
{
auto result{std::string{""}};
DoSeq(array, DoSeqArrayFn(ElementType, e, {
result += std::to_string(e) + " ";
CONTINUE_PROCESSING_SEQ;
}));
return result;
}

int main()
{
auto mapUnaryArray{Array(int, 5){3, 7, 11, 15, 19}};
std::cout << "Map(MapUnaryFn(...), mapUnaryArray):\n    "
<< ArrayToString
(Map(MapUnaryFn(double, int, i, { return static_cast<double>(2.1 * i); }), mapUnaryArray))
<< "\n";

auto mapBinaryRange{RangeSE(10, 14)};
auto mapBinaryRepeat{RepeatTCV(double, 4, 2.2)};
std::cout << "Map(MapBinaryFn(...), mapBinaryRange, mapBinaryRepeat):\n    "
<< ArrayToString
(Map(MapBinaryFn(double, TRange, i, double, j, { return (static_cast<double>(i) * j); }),
mapBinaryRange,
mapBinaryRepeat))
<< "\n";

auto mapTernaryArray{Array(int, 5){3, 7, 11, 15, 19}};
auto mapTernaryRange{RangeSE(10, 14)};
auto mapTernaryRepeat{RepeatTCV(double, 4, 2.2)};
std::cout << "Map(MapTernaryFn(...), mapTernaryRepeat, mapTernaryRange, mapTernaryArray):\n    "
<< ArrayToString(Map(
MapTernaryFn(double, double, i, TRange, j, int, k, { return (i * static_cast<double>(j * k)); }),
mapTernaryRepeat,
mapTernaryRange,
mapTernaryArray))
<< "\n";

return 0;
}
~~~~~
*/

} // namespace core

} // namespace cljonic

#endif // CLJONIC_CORE_MAP_HPP
