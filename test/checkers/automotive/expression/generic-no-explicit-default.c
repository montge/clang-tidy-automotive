// XFAIL: *
// Test file for: automotive-generic-no-explicit-default
// Generic selections should not have explicit default associations
//
// RUN: %check_clang_tidy %s automotive-generic-no-explicit-default %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

#define TYPE_ID_NO_DEFAULT(x) _Generic((x), int: 1, float: 2, double: 3)

void test_no_default(void) {
    int x = 5;

    // Compliant: no default association
    int result1 = _Generic(x, int: 1, float: 2, double: 3);

    // Compliant: macro without default
    int result2 = TYPE_ID_NO_DEFAULT(x);

    (void)result1;
    (void)result2;
}

void test_explicit_types_only(void) {
    float f = 3.14f;

    // Compliant: all types explicitly listed
    int result = _Generic(f,
        char: 1,
        short: 2,
        int: 3,
        long: 4,
        float: 5,
        double: 6);

    (void)result;
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_with_default_simple(void) {
    int x = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: generic selection should not have an explicit default association [automotive-generic-no-explicit-default]
    int result = _Generic(x, int: 1, float: 2, default: 0);

    (void)result;
}

void test_default_only(void) {
    int x = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: generic selection should not have an explicit default association [automotive-generic-no-explicit-default]
    int result = _Generic(x, default: 42);

    (void)result;
}

void test_default_first(void) {
    float y = 3.14f;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: generic selection should not have an explicit default association [automotive-generic-no-explicit-default]
    int result = _Generic(y, default: 0, int: 1, float: 2);

    (void)result;
}

void test_default_middle(void) {
    double d = 2.71;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: generic selection should not have an explicit default association [automotive-generic-no-explicit-default]
    int result = _Generic(d, int: 1, default: 0, double: 3);

    (void)result;
}

void test_multiple_with_default(void) {
    int x = 1;
    float y = 2.0f;

    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: generic selection should not have an explicit default association [automotive-generic-no-explicit-default]
    int result1 = _Generic(x, int: 1, float: 2, default: 0);

    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: generic selection should not have an explicit default association [automotive-generic-no-explicit-default]
    int result2 = _Generic(y, int: 1, float: 2, default: 0);

    (void)result1;
    (void)result2;
}

#define TYPE_ID_WITH_DEFAULT(x) _Generic((x), int: 1, float: 2, default: 0)

void test_macro_with_default(void) {
    int x = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: generic selection should not have an explicit default association [automotive-generic-no-explicit-default]
    int result = TYPE_ID_WITH_DEFAULT(x);

    (void)result;
}
