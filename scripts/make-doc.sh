#!/usr/bin/env bash

LAST_EXIT_CODE=0

generate_doc () {
    echo -n "Generating Documentation ... "
    cd doc
    rm -rf html 2>/dev/null >/dev/null
    doxygen Doxyfile 2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    cd ..
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
    echo '== make doc'
    echo '========================================'
}

################################################################################
## Main
################################################################################
render_header
generate_doc ; handle_error "Make Doc"
exit 0
