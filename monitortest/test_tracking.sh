#!/usr/bin/env bash
# Using clang instead of GCC.
set -e
clang test_violation_tracking.c ../results/uca_reason_tracking.c -o test_reasontracking_full
echo "Testing UCA Violation Reason tracking monitor with output."
./test_reasontracking_full
echo "======================="
echo "Test successfully run. You should see debug output above.!"

