// RUN: %check_clang_tidy %s automotive-c23-req-13.5 %t

// Test: Side effects in logical operand (MISRA Rule 13.5)

// Violation - side effect in && right operand
int test_and_side_effect(int x) {
    int y = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: right operand of && has side effect
    if (x > 0 && (y = x) > 0) {
        return 1;
    }
    return 0;
}

// Violation - side effect in || right operand
int test_or_side_effect(int x) {
    int y = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: right operand of || has side effect
    if (x == 0 || (y = x) > 0) {
        return 1;
    }
    return 0;
}

// Compliant - no side effects in operands
int test_compliant(int x, int y) {
    if (x > 0 && y > 0) {
        return 1;
    }
    return 0;
}
