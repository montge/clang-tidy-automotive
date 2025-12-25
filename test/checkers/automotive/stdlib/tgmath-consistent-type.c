// RUN: %check_clang_tidy %s automotive-c25-req-21.23 %t -- -- -std=c11

// Test for consistent types in multi-argument tgmath macros

#include <math.h>

void test_pow(void) {
  float f = 1.0f;
  double d = 2.0;
  long double ld = 3.0L;

  // Non-compliant: mixing float and double
  double r1 = pow(f, d);
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: arguments to 'pow' have inconsistent types: 'float' and 'double'

  // Non-compliant: mixing double and long double
  long double r2 = powl(d, ld);
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: arguments to 'powl' have inconsistent types: 'double' and 'long double'

  // Compliant: both double
  double r3 = pow(d, d);

  // Compliant: both float
  float r4 = powf(f, f);

  // Compliant: both long double
  long double r5 = powl(ld, ld);

  (void)r1; (void)r2; (void)r3; (void)r4; (void)r5;
}

void test_atan2(void) {
  float f = 1.0f;
  double d = 2.0;

  // Non-compliant: mixing float and double
  double r1 = atan2(f, d);
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: arguments to 'atan2' have inconsistent types: 'float' and 'double'

  // Compliant: both double
  double r2 = atan2(d, d);

  (void)r1; (void)r2;
}

void test_fma(void) {
  float f = 1.0f;
  double d = 2.0;

  // Non-compliant: mixing types in 3-argument function
  double r1 = fma(f, d, d);
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: arguments to 'fma' have inconsistent types: 'float' and 'double'

  // Compliant: all double
  double r2 = fma(d, d, d);

  (void)r1; (void)r2;
}

void test_remainder(void) {
  float f = 1.0f;
  double d = 2.0;

  // Non-compliant: mixing types
  double r1 = remainder(f, d);
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: arguments to 'remainder' have inconsistent types: 'float' and 'double'

  // Compliant: both double
  double r2 = remainder(d, d);

  (void)r1; (void)r2;
}

void test_compliant_cases(void) {
  float f1 = 1.0f, f2 = 2.0f;
  double d1 = 1.0, d2 = 2.0;

  // All these should be compliant
  float r1 = fmaxf(f1, f2);
  float r2 = fminf(f1, f2);
  double r3 = fmax(d1, d2);
  double r4 = fmin(d1, d2);
  double r5 = hypot(d1, d2);
  double r6 = copysign(d1, d2);

  (void)r1; (void)r2; (void)r3; (void)r4; (void)r5; (void)r6;
}
