// RUN: %check_clang_tidy %s automotive-cpp23-req-19.3.1 %t
// Test for automotive-cpp23-req-19.3.1: #include directive shall use standard form
// All test cases are compliant - no warnings expected

// OK - standard form includes
#include <stdio.h>

// The check validates include syntax
int main() {
  return 0;
}
