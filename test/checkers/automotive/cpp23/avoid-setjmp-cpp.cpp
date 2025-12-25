// RUN: %check_clang_tidy %s automotive-cpp23-req-21.10.2 %t -- -- -std=c++17
// Test for automotive-cpp23-req-21.10.2: <csetjmp> header shall not be used

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: <csetjmp> header shall not be used
#include <csetjmp>

void test_setjmp() {
  // Using setjmp is not allowed
}
