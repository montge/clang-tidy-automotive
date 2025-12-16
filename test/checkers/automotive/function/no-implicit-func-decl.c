// Test for automotive-implicit-function-decl check
// MISRA C:2025 Rule 8.1 - Types shall be explicitly specified

// Declared function - compliant
int g(int x);

// Function calling undeclared function - non-compliant
void test_implicit_function_call(void) {
  int result = g(10);  // Compliant - g is declared

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: function 'undeclared_func' is called without a prior declaration
  undeclared_func();   // Non-compliant - implicit declaration

  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: function 'another_undeclared' is called without a prior declaration
  int x = another_undeclared(5);  // Non-compliant
}

// More test cases for implicit function declarations
void test_more_cases(void) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: function 'implicit_void' is called without a prior declaration
  implicit_void();

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: function 'implicit_in_expr' is called without a prior declaration
  int value = implicit_in_expr() + 1;
}

// Compliant cases - functions are declared
extern int declared_extern(int);
static int declared_static(int x) { return x; }

void test_compliant(void) {
  declared_extern(1);    // Compliant - declared
  declared_static(2);    // Compliant - defined
  g(3);                  // Compliant - declared above
}




