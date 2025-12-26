// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-21.10.3 %t -- -- -std=c++17
// Test for automotive-cpp23-req-21.10.3: <csignal> header shall not be used

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: <csignal> header shall not be used
#include <csignal>

void test_signal() {
  // Using signal is not allowed
}
