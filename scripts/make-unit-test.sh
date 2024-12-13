#!/usr/bin/env bash

get_current_directory () {
    pwd
}

get_processor_count () {
    grep processor /proc/cpuinfo | wc -l
}

CMAKELISTS_FILE_DIRECTORY=$1
CPU_COUNT=$(get_processor_count)
CURRENT_DIRECTORY=$(get_current_directory)
LAST_EXIT_CODE=0

enter_build_directory () {
    echo -n "Entering Build Directory ... "
    cd build
    LAST_EXIT_CODE=$?
    echo "Done"
}

execute_cljonic_unit_test_program () {
    echo "Executing Cljonic Unit Test Program"
    ./cljonic
    LAST_EXIT_CODE=$?
}

exit_build_directory () {
    echo -n "Exiting Build Directory ... "
    cd $CURRENT_DIRECTORY
    LAST_EXIT_CODE=$?
    echo "Done"
}

handle_error () { # <message>
    if [ "$LAST_EXIT_CODE" != "0" ]; then
        echo "***** Error: Could Not $1"
        exit_build_directory
        exit 1
    fi
}

handle_make_error () { # <message>
    if [ "$LAST_EXIT_CODE" != "0" ]; then
        echo "***** Error: Could Not $1"
        cd $CURRENT_DIRECTORY
        exit 1
    fi
}

make_cljonic_unit_test_program () {
    echo "Making Cljonic Unit Test Program"
    echo
    make -j $CPU_COUNT cljonic >/dev/null
    LAST_EXIT_CODE=$?
    echo
    echo "Done"
}

render_header () {
    echo
    echo '========================================'
    echo '== make unit-test '$CMAKELISTS_FILE_DIRECTORY
    echo '========================================'
}

################################################################################
## Main
################################################################################
if [ -d "build" ]; then
    render_header
    enter_build_directory ; handle_error "Enter Build Directory"
    make_cljonic_unit_test_program ; handle_make_error "Make Cljonic Unit Test Program"
    execute_cljonic_unit_test_program ; handle_error "Execute Cljonic Unit Test Program"
    exit_build_directory ; handle_error "leave Build Directory"
    exit 0
fi

scripts/make-all.sh $CMAKELISTS_FILE_DIRECTORY
exit 0
