#!/usr/bin/env bash

echo
echo '========================================'
echo '== make cljonic'
echo '========================================'
PWD=$(pwd)
echo -n 'Building "cljonic.hpp" ... '
cd code/source

# glue all the cljonic files together
cat cljonic-collection-iterator.hpp \
    cljonic-collection-type.hpp \
    cljonic-concepts.hpp \
    cljonic-shared.hpp \
    cljonic-pre-declarations.hpp \
    cljonic-array.hpp \
    cljonic-lazyiterate.hpp \
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
    cljonic-core-dedupe.hpp \
    cljonic-core-dedupeby.hpp \
    cljonic-core-defaultelement.hpp \
    cljonic-core-drop.hpp \
    cljonic-core-droplast.hpp \
    cljonic-core-dropwhile.hpp \
    cljonic-core-equal.hpp \
    cljonic-core-equalby.hpp \
    cljonic-core-every.hpp \
    cljonic-core-filter.hpp \
    cljonic-core-first.hpp \
    cljonic-core-identical.hpp \
    cljonic-core-identity.hpp \
    cljonic-core-inc.hpp \
    cljonic-core-indexof.hpp \
    cljonic-core-indexofby.hpp \
    cljonic-core-interleave.hpp \
    cljonic-core-interpose.hpp \
    cljonic-core-isdistinct.hpp \
    cljonic-core-isdistinctby.hpp \
    cljonic-core-isempty.hpp \
    cljonic-core-isfull.hpp \
    cljonic-core-last.hpp \
    cljonic-core-lastindexof.hpp \
    cljonic-core-lastindexofby.hpp \
    cljonic-core-map.hpp \
    cljonic-core-max.hpp \
    cljonic-core-maxby.hpp \
    cljonic-core-min.hpp \
    cljonic-core-minby.hpp \
    cljonic-core-notany.hpp \
    cljonic-core-notevery.hpp \
    cljonic-core-nth.hpp \
    cljonic-core-partial.hpp \
    cljonic-core-reduce.hpp \
    cljonic-core-remove.hpp \
    cljonic-core-replace.hpp \
    cljonic-core-reverse.hpp \
    cljonic-core-second.hpp \
    cljonic-core-seq.hpp \
    cljonic-core-size.hpp \
    cljonic-core-some.hpp \
    cljonic-core-sort.hpp \
    cljonic-core-sortby.hpp \
    cljonic-core-splitat.hpp \
    cljonic-core-splitwith.hpp \
    cljonic-core-subs.hpp \
    cljonic-core-take.hpp \
    cljonic-core-takelast.hpp \
    cljonic-core-takenth.hpp \
    cljonic-core-takewhile.hpp > /tmp/cljonic-glued.hpp

# remove all the comments
g++ -fpreprocessed -dD -E -o /tmp/cljonic.hpp /tmp/cljonic-glued.hpp

# remove unneeded lines
sed -i '/^#include "/d' /tmp/cljonic.hpp
sed -i '/^#ifndef CLJONIC_/d' /tmp/cljonic.hpp
sed -i '/^#define CLJONIC_/d' /tmp/cljonic.hpp
sed -i '/^#endif/d' /tmp/cljonic.hpp
sed -i '/^# *[0-9]/d' /tmp/cljonic.hpp

# prepare final cljonic.hpp
printf "\n// This file was generated $(date)\n\n" | \
cat ../../resources/cljonic-head.hpp \
    - \
    cljonic-collection-maximum-element-count.hpp \
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
