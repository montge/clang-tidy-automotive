// RUN: %check_clang_tidy %s automotive-cpp23-mand-6.8.2 %t

// MISRA C++:2023 Rule 6.8.2: A function must not return a reference or a
// pointer to a local variable with automatic storage duration.

typedef int int32_t;

// Non-compliant: returning address of local variable
int32_t* f1() {
  int32_t x = 99;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: returning address of local variable 'x' with automatic storage duration
  return &x;
}

// Non-compliant: returning address of parameter (passed by value)
int32_t* f2(int32_t y) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: returning address of local variable 'y' with automatic storage duration
  return &y;
}

// TODO: Reference returns need enhanced detection
// int32_t& f3() {
//   int32_t x = 99;
//   return x;  // Non-compliant - reference to local
// }

// TODO: Reference returns need enhanced detection
// int32_t& f4(int32_t y) {
//   return y;  // Non-compliant - reference to parameter
// }

// Compliant: returning reference to reference parameter
int32_t& f5(int32_t& x) {
  return x;  // OK - x is not a local with automatic storage
}

// Compliant: returning pointer to static local
int32_t* f6() {
  static int32_t x = 0;
  return &x;  // OK - x has static storage duration
}

// Non-compliant: throwing address of local variable
void f7() {
  int32_t x = 0;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: throwing address of local variable 'x' with automatic storage duration
  throw &x;
}

// Compliant: throw caught within same function
void f8() {
  try {
    int32_t x = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: throwing address of local variable 'x' with automatic storage duration
    throw &x;  // Would be caught locally, but still flagged as dangerous pattern
  } catch (...) {
  }
}

// Compliant: returning value (not address/reference)
int32_t f9() {
  int32_t x = 42;
  return x;  // OK - returning by value
}

// Compliant: returning pointer to heap-allocated memory
int32_t* f10() {
  int32_t* p = new int32_t(42);
  return p;  // OK - p points to heap memory
}
