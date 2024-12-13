#!/usr/bin/env bash

execute_cppcheck () {
    cppcheck --enable=all --quiet $(find code/source -type f -name '*.[ch]pp') 2>/tmp/execute_cppcheck.txt >/dev/null
    if [ "$?" == "0" ]; then
        # sed -i '/cljonic-core.hpp:35:0: style: The function.*Equal.*never used/,/^$/d' /tmp/execute_cppcheck.txt
        sed -i '/.*\[unusedFunction\]/,/^$/d' /tmp/execute_cppcheck.txt
        sed -i '/.*information:/,/^$/d' /tmp/execute_cppcheck.txt
        cat /tmp/execute_cppcheck.txt
        LAST_EXIT_CODE=0
    else
        LAST_EXIT_CODE=1
    fi
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
    echo '== make cppcheck'
    echo '========================================'
}

################################################################################
## Main
################################################################################
render_header
execute_cppcheck ; handle_error "Execute cppcheck"
exit 0
