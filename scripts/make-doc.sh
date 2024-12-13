#!/usr/bin/env bash

LAST_EXIT_CODE=0

generate_html () {
    echo -n "Generating HTML ... "
    cd doc
    rm -rf html 2>/dev/null >/dev/null
    doxygen Doxyfile 2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    cd ..
    echo "Done"
}

move_html_to_docs () {
    echo -n "Moving HTML To docs Directory ... "
    rm -rf docs 2>/dev/null >/dev/null
    mkdir docs 2>/dev/null >/dev/null
    mv doc/html/* docs 2>/dev/null >/dev/null
    LAST_EXIT_CODE=$?
    rm -rf doc/html 2>/dev/null >/dev/null
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
generate_html ; handle_error "Make Doc: Generate HTML"
move_html_to_docs ; handle_error "Move Doc: Move HTML To 'docs' Directory"
exit 0
