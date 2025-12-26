// RUN: %check_clang_tidy %s automotive-function-declaration-mismatch %t -- -- -std=c11
// Test for automotive-function-declaration-mismatch: function declarations shall match definitions

// Forward declaration
int add(int a, int b);

// CHECK-MESSAGES: :[[@LINE+2]]:13: warning: parameter name 'x' mismatch with 'a' in function 'add' [automotive-function-declaration-mismatch]
// CHECK-MESSAGES: :[[@LINE+1]]:20: warning: parameter name 'y' mismatch with 'b' in function 'add' [automotive-function-declaration-mismatch]
int add(int x, int y) {  // Different parameter names
  return x + y;
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
int main(void) {
  (void)add(1, 2);
  (void)multiply(3, 4);
  (void)helper(5);
  return 0;
}
