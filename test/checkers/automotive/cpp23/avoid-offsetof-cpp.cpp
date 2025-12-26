// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-21.2.4 %t -- -- -std=c++17
// Test for automotive-cpp23-req-21.2.4: offsetof macro shall not be used

#include <cstddef>

struct Data {
  int x;
  int y;
};

void test_offsetof() {
  // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: avoid offsetof macro
  size_t offset = offsetof(Data, y);
  (void)offset;
}
