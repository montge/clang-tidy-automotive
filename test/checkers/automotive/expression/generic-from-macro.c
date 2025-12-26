// Test file for: automotive-c25-adv-23.1
// Related MISRA C:2025 Rule: 23.1
//
// This file tests the detection of _Generic expressions that are not
// expanded from a macro.

// RUN: %check_clang_tidy %s automotive-c25-adv-23.1 %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Define a macro that wraps _Generic
#define TYPE_ID(x) _Generic((x), int: 1, float: 2, double: 3, default: 0)

void test_generic_in_macro(void) {
    int x = 5;
    float y = 3.14f;

    // These use _Generic through a macro - compliant
    int result1 = TYPE_ID(x);
    int result2 = TYPE_ID(y);

    (void)result1;
    (void)result2;
}

// More complex macro example
#define CHOOSE_FUNC(x) _Generic((x), \
    int: process_int, \
    float: process_float, \
    default: process_default)(x)

void process_int(int x) { (void)x; }
void process_float(float x) { (void)x; }
void process_default(int x) { (void)x; }

void test_complex_macro(void) {
    int i = 10;
    float f = 5.5f;

    // Using _Generic through a macro - compliant
    CHOOSE_FUNC(i);
    CHOOSE_FUNC(f);
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_direct_generic_simple(void) {
    int x = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: generic selection should only be expanded from a macro, not used directly in source code [automotive-c25-adv-23.1]
    int result = _Generic(x, int: 1, float: 2, default: 0);

    (void)result;
}

void test_direct_generic_no_default(void) {
    int x = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: generic selection should only be expanded from a macro, not used directly in source code [automotive-c25-adv-23.1]
    int result = _Generic(x, int: 100, float: 200, double: 300);

    (void)result;
}

void test_direct_generic_as_function_arg(void) {
    int x = 42;

    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: generic selection should only be expanded from a macro, not used directly in source code [automotive-c25-adv-23.1]
    process_int(_Generic(x, int: x, float: 0, default: -1));
}

void test_direct_generic_in_expression(void) {
    int x = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: generic selection should only be expanded from a macro, not used directly in source code [automotive-c25-adv-23.1]
    int result = 5 + _Generic(x, int: 10, float: 20, default: 0);

    (void)result;
}

void test_direct_generic_with_types(void) {
    int x = 1;

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: generic selection should only be expanded from a macro, not used directly in source code [automotive-c25-adv-23.1]
    int result = _Generic(x,
        char: 1,
        short: 2,
        int: 3,
        long: 4,
        float: 5,
        double: 6,
        default: 0);

    (void)result;
}

void test_multiple_direct_generics(void) {
    int x = 1;
    float y = 2.0f;

    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: generic selection should only be expanded from a macro, not used directly in source code [automotive-c25-adv-23.1]
    int result1 = _Generic(x, int: 1, float: 2, default: 0);

    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: generic selection should only be expanded from a macro, not used directly in source code [automotive-c25-adv-23.1]
    int result2 = _Generic(y, int: 1, float: 2, default: 0);

    (void)result1;
    (void)result2;
}

void test_nested_generic_direct(void) {
    int x = 5;

    // CHECK-MESSAGES: :[[@LINE+2]]:18: warning: generic selection should only be expanded from a macro, not used directly in source code [automotive-c25-adv-23.1]
    // CHECK-MESSAGES: :[[@LINE+1]]:35: warning: generic selection should only be expanded from a macro, not used directly in source code [automotive-c25-adv-23.1]
    int result = _Generic(x, int: _Generic(x, int: 1, default: 2), default: 0);

    (void)result;
}
