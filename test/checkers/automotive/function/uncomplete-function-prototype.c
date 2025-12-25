// RUN: %check_clang_tidy %s automotive-c25-req-8.2 %t -- -- -std=c11
// Test for automotive-c25-req-8.2: functions shall have complete prototypes

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function has incomplete prototype
int old_style_func();  // K&R style - no parameters specified

// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function has incomplete prototype
void empty_parens_func();  // Empty parentheses

// CHECK-MESSAGES: :[[@LINE+2]]:5: warning: function has incomplete prototype
// K&R style definition
int kr_definition(x, y)
int x;
int y;
{
  return x + y;
}

// OK - complete prototype with void
void no_params_func(void) {
  // function body
}

// OK - complete prototype with parameters
int complete_func(int a, int b) {
  return a + b;
}

// OK - variadic function with at least one parameter
int variadic_func(const char *fmt, ...) {
  (void)fmt;
  return 0;
}

// OK - function pointer with complete prototype
typedef int (*func_ptr_t)(int, int);

void use_func_ptr(func_ptr_t fp) {
  int result = fp(1, 2);
  (void)result;
}

// CHECK-MESSAGES: :[[@LINE+1]]:22: warning: function has incomplete prototype
typedef int (*incomplete_fp_t)();  // Function pointer without params

void test_calls(void) {
  old_style_func();  // Call to incomplete prototype
  empty_parens_func();  // Call to incomplete prototype
  no_params_func();  // OK
  complete_func(1, 2);  // OK
}
