// RUN: %check_clang_tidy %s automotive-uncomplete-function-prototype %t -- -- -std=c89
// Test for automotive-uncomplete-function-prototype: functions shall have complete prototypes

// CHECK-MESSAGES: :[[@LINE+2]]:5: warning: function is not in prototype form [automotive-uncomplete-function-prototype]
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function with no parameters must use 'void' in prototype [automotive-uncomplete-function-prototype]
int old_style_func();  // K&R style - no parameters specified

// CHECK-MESSAGES: :[[@LINE+2]]:6: warning: function is not in prototype form [automotive-uncomplete-function-prototype]
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function with no parameters must use 'void' in prototype [automotive-uncomplete-function-prototype]
void empty_parens_func();  // Empty parentheses

// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function is not in prototype form [automotive-uncomplete-function-prototype]
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

// OK - declaration with prototype
int declared_func(int x);

int declared_func(int x) {
  return x * 2;
}

int main(void) {
  return 0;
}
