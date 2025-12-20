// RUN: %check_clang_tidy %s automotive-cpp23-req-8.2.3 %t
// Test for automotive-cpp23-req-8.2.3: const_cast shall not be used

void test_const_cast() {
  const int x = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: const_cast shall not be used
  int* ptr = const_cast<int*>(&x);
  (void)ptr;

  const char* cstr = "hello";
  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: const_cast shall not be used
  char* str = const_cast<char*>(cstr);
  (void)str;

  volatile int vi = 10;
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: const_cast shall not be used
  int* pvi = const_cast<int*>(&vi);
  (void)pvi;
}

void test_compliant() {
  // Use mutable members instead of const_cast for logical constness
  // Use proper non-const references when modification is needed
  int x = 42;
  int* ptr = &x;  // OK - no const to cast away
  *ptr = 10;
}
