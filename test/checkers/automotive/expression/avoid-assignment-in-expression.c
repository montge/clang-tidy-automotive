// Test file for: automotive-c23-adv-13.4
// Related MISRA C:2025 Rule: 13.4
//
// This file tests the detection of assignment used as expression value

// RUN: %check_clang_tidy %s automotive-c23-adv-13.4 %t

// Helper function declaration
int get_value(void);

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_assignment_violations(void) {
    int x, y, z;

    // Suppress clang diagnostic for assignment in condition
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wparentheses"
    if (x = 5) {  // Assignment in condition
        y = 1;
    }

    while (y = get_value()) {  // Assignment in loop condition
        // Process
    }
    #pragma clang diagnostic pop

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: Avoid using the result of an assignment operator '=' [automotive-c23-adv-13.4]
    z = (x = 10) + 5;  // Assignment result used in expression

    // CHECK-MESSAGES: :[[@LINE+2]]:11: warning: Avoid using the result of an assignment operator '=' [automotive-c23-adv-13.4]
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: Avoid using the result of an assignment operator '=' [automotive-c23-adv-13.4]
    x = y = z = 0;  // Chained assignment
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    int x, y, z;

    // Separate assignment and condition
    x = 5;
    if (x != 0) {
        y = 1;
    }

    // Separate assignment and loop
    y = get_value();
    while (y != 0) {
        // Process
        y = get_value();
    }

    // Simple assignment (not used as expression)
    z = 10;
    x = z + 5;

    // Individual assignments
    x = 0;
    y = 0;
    z = 0;
}
