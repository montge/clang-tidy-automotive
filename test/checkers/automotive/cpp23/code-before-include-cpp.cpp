// RUN: %check_clang_tidy %s automotive-cpp23-req-19.3.2 %t -- -- -std=c++17
// Test for automotive-cpp23-req-19.3.2: #include shall only be preceded by directives or comments

// OK - comment before include
// This is a comment
#include <cstdio>

// OK - preprocessor directive before include
#define FEATURE 1
#include <cstdlib>

int main() {
  return 0;
}
