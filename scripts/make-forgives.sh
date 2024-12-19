#!/usr/bin/env bash

generate_forgives () {
    echo "Generating List Of code/source Files With Disabled Metrics Notification Functions"
    echo
    grep 'lizard forgives' code/source/*
    echo
    echo "Done"
}

render_header () {
    echo
    echo '========================================'
    echo '== make forgives'
    echo '========================================'
}

################################################################################
## Main
################################################################################
render_header
generate_forgives
exit 0