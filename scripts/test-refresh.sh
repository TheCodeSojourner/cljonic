#!/usr/bin/env bash

if [ "$1" == "--help" ]; then
    echo
    echo "Usage: test-refresh.sh [--build-all]"
    echo
    echo "Where --build-all causes 'make clean test' to be executed prior to"
    echo "monitoring and doing 'make test' whenever files change."
    echo
    exit
fi

clear
echo
echo '========================================'
echo '== test-refresh'
echo '========================================'

if [ "$1" == "--build-all" ]; then
    echo "Initial 'make all'"
    echo
    make all
fi

while sleep 1; do
    find code | entr -c -p sh -c 'make test';
done