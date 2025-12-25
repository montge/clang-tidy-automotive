// RUN: %check_clang_tidy %s automotive-c25-req-8.3 %t -- -- -std=c11
// Test for automotive-c25-req-8.3: function declarations shall match definitions

// Forward declaration
int add(int a, int b);

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function declaration does not match previous declaration
int add(int x, int y) {  // Different parameter names
  return x + y;
}

// Declaration with different qualifiers
void process(const int *data);

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function declaration does not match previous declaration
void process(int *data) {  // Missing const
  (void)data;
}

// Compliant - matching declarations and definitions
int multiply(int a, int b);

int multiply(int a, int b) {  // OK - same parameter names and types
  return a * b;
}

// Compliant - no previous declaration
static int helper(int x) {
  return x * 2;
}

// Use functions to avoid warnings
void use_functions(void) {
  add(1, 2);
  int data = 5;
  process(&data);
  multiply(3, 4);
  helper(5);
}
