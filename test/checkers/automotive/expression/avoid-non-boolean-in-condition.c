// Test file for: automotive-c23-req-14.4
// Related MISRA C:2025 Rule: 14.4
//
// This file tests the detection of non-boolean expressions in conditions

// RUN: %check_clang_tidy %s automotive-c23-req-14.4 %t

#include <stdbool.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_non_boolean_violations(void) {
    int x = 5;
    int *ptr = &x;

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    if (x) {  // Integer in condition
        // Do something
    }

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    if (ptr) {  // Pointer in condition  codeql[cpp/redundant-null-check-simple]
        // Do something
    }

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    while (x) {  // Integer in loop condition
        x--;
    }

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    for (; x; x--) {  // Integer in for condition
        // Do something
    }
}

int get_status(void);

void test_function_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    if (get_status()) {  // Non-boolean return value
        // Do something
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    int x = 5;
    int *ptr = &x;
    bool flag = true;

    // Boolean variable
    if (flag) {
        // Do something
    }

    // Explicit comparison
    if (x != 0) {
        // Do something
    }

    // Pointer comparison with NULL
    if (ptr != 0) {
        // Do something
    }

    // Boolean expression
    if (x > 0 && x < 10) {
        // Do something
    }

    // Explicit comparison in loop
    while (x != 0) {
        x--;
    }

    // Logical operators produce boolean
    if (!flag) {
        // Do something
    }
}
