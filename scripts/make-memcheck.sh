#!/usr/bin/env bash

execute_memcheck () {
    echo "Executing memcheck"
    valgrind \
        --gen-suppressions=all \
        --leak-check=yes \
        --quiet \
        --show-error-list=yes \
        --show-reachable=yes \
        --suppressions=resources/cljonic-valgrind-suppressions \
        --tool=memcheck \
        --track-origins=yes \
        --verbose \
        build/cljonic
    LAST_EXIT_CODE=$?
}

handle_error () { # <message>
    if [ "$LAST_EXIT_CODE" != "0" ]; then
        echo "***** Error: Could Not $1"
        exit 1
    fi
}

rebuild_unit_test () {
    echo -n "Rebuilding Unit Test ... "
    scripts/make-all.sh unit-test 2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    echo "Done"
}

render_header () {
    echo
    echo '========================================'
    echo '== make memcheck'
    echo '========================================'
}

################################################################################
## Main
################################################################################
render_header
rebuild_unit_test ; handle_error "Rebuild Unit Test"
execute_memcheck ; handle_error "Executing memcheck"
exit 0
