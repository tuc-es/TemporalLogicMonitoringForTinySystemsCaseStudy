#!/usr/bin/env bash
# Using clang instead of GCC because gcc appears to run out of memory even with 32 GB of Ram on the monolithic_nba.
set -e
clang test.c ../results/monolithic_nba.c -o test_monolithic
echo "Testing monolithic monitor."
./test_monolithic
clang test.c ../results/monolithic_det_monitor.c -o test_deterministic
echo "Testing deterministic monitor."
./test_deterministic
clang test.c ../results/fragmented_monitor.c -o test_fragmented
echo "Testing fragmented monitor."
./test_fragmented


echo "==========================="
echo "All tests successfully run!"
echo "==========================="

