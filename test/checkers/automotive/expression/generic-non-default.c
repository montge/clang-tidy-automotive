// Test file for: automotive-c25-adv-23.3
// Related MISRA C:2025 Rule: 23.3
//
// This file tests the detection of generic selections that contain only a
// default association without any type-specific associations.

// RUN: %check_clang_tidy %s automotive-c25-adv-23.3 %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_only_default(void) {
    int x = 0;

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: generic selection should contain at least one non-default association [automotive-c25-adv-23.3]
    int result = _Generic(x, default: 42);

    (void)result;
}

void test_only_default_in_expression(void) {
    int x = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: generic selection should contain at least one non-default association [automotive-c25-adv-23.3]
    if (_Generic(x, default: 1) > 0) {
        // This generic selection provides no type-specific behavior
    }
}

void test_only_default_as_initializer(void) {
    int x = 0;

    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: generic selection should contain at least one non-default association [automotive-c25-adv-23.3]
    int value = _Generic(x, default: 100);

    (void)value;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_with_one_type_and_default(void) {
    int x = 0;

    // Has at least one non-default association - compliant
    int result1 = _Generic(x, int: 1, default: 0);

    // Has at least one non-default association - compliant
    int result2 = _Generic(x, default: 0, int: 1);

    (void)result1;
    (void)result2;
}

void test_with_multiple_types_and_default(void) {
    int x = 0;

    // Multiple non-default associations - compliant
    int result1 = _Generic(x, int: 1, float: 2, default: 0);

    // Multiple non-default associations - compliant
    int result2 = _Generic(x, default: 0, int: 1, long: 2, float: 3, double: 4);

    (void)result1;
    (void)result2;
}

void test_without_default(void) {
    int x = 0;

    // No default association, but has type-specific associations - compliant
    int result1 = _Generic(x, int: 1, long: 2);

    // Multiple type-specific associations without default - compliant
    int result2 = _Generic(x, int: 1, long: 2, float: 3, double: 4);

    (void)result1;
    (void)result2;
}

void test_multiple_types_no_default(void) {
    int x = 0;

    // Has multiple non-default associations, no default - compliant
    int result = _Generic(x,
        char: 1,
        short: 2,
        int: 3,
        long: 4);

    (void)result;
}

void test_generic_in_macro(void) {
    int x = 0;

    // Compliant - has non-default associations
    #define GET_VALUE(x) _Generic((x), int: 1, float: 2.0f, default: 0)
    int result = GET_VALUE(x);

    (void)result;
}
