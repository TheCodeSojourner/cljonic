#!/usr/bin/env bash

render_header () {
    echo
    echo '========================================'
    echo '== make clean'
    echo '========================================'
}

################################################################################
## Main
################################################################################
render_header
rm -rf build >/dev/null 2>/dev/null
exit 0
