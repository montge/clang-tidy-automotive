// RUN: %check_clang_tidy %s automotive-cpp23-req-9.5 %t -- -- -Wno-error
// Test for automotive-cpp23-req-9.5
// Related MISRA C++:2023 Rule: 9.5

// This test verifies that nested switch labels are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_case_in_if_block(int x, bool y) {
    switch (x) {
        case 1:
            break;
        case 2:
            if (y) {
                // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: case label is nested inside a compound statement
                case 3:
                    break;
            }
            break;
    }
}

void test_default_in_if_block(int x, bool y) {
    switch (x) {
        case 1:
            break;
        case 2:
            if (y) {
                // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: default label is nested inside a compound statement
                default:
                    break;
            }
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_normal_switch(int x) {
    switch (x) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
}

void test_switch_with_braced_cases(int x) {
    // The braces after case labels are allowed - they create compound
    // statements for the case body, but the case labels themselves are
    // at the outermost level
    switch (x) {
        case 1: {
            int a = 1;
            (void)a;
            break;
        }
        case 2: {
            int b = 2;
            (void)b;
            break;
        }
        default: {
            int c = 3;
            (void)c;
            break;
        }
    }
}

void test_nested_switch(int x, int y) {
    // Nested switches are fine - each case belongs to its own switch
    switch (x) {
        case 1:
            switch (y) {
                case 10:
                    break;
                case 20:
                    break;
            }
            break;
        case 2:
            break;
    }
}
