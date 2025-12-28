// RUN: %check_clang_tidy %s automotive-c25-req-16.1 %t
// Test for automotive-c25-req-16.1
// Related MISRA C:2025 Rule: 16.1

// This test verifies that switch statements are well-formed:
// 1. Every case ends with break/continue/return/throw
// 2. Every switch has a default clause

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_missing_default(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: switch statement does not have a 'default' clause [automotive-c25-req-16.1]
    switch (x) {
    case 1:
        break;
    case 2:
        break;
    }
}

void test_fallthrough(int x) {
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: case does not end with 'break', 'return', 'continue', or 'throw'; implicit fall-through is not allowed [automotive-c25-req-16.1]
    case 1:
        x = 1;
    case 2:
        x = 2;
        break;
    default:
        break;
    }
}

int test_multiple_fallthrough(int x) {
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: case does not end with 'break', 'return', 'continue', or 'throw'; implicit fall-through is not allowed [automotive-c25-req-16.1]
    case 1:
        x = 1;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: case does not end with 'break', 'return', 'continue', or 'throw'; implicit fall-through is not allowed [automotive-c25-req-16.1]
    case 2:
        x = 2;
    case 3:
        return x;
    default:
        return 0;
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_well_formed_switch(int x) {
    switch (x) {
    case 1:
        x = 1;
        break;
    case 2:
        x = 2;
        break;
    default:
        x = 0;
        break;
    }
}

int test_return_terminates(int x) {
    switch (x) {
    case 1:
        return 1;
    case 2:
        return 2;
    default:
        return 0;
    }
}

void test_empty_case_fallthrough(int x) {
    // Empty cases falling through to next case are OK
    switch (x) {
    case 1:
    case 2:
    case 3:
        x = 3;
        break;
    default:
        break;
    }
}

typedef enum { RED, GREEN, BLUE } Color;

void test_enum_full_coverage(Color c) {
    // Full enum coverage doesn't need default
    switch (c) {
    case RED:
        break;
    case GREEN:
        break;
    case BLUE:
        break;
    }
}

void test_continue_in_loop(int x) {
    for (int i = 0; i < 10; i++) {
        switch (x) {
        case 1:
            continue;  // OK - continue is terminating
        default:
            break;
        }
    }
}

// Test for compound statement requirement
void test_missing_compound_body(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: switch statement body must be a compound statement [automotive-c25-req-16.1]
    switch (x)
        case 1:
            break;
}
