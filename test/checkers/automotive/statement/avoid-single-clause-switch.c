// RUN: %check_clang_tidy %s automotive-c23-req-16.6 %t
// Test for automotive-c23-req-16.6
// Related MISRA C:2025 Rule: 16.6

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_single_case(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: switch statement shall have at least two switch-clauses [automotive-c23-req-16.6]
    switch (x) {
        case 1:
            break;
    }
}

void test_only_default(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: switch statement shall have at least two switch-clauses [automotive-c23-req-16.6]
    switch (x) {
        default:
            break;
    }
}

void test_empty_switch(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: switch statement shall have at least two switch-clauses [automotive-c23-req-16.6]
    switch (x) {
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_two_cases(int x) {
    switch (x) {
        case 1:
            break;
        case 2:
            break;
    }
}

void test_case_and_default(int x) {
    switch (x) {
        case 1:
            break;
        default:
            break;
    }
}

void test_multiple_cases(int x) {
    switch (x) {
        case 1:
        case 2:
        case 3:
            break;
        default:
            break;
    }
}
