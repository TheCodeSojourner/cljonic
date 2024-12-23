#!/usr/bin/env bash

echo
echo '========================================'
echo '== make noheap'
echo '========================================'
PWD=$(pwd)
cd resources
rm *.o 2>/dev/null >/dev/null
g++ -std=c++20 -I.. -c -Wfatal-errors no-dynamic-memory.cpp 2>/dev/null >/dev/null
if [ -f "no-dynamic-memory.o" ]; then
  echo "No heap usage detected"
else
  g++ -std=c++20 -I.. -c -Wfatal-errors no-dynamic-memory.cpp
fi
rm *.o 2>/dev/null >/dev/null
cd $PWD
exit 0
