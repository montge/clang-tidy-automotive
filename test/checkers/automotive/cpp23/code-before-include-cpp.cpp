// RUN: %check_clang_tidy %s automotive-cpp23-req-19.3.2 %t
// Test for automotive-cpp23-req-19.3.2: #include shall only be preceded by directives or comments
// All test cases are compliant - no warnings expected

// OK - comment before include
// This is a comment
#include <stdio.h>

// OK - preprocessor directive before include
#define FEATURE 1
#include <stdlib.h>

int main() {
  return 0;
}
