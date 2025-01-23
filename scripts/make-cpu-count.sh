#!/usr/bin/env bash

echo $(grep processor /proc/cpuinfo | wc -l) "/ 2" | bc

