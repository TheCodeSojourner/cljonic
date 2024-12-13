#!/usr/bin/env bash

CURRENT_DIRECTORY=$(pwd)
LAST_EXIT_CODE=0
LCOV_DIRECTORY="build/CMakeFiles/cljonic.dir"$CURRENT_DIRECTORY"/code/test/"

execute_lcov () {
    echo -n "Executing lcov ... "
    lcov -c -d $LCOV_DIRECTORY -o build/lcov.info 2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    echo "Done"
}

exit_build_directory () {
    echo -n "Exiting Build Directory ... "
    cd $CURRENT_DIRECTORY
    LAST_EXIT_CODE=$?
    echo "Done"
}

execute_genhtml () {
    echo -n "Executing genhtml ... "
    genhtml -o build/html build/lcov.info 2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    echo "Done"
}

generate_coverage_report () { # <htmlDir>
    INDEX_FILE_LIST=$(get_index_file_list $1)

    echo
    for INDEX_FILE in $INDEX_FILE_LIST; do
        egrep '<td class="headerCovTableEntryHi">(.*)<' $INDEX_FILE | head -n 1 | \
            perl -pe 's|<td class="headerCovTableEntryHi">(.*?)<.*|\1|' | \
            tr --delete "&nbsp;" | \
            tr --delete " " | \
            tr --delete "\n"
        echo " - " $INDEX_FILE
    done
}

get_index_file_list () { # <htmlDir>
    find $1 -type f -name index.html | grep source | grep -v resource | grep -v source-application-specific
}

handle_error () { # <message>
    if [ "$LAST_EXIT_CODE" != "0" ]; then
        echo "***** Error: Could Not $1"
        exit_build_directory
        exit 1
    fi
}

make_all_gcov () {
    echo -n "Making All gcov ... "
    scripts/make-all.sh gcov
    LAST_EXIT_CODE=$?
    echo "Done"
}

render_header () {
    echo
    echo '========================================'
    echo '== make coverage'
    echo '========================================'
}

################################################################################
## Main
################################################################################
render_header
make_all_gcov ; handle_error "Make All gcov"
execute_lcov ; handle_error "Execute lcov"
execute_genhtml ; handle_error "Execute genhtml"
generate_coverage_report build/html
exit 0
