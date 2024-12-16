#!/usr/bin/env bash

echo
echo '========================================'
echo '== make cljonic'
echo '========================================'
PWD=$(pwd)
echo -n 'Building "cljonic.hpp" ... '
cd code/source

# glue all the cljonic files together
cat cljonic-collection-type.hpp \
    cljonic-concepts.hpp \
    cljonic-shared.hpp \
    cljonic-array.hpp \
    cljonic-range.hpp \
    cljonic-set.hpp \
    cljonic-string.hpp \
    cljonic-core.hpp \
    cljonic-core-equal.hpp > /tmp/cljonic-glued.hpp

# remove all the comments
g++ -fpreprocessed -dD -E -o /tmp/cljonic.hpp /tmp/cljonic-glued.hpp

# remove unneeded lines
sed -i '/^#include "/d' /tmp/cljonic.hpp
sed -i '/^#ifndef CLJONIC_/d' /tmp/cljonic.hpp
sed -i '/^#define CLJONIC_/d' /tmp/cljonic.hpp
sed -i '/^#endif/d' /tmp/cljonic.hpp
sed -i '/^# *[0-9]/d' /tmp/cljonic.hpp

# prepare final cljonic.hpp
printf "// This file was generated $(date)\n\n" | \
cat ../../resources/cljonic-head.hpp \
    - \
    /tmp/cljonic.hpp \
    ../../resources/cljonic-tail.hpp > ../../cljonic.hpp

# clean up to minimize size
clang-format \
    -i \
    --style='{AllowShortBlocksOnASingleLine: true, AllowShortCaseLabelsOnASingleLine: true, AllowShortFunctionsOnASingleLine: true, AllowShortIfStatementsOnASingleLine: true, AllowShortLoopsOnASingleLine: true, AlwaysBreakTemplateDeclarations: true, BinPackArguments: true, BinPackParameters: true, BreakBeforeBraces: Attach, ColumnLimit: 0, Cpp11BracedListStyle: true, IndentWidth: 0, PointerAlignment: Left, SortIncludes: true, SpaceBeforeParens: Never, CompactNamespaces: true, MaxEmptyLinesToKeep: 1}' \
    ../../cljonic.hpp

echo 'Done'
cd $PWD
exit 0
