// Test file for: automotive expression checks (Edge Cases)
// Related MISRA C:2025 Rules: 13.4, 14.3, 14.4
//
// This file tests edge cases for expression-related checks

// RUN: %check_clang_tidy %s automotive-c23-adv-13.4,automotive-c23-req-14.4 %t

#include <stdbool.h>
#include <stddef.h>

//===----------------------------------------------------------------------===//
// Edge Case: Assignment in complex expressions (13.4)
//===----------------------------------------------------------------------===//

void test_assignment_edge_cases(int *ptr, int x, int y) {
    int a, b, c;

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: Avoid using the result of an assignment operator '=' [automotive-c23-adv-13.4]
    if ((a = x) != 0) { }  // Assignment in condition

    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: Avoid using the result of an assignment operator '=' [automotive-c23-adv-13.4]
    while ((b = *ptr++) != 0) { }  // Assignment in while

    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: Avoid using the result of an assignment operator '=' [automotive-c23-adv-13.4]
    for (c = 0; (c = x) < 10; c++) { }  // Assignment in for condition

    // Compliant: Assignment as statement
    a = x;
    if (a != 0) { }
}

//===----------------------------------------------------------------------===//
// Edge Case: Non-boolean in condition (14.4)
//===----------------------------------------------------------------------===//

void test_non_boolean_edge_cases(int x, int *ptr, void *vptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    if (x) { }  // Integer in condition

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    if (ptr) { }  // Pointer in condition

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    while (x) { }  // Integer in while

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    while (vptr) { }  // void pointer in condition

    // Compliant: Explicit boolean comparison
    if (x != 0) { }
    if (ptr != NULL) { }
    while (x > 0) { }

    // Compliant: Boolean type
    bool flag = true;
    if (flag) { }
    while (flag) { }
}

//===----------------------------------------------------------------------===//
// Edge Case: Ternary operator with non-boolean
//===----------------------------------------------------------------------===//

void test_ternary_non_boolean(int x, int *ptr) {
    int result;

    // Ternary operator conditions now detected by this check
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    result = x ? 1 : 0;  // Integer as ternary condition
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    result = ptr ? 1 : 0;  // Pointer as ternary condition

    // Compliant: Explicit comparison
    result = (x != 0) ? 1 : 0;
    result = (ptr != NULL) ? 1 : 0;
}

//===----------------------------------------------------------------------===//
// Edge Case: Logical operators with non-boolean operands
//===----------------------------------------------------------------------===//

void test_logical_operators(int x, int y) {
    bool result;

    // Logical operators with integers are not currently detected by this check
    result = x && y;  // Integers with logical AND
    result = x || y;  // Integers with logical OR
    result = !x;  // Integer with logical NOT

    // Compliant: Boolean operands
    bool a = true, b = false;
    result = a && b;
    result = a || b;
    result = !a;
}

//===----------------------------------------------------------------------===//
// Edge Case: Function returning int used as boolean
//===----------------------------------------------------------------------===//

int get_status(void);
bool is_ready(void);

void test_function_return(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid using non-boolean expression in control flow condition [automotive-c23-req-14.4]
    if (get_status()) { }  // int return used as boolean

    // Compliant: bool return
    if (is_ready()) { }

    // Compliant: Explicit comparison
    if (get_status() != 0) { }
}

//===----------------------------------------------------------------------===//
// Compliant Cases
//===----------------------------------------------------------------------===//

void test_compliant_expressions(void) {
    bool flag = true;
    int x = 5;
    int *ptr = &x;

    // Explicit boolean conditions
    if (flag) { }
    if (x == 5) { }
    if (x > 0) { }
    if (ptr != NULL) { }  // codeql[cpp/redundant-null-check-simple] Intentional test case

    // Assignment as separate statement
    int y;
    y = x;
    if (y != 0) { }

    // Boolean expressions
    bool combined = flag && (x > 0);
    if (combined) { }
}
