#!/usr/bin/env bash

generate_metrics () {
    echo "Generating Metrics"
    echo
    HPPS=$(find 'code/source' -type f -name '*.hpp')
    lizard --CCN 3 --length 15 --arguments 4 -Tnloc=12 --warnings_only $HPPS
}

render_header () {
    echo
    echo '========================================'
    echo '== make metrics_source'
    echo '========================================'
}

################################################################################
## Main
################################################################################
render_header
generate_metrics 
exit 0