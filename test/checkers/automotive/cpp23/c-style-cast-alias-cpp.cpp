// RUN: %check_clang_tidy %s automotive-cpp23-adv-11.3 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-11.3: C-style casts shall not be used (alias)

void test_c_style_cast() {
  int x = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-adv-11.3]
  double d = (double)x;

  // OK - C++ style casts
  double d2 = static_cast<double>(x);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-adv-11.3]
  (void)d;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-adv-11.3]
  (void)d2;
}
