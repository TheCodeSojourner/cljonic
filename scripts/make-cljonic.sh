#!/usr/bin/env bash

echo
echo '========================================'
echo '== make cljonic'
echo '========================================'
PWD=$(pwd)
echo -n 'Building "cljonic.hpp" ... '
cd code/source

# glue all the cljonic files together
# seq requires take so it must be first
cat cljonic-collection-iterator.hpp \
    cljonic-collection-type.hpp \
    cljonic-concepts.hpp \
    cljonic-shared.hpp \
    cljonic-array.hpp \
    cljonic-range.hpp \
    cljonic-repeat.hpp \
    cljonic-set.hpp \
    cljonic-string.hpp \
    cljonic-core.hpp \
    cljonic-core-compose.hpp \
    cljonic-core-concat.hpp \
    cljonic-core-conj.hpp \
    cljonic-core-count.hpp \
    cljonic-core-cycle.hpp \
    cljonic-core-equal.hpp \
    cljonic-core-equalby.hpp \
    cljonic-core-every.hpp \
    cljonic-core-filter.hpp \
    cljonic-core-map.hpp \
    cljonic-core-partial.hpp \
    cljonic-core-reduce.hpp \
    cljonic-core-take.hpp \
    cljonic-core-seq.hpp > /tmp/cljonic-glued.hpp

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
