// RUN: %check_clang_tidy %s automotive-c25-req-17.3 %t -- -- -std=c99
// Test for automotive-c25-req-17.3: implicit function declarations shall not be used

// Note: In C99 and later, implicit function declarations are not allowed
// This test verifies the check catches attempts to use undeclared functions

void test_undeclared_function_call(void) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: implicit declaration of function
  undeclared_function();  // No declaration exists
}

void test_undeclared_with_args(void) {
  int x = 10;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: implicit declaration of function
  another_undeclared(x, 20);
}

void test_undeclared_with_return(void) {
  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: implicit declaration of function
  int y = get_value();  // Undeclared function with return
  (void)y;
}

// Proper forward declaration
int properly_declared(int x);

void test_declared_function_compliant(void) {
  // OK - function is declared before use
  int result = properly_declared(42);
  (void)result;
}

// Function definition makes it declared
void defined_function(void) {
  // function body
}

void test_defined_function_compliant(void) {
  // OK - function is defined before use
  defined_function();
}

// Standard library functions with proper includes
#include <stdlib.h>

void test_stdlib_function_compliant(void) {
  // OK - malloc is declared in stdlib.h
  void *ptr = malloc(100);
  free(ptr);
}
