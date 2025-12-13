// Test file for: automotive-c23-adv-13.4
// Related MISRA C:2025 Rule: 13.4
//
// This file tests the detection of assignment used as expression value

// RUN: %check_clang_tidy %s automotive-c23-adv-13.4 %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_assignment_violations(void) {
    int x, y, z;

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: assignment in expression
    if (x = 5) {  // Assignment in condition
        y = 1;
    }

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: assignment in expression
    while (y = get_value()) {  // Assignment in loop condition
        // Process
    }

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: assignment in expression
    z = (x = 10) + 5;  // Assignment result used in expression

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: assignment in expression
    x = y = z = 0;  // Chained assignment
}

// Helper function
int get_value(void);

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
