#!/usr/bin/env bash

LAST_EXIT_CODE=0

format_files () {
    CODE_CPPS=$(find 'code' -type f -name '*.cpp')
    CODE_HPPS=$(find 'code' -type f -name '*.hpp')
    clang-format \
        -i \
        -style="{AllowShortBlocksOnASingleLine: false, AllowShortCaseLabelsOnASingleLine: false, AllowShortFunctionsOnASingleLine: false, AllowShortIfStatementsOnASingleLine: false, AllowShortLoopsOnASingleLine: false, AlwaysBreakTemplateDeclarations: true, BasedOnStyle: LLVM, BinPackArguments: false, BinPackParameters: false, BreakBeforeBraces: Allman, ColumnLimit: 120, Cpp11BracedListStyle: true, IndentWidth: 4, PointerAlignment: Left, SortIncludes: false, SpaceBeforeParens: ControlStatements, Standard: Latest}" \
        $CODE_CPPS \
        $CODE_HPPS \
        resources/no-heap.hpp \
        2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
}

handle_error () { # <message>
    if [ "$LAST_EXIT_CODE" != "0" ]; then
        echo "***** Error: Could Not $1"
        exit 1
    fi
}

################################################################################
## Main
################################################################################
format_files ; handle_error "Formatting Files"
exit 0