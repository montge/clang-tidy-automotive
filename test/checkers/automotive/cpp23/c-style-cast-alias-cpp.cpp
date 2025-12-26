// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-adv-11.3 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-11.3: C-style casts shall not be used (alias)

void test_c_style_cast() {
  int x = 42;
  
  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: avoid C-style cast
  double d = (double)x;
  
  // OK - C++ style casts
  double d2 = static_cast<double>(x);
  (void)d;
  (void)d2;
}
