// Test file for: automotive-c25-mand-21.22
// Related MISRA C:2025 Rule: 21.22
//
// All operand arguments to type-generic macros in <tgmath.h> shall have an
// appropriate essential type.

// RUN: %check_clang_tidy %s automotive-c25-mand-21.22 %t

#include <tgmath.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_integer_argument_violations(void) {
    int x = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'sin' has inappropriate type 'int'; expected floating-point or complex type
    double result1 = sin(x);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'cos' has inappropriate type 'int'; expected floating-point or complex type
    double result2 = cos(10);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'sqrt' has inappropriate type 'int'; expected floating-point or complex type
    double result3 = sqrt(16);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'exp' has inappropriate type 'int'; expected floating-point or complex type
    double result4 = exp(2);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'log' has inappropriate type 'int'; expected floating-point or complex type
    double result5 = log(100);

    long y = 10L;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'tan' has inappropriate type 'long'; expected floating-point or complex type
    double result6 = tan(y);

    unsigned int z = 25U;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'ceil' has inappropriate type 'unsigned int'; expected floating-point or complex type
    double result7 = ceil(z);

    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    (void)result5;
    (void)result6;
    (void)result7;
}

void test_pow_integer_arguments(void) {
    int base = 2;
    int exponent = 3;

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'pow' has inappropriate type 'int'; expected floating-point or complex type
    double result1 = pow(base, 3.0);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 2 to type-generic macro 'pow' has inappropriate type 'int'; expected floating-point or complex type
    double result2 = pow(2.0, exponent);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'pow' has inappropriate type 'int'; expected floating-point or complex type
    double result3 = pow(base, exponent);

    (void)result1;
    (void)result2;
    (void)result3;
}

void test_atan2_integer_arguments(void) {
    int y = 1;
    int x = 1;

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'atan2' has inappropriate type 'int'; expected floating-point or complex type
    double result1 = atan2(y, 1.0);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 2 to type-generic macro 'atan2' has inappropriate type 'int'; expected floating-point or complex type
    double result2 = atan2(1.0, x);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: argument 1 to type-generic macro 'atan2' has inappropriate type 'int'; expected floating-point or complex type
    double result3 = atan2(y, x);

    (void)result1;
    (void)result2;
    (void)result3;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_float_arguments(void) {
    float f = 3.14f;
    double d = 2.718;
    long double ld = 1.414L;

    // Compliant: float argument
    double result1 = sin(f);

    // Compliant: double argument
    double result2 = cos(d);

    // Compliant: long double argument
    double result3 = tan(ld);

    // Compliant: floating-point literal
    double result4 = sqrt(16.0);

    // Compliant: floating-point expression
    double result5 = exp(d + 1.0);

    // Compliant: pow with floating-point arguments
    double result6 = pow(2.0, 3.0);

    // Compliant: atan2 with floating-point arguments
    double result7 = atan2(1.0, 1.0);

    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    (void)result5;
    (void)result6;
    (void)result7;
}

void test_compliant_complex_arguments(void) {
    // Compliant: complex types are appropriate
    _Complex double cd = 1.0 + 2.0 * _Complex_I;
    _Complex double result1 = sin(cd);
    _Complex double result2 = cos(cd);
    _Complex double result3 = exp(cd);

    (void)result1;
    (void)result2;
    (void)result3;
}

void test_compliant_various_functions(void) {
    double x = 1.5;
    double y = 2.5;

    // Compliant: various tgmath functions with floating-point arguments
    double result1 = acos(0.5);
    double result2 = asin(0.5);
    double result3 = sinh(x);
    double result4 = cosh(x);
    double result5 = tanh(x);
    double result6 = log10(100.0);
    double result7 = log2(8.0);
    double result8 = floor(x);
    double result9 = ceil(y);
    double result10 = fabs(-3.14);
    double result11 = fmod(x, y);
    double result12 = hypot(3.0, 4.0);

    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    (void)result5;
    (void)result6;
    (void)result7;
    (void)result8;
    (void)result9;
    (void)result10;
    (void)result11;
    (void)result12;
}
