#!/usr/bin/env bash

LAST_EXIT_CODE=0

format_files () {
    echo -n "Formatting Files ... "
    CPPS=$(find 'code' -type f -name '*.cpp')
    HPPS=$(find 'code' -type f -name '*.hpp')
    clang-format \
        -i \
        -style="{AllowShortBlocksOnASingleLine: false, AllowShortCaseLabelsOnASingleLine: false, AllowShortFunctionsOnASingleLine: false, AllowShortIfStatementsOnASingleLine: false, AllowShortLoopsOnASingleLine: false, AlwaysBreakTemplateDeclarations: true, BasedOnStyle: LLVM, BinPackArguments: false, BinPackParameters: false, BreakBeforeBraces: Allman, ColumnLimit: 120, Cpp11BracedListStyle: true, IndentWidth: 4, PointerAlignment: Left, SortIncludes: false, SpaceBeforeParens: ControlStatements, Standard: Latest}" \
        $CPPS \
        $HPPS \
        2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    echo "Done"
}

handle_error () { # <message>
    if [ "$LAST_EXIT_CODE" != "0" ]; then
        echo "***** Error: Could Not $1"
        exit 1
    fi
}

render_header () {
    echo
    echo '========================================'
    echo '== make format'
    echo '========================================'
}

################################################################################
## Main
################################################################################
render_header
format_files ; handle_error "Formatting Files"
exit 0