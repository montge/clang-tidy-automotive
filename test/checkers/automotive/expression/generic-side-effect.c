// RUN: %check_clang_tidy %s automotive-c25-req-23.2 %t -- -- -std=c11

// Test for side effects in _Generic controlling expression

int get_value(void);

void test_side_effects(void) {
  int x = 0;

  // Non-compliant: increment in controlling expression
  int a = _Generic(++x, int: 1, default: 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: controlling expression of _Generic contains increment which will not be evaluated

  // Non-compliant: decrement in controlling expression
  int b = _Generic(--x, int: 1, default: 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: controlling expression of _Generic contains decrement which will not be evaluated

  // Non-compliant: assignment in controlling expression
  int y;
  int c = _Generic((y = 5), int: 1, default: 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: controlling expression of _Generic contains assignment which will not be evaluated

  // Non-compliant: function call in controlling expression
  int d = _Generic(get_value(), int: 1, default: 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: controlling expression of _Generic contains function call

  // Compliant: simple variable reference
  int e = _Generic(x, int: 1, default: 0);

  // Compliant: pointer dereference (no side effect)
  int *p = &x;
  int f = _Generic(*p, int: 1, default: 0);

  // Compliant: array subscript
  int arr[10];
  int g = _Generic(arr[0], int: 1, default: 0);

  // Suppress unused variable warnings
  (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g;
}

// Compliant: when used via macro, side effects in controlling expression
// are evaluated in the macro expansion context
#define TYPE_SELECTOR(expr) _Generic((expr), int: 1, double: 2, default: 0)

void test_macro_usage(void) {
  int x = 0;
  // This is compliant because the expression is a macro argument
  int a = TYPE_SELECTOR(++x);
  (void)a;
}
