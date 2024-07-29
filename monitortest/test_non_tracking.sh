#!/usr/bin/env bash
# Using clang instead of GCC because gcc appears to run out of memory even with 32 GB of Ram on the monolithic_nba.
set -e
if test -f ../results/uca_reason_tracking.c; then
  clang test.c ../results/uca_reason_tracking.c -o test_reasontracking
  echo "Testing UCA Violation Reason tracking monitor."
  ./test_reasontracking
fi
clang test.c ../results/monolithic_nba.c -o test_monolithic
echo "Testing monolithic monitor."
./test_monolithic
clang test.c ../results/monolithic_det_monitor.c -o test_deterministic
echo "Testing deterministic monitor."
./test_deterministic
clang test.c ../results/fragmented_monitor.c -o test_fragmented
echo "Testing fragmented monitor."
./test_fragmented
clang test.c ../results/uca.c -o test_uca
echo "Testing UCA monitor."
./test_uca


echo "==========================="
echo "All tests successfully run!"
echo "==========================="

