// RUN: %check_clang_tidy %s automotive-c25-req-23.6 %t -- -- -std=c11

// Test for essential type matching standard type in _Generic

void test_short_promotion(void) {
  short s16 = 0;

  // Non-compliant: s16 + s16 has essential type short but standard type int
  int a = _Generic(s16 + s16, short: 1, int: 2, default: 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: controlling expression was promoted from 'short' to 'int'

  // Compliant: s16 has same essential and standard type
  int b = _Generic(s16, short: 1, default: 0);

  (void)a; (void)b;
}

void test_char_literal(void) {
  // Non-compliant: 'c' has essential type char but standard type int
  int a = _Generic('c', char: 1, int: 2, default: 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: controlling expression has essential type 'Character' but standard type 'Signed'

  // Compliant: explicit char variable
  char c = 'x';
  int b = _Generic(c, char: 1, default: 0);

  (void)a; (void)b;
}

void test_enum_constant(void) {
  enum E { A = 0, B = 1 };

  // Note: enum variable 'e' has type 'enum E', not int, so it won't trigger
  // The issue is with enum *constants* which have type int in C
  // Non-compliant: enum constant A has essential type enum but standard type int
  int a = _Generic(A, int: 1, default: 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: controlling expression has essential type 'Enum' but standard type 'Signed'

  (void)a;
}

void test_constant_exception(void) {
  // Compliant by exception: integer constant expressions
  // (that are not character constants and not boolean)
  int a = _Generic(10u, unsigned char: 1, unsigned int: 2, default: 0);
  int b = _Generic(250 + 350, short: 1, int: 2, default: 0);

  (void)a; (void)b;
}

void test_compliant_cases(void) {
  int i32 = 0;
  long l64 = 0;
  float f32 = 0.0f;

  // Compliant: types match
  int a = _Generic(i32, int: 1, default: 0);
  int b = _Generic(l64, long: 1, default: 0);
  int c = _Generic(f32, float: 1, default: 0);

  (void)a; (void)b; (void)c;
}
