// RUN: %check_clang_tidy %s automotive-cpp23-req-6.7 %t
// Test for automotive-cpp23-req-6.7
// Related MISRA C++:2023 Rule: 6.7

// This test verifies that floating-point equality comparisons are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_float_equality() {
    float a = 0.1f + 0.2f;
    float b = 0.3f;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: floating-point equality comparison using '=='
    if (a == b) { }

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: floating-point inequality comparison using '!='
    if (a != b) { }
}

void test_double_equality() {
    double x = 0.1 + 0.2;
    double y = 0.3;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: floating-point equality comparison using '=='
    if (x == y) { }

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: floating-point inequality comparison using '!='
    if (x != y) { }
}

void test_long_double_equality() {
    long double ld1 = 1.0L;
    long double ld2 = 1.0L;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: floating-point equality comparison using '=='
    if (ld1 == ld2) { }
}

void test_mixed_types() {
    float f = 1.0f;
    double d = 1.0;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: floating-point equality comparison using '=='
    if (f == d) { }

    // Float compared to int literal (float is still there)
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: floating-point equality comparison using '=='
    if (f == 0) { }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_integer_equality() {
    int a = 1;
    int b = 1;

    // Integer comparison - no warning
    if (a == b) { }
    if (a != b) { }
}

void test_relational_operators() {
    float a = 1.0f;
    float b = 2.0f;

    // Relational operators are allowed
    if (a < b) { }
    if (a > b) { }
    if (a <= b) { }
    if (a >= b) { }
}

#include <cmath>

void test_epsilon_comparison() {
    float a = 0.1f + 0.2f;
    float b = 0.3f;
    const float epsilon = 1e-6f;

    // Proper epsilon-based comparison - no warning on this pattern
    if (std::abs(a - b) < epsilon) { }
}

void test_pointer_equality() {
    float *p1 = nullptr;
    float *p2 = nullptr;

    // Pointer comparison, not floating-point value comparison
    if (p1 == p2) { }
}
