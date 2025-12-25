// RUN: %check_clang_tidy -std=c++17 %s automotive-cpp23-req-7.11.2 %t

// Test MISRA C++:2023 Rule 7.11.2 - nullptr shall be the only null pointer constant

#define NULL 0

void test_initialization() {
  // Compliant - using nullptr
  int* p1 = nullptr;
  char* p2 = nullptr;

  // Non-compliant - using 0
  int* p3 = 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: use 'nullptr' instead of integer literal '0'; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2) [automotive-cpp23-req-7.11.2]

  // Non-compliant - using NULL macro
  int* p4 = NULL;
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: use 'nullptr' instead of NULL macro; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)
}

void test_assignment() {
  int* p = nullptr;

  // Compliant
  p = nullptr;

  // Non-compliant - assignment with 0
  p = 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: use 'nullptr' instead of integer literal '0'; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  // Non-compliant - assignment with NULL
  p = NULL;
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: use 'nullptr' instead of NULL macro; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)
}

void test_comparison() {
  int* p = nullptr;

  // Compliant - comparison with nullptr
  if (p == nullptr) {}
  if (p != nullptr) {}

  // Non-compliant - comparison with 0
  if (p == 0) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: use 'nullptr' instead of integer literal '0' in pointer comparison; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  if (p != 0) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: use 'nullptr' instead of integer literal '0' in pointer comparison; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  // Non-compliant - comparison with NULL
  if (p == NULL) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: use 'nullptr' instead of NULL macro in pointer comparison; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  if (p != NULL) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: use 'nullptr' instead of NULL macro in pointer comparison; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  // Reversed order
  if (0 == p) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: use 'nullptr' instead of integer literal '0' in pointer comparison; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  if (NULL == p) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: use 'nullptr' instead of NULL macro in pointer comparison; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)
}

void function_taking_pointer(int* ptr) {}

void test_function_call() {
  // Compliant
  function_taking_pointer(nullptr);

  // Non-compliant - passing 0
  function_taking_pointer(0);
  // CHECK-MESSAGES: :[[@LINE-1]]:27: warning: use 'nullptr' instead of integer literal '0'; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  // Non-compliant - passing NULL
  function_taking_pointer(NULL);
  // CHECK-MESSAGES: :[[@LINE-1]]:27: warning: use 'nullptr' instead of NULL macro; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)
}

int* function_returning_pointer_good() {
  // Compliant
  return nullptr;
}

int* function_returning_pointer_bad1() {
  // Non-compliant - returning 0
  return 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: use 'nullptr' instead of integer literal '0'; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)
}

int* function_returning_pointer_bad2() {
  // Non-compliant - returning NULL
  return NULL;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: use 'nullptr' instead of NULL macro; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)
}

class TestClass {
public:
  TestClass(int* p) {}
};

void test_constructor() {
  // Compliant
  TestClass t1(nullptr);

  // Non-compliant - constructor with 0
  TestClass t2(0);
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: use 'nullptr' instead of integer literal '0'; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  // Non-compliant - constructor with NULL
  TestClass t3(NULL);
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: use 'nullptr' instead of NULL macro; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)
}

void test_ternary() {
  int* p1 = nullptr;
  int* p2 = nullptr;
  bool condition = true;

  // Compliant
  int* result1 = condition ? nullptr : p1;
  int* result2 = condition ? p1 : nullptr;

  // Non-compliant
  int* result3 = condition ? 0 : p1;
  // CHECK-MESSAGES: :[[@LINE-1]]:30: warning: use 'nullptr' instead of integer literal '0'; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  int* result4 = condition ? p1 : 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:35: warning: use 'nullptr' instead of integer literal '0'; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  int* result5 = condition ? NULL : p1;
  // CHECK-MESSAGES: :[[@LINE-1]]:30: warning: use 'nullptr' instead of NULL macro; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)

  int* result6 = condition ? p1 : NULL;
  // CHECK-MESSAGES: :[[@LINE-1]]:35: warning: use 'nullptr' instead of NULL macro; nullptr shall be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)
}

// Integer 0 used as integer should not warn
void test_integer_zero_not_pointer() {
  int i = 0;  // OK - this is an integer, not a pointer
  int j = 0;  // OK
  if (i == 0) {}  // OK - integer comparison
}
