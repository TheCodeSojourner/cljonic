#!/usr/bin/env bash

CURRENT_DIRECTORY=$(pwd)
LAST_EXIT_CODE=0
LCOV_DIRECTORY="build/CMakeFiles/cljonic.dir"$CURRENT_DIRECTORY"/code/"

build_unit_test () {
    echo -n "Make Unit Test gcov ... "
    scripts/make-unit-test.sh gcov 2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    echo "Done"
}

execute_lcov () {
    echo -n "Executing lcov ... "
    lcov -c -d $LCOV_DIRECTORY -o build/lcov.info 2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    echo "Done"
}

execute_genhtml () {
    echo -n "Executing genhtml ... "
    genhtml -o build/html build/lcov.info 2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    echo "Done"
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
build_unit_test ; handle_error "Build Unit Test"
execute_lcov ; handle_error "Execute lcov"
execute_genhtml ; handle_error "Execute genhtml"
exit 0
