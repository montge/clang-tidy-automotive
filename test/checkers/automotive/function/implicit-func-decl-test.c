// RUN: %check_clang_tidy %s automotive-implicit-function-decl %t -- -- -std=c89
// Test for automotive-implicit-function-decl: function calls without prior declaration

// Note: In C89, implicit function declarations were allowed but are unsafe.
// This test verifies the check catches attempts to use undeclared functions.

void test_undeclared_function_call(void) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: function 'undeclared_function' is called without a prior declaration [automotive-implicit-function-decl]
  undeclared_function();  // No declaration exists
}

void test_undeclared_with_args(void) {
  int x = 10;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: function 'another_undeclared' is called without a prior declaration [automotive-implicit-function-decl]
  another_undeclared(x, 20);
}

void test_undeclared_with_return(void) {
  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: function 'get_value' is called without a prior declaration [automotive-implicit-function-decl]
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
