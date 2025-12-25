// RUN: %check_clang_tidy %s automotive-cpp23-req-19.3.1 %t -- -- -std=c++17
// Test for automotive-cpp23-req-19.3.1: #include directive shall use standard form

// OK - standard form includes
#include <iostream>
#include "local_header.h"

// The check validates include syntax
int main() {
  return 0;
}
