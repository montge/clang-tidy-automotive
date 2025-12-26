// RUN: %check_clang_tidy %s automotive-cpp23-req-8.2.1 %t -- -- -std=c++17
// Test for automotive-cpp23-req-8.2.1: C-style casts shall not be used

void test_c_style_cast_int_violation() {
  double d = 3.14;
  // CHECK-MESSAGES: :[[@LINE+2]]:11: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  int i = (int)d;
  (void)i;
}

void test_c_style_cast_pointer_violation() {
  int x = 42;
  // CHECK-MESSAGES: :[[@LINE+2]]:15: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  void *ptr = (void *)&x;
  (void)ptr;
}

void test_c_style_cast_const_violation() {
  const int *cp = nullptr;
  // CHECK-MESSAGES: :[[@LINE+2]]:12: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  int *p = (int *)cp;
  (void)p;
}

void test_static_cast_compliant() {
  // OK - using C++ style cast
  double d = 3.14;
  int i = static_cast<int>(d);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  (void)i;
}

void test_reinterpret_cast_compliant() {
  // OK - using C++ style cast (though reinterpret_cast has its own rule)
  int x = 42;
  void *ptr = reinterpret_cast<void *>(&x);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  (void)ptr;
}

void test_const_cast_compliant() {
  // OK - using C++ style cast (though const_cast has its own rule)
  const int *cp = nullptr;
  int *p = const_cast<int *>(cp);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  (void)p;
}

void test_implicit_conversion_compliant() {
  // OK - implicit conversion, no cast
  int i = 42;
  double d = i;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: C-style casts shall not be used; use static_cast, const_cast, reinterpret_cast, or dynamic_cast instead [automotive-cpp23-req-8.2.1]
  (void)d;
}
