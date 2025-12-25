// RUN: %check_clang_tidy %s automotive-cpp23-adv-11.6.1 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-11.6.1: variables shall be initialized

void test_uninitialized() {
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: variable 'x' is not initialized
  int x;
  
  // OK - initialized
  int y = 0;
  int z{42};
  (void)x;
  (void)y;
  (void)z;
}
