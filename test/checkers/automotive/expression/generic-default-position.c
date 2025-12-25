// Test file for: automotive-c25-req-23.8
// Related MISRA C:2025 Rule: 23.8
//
// This file tests the detection of default associations that are not
// positioned first or last in a generic selection.

// RUN: %check_clang_tidy %s automotive-c25-req-23.8 %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_default_in_middle(void) {
    int x = 0;

    // CHECK-MESSAGES: :[[@LINE+1]]:48: warning: default association in generic selection should appear first or last, not in the middle [automotive-c25-req-23.8]
    int result1 = _Generic(x, int: 1, default: 2, float: 3);

    // CHECK-MESSAGES: :[[@LINE+1]]:50: warning: default association in generic selection should appear first or last, not in the middle [automotive-c25-req-23.8]
    int result2 = _Generic(x, long: 10, default: 20, float: 30, double: 40);

    // CHECK-MESSAGES: :[[@LINE+1]]:59: warning: default association in generic selection should appear first or last, not in the middle [automotive-c25-req-23.8]
    int result3 = _Generic(x, char: 5, short: 6, default: 7, long: 8);

    (void)result1;
    (void)result2;
    (void)result3;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_default_first(void) {
    int x = 0;

    // Default appears first - compliant
    int result1 = _Generic(x, default: 0, int: 1, float: 2);

    // Default appears first in longer list - compliant
    int result2 = _Generic(x, default: 100, int: 1, long: 2, float: 3, double: 4);

    (void)result1;
    (void)result2;
}

void test_default_last(void) {
    int x = 0;

    // Default appears last - compliant
    int result1 = _Generic(x, int: 1, float: 2, default: 0);

    // Default appears last in longer list - compliant
    int result2 = _Generic(x, int: 1, long: 2, float: 3, double: 4, default: 100);

    (void)result1;
    (void)result2;
}

void test_no_default(void) {
    int x = 0;

    // No default association - compliant (no violation)
    int result = _Generic(x, int: 1, long: 2, float: 3);

    (void)result;
}

void test_only_default(void) {
    int x = 0;

    // Only default association - compliant (automatically first and last)
    int result = _Generic(x, default: 42);

    (void)result;
}

void test_one_association_plus_default(void) {
    int x = 0;

    // Default first with one other association - compliant
    int result1 = _Generic(x, default: 0, int: 1);

    // Default last with one other association - compliant
    int result2 = _Generic(x, int: 1, default: 0);

    (void)result1;
    (void)result2;
}
