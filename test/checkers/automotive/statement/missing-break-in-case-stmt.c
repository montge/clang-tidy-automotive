// Test file for: automotive-missing-break-in-case
// Related MISRA C:2025 Rule: 16.3
//
// This file tests the detection of switch case statements without break

// RUN: %check_clang_tidy %s automotive-missing-break-in-case %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_missing_break_violations(void) {
    int x = 5;
    int result = 0;

    // Case without break (falls through to next case)
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case]
    case 1:
        result = 1;
    case 2:
        result = 2;
        break;
    }

    // Multiple cases without break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case]
    case 1:
        result = 1;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case]
    case 2:
        result = 2;
    case 3:
        result = 3;
        break;
    }

    // Default case without break
    switch (x) {
    case 1:
        result = 1;
        break;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case]
    default:
        result = 0;
    }

    // Case with statement but no break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case]
    case 1:
        result = x * 2;
    case 2:  // Empty case with only break - compliant
        break;
    }
}

void test_complex_missing_break(void) {
    int x = 5;
    int y = 0;

    // Case with multiple statements but no break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case]
    case 1:
        y = x + 1;
        y = y * 2;
        y = y - 3;
    case 2:
        y = 10;
        break;
    }

    // Case with if statement but no break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case]
    case 1:
        if (x > 0) {
            y = 1;
        }
    case 2:
        y = 2;
        break;
    }

    // Case with loop but no break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case]
    case 1:
        for (int i = 0; i < 5; i++) {
            y += i;
        }
    case 2:  // Empty case with only break - compliant
        break;
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_cases(void) {
    int x = 5;
    int result = 0;

    // All cases have break
    switch (x) {
    case 1:
        result = 1;
        break;
    case 2:
        result = 2;
        break;
    case 3:
        result = 3;
        break;
    default:
        result = 0;
        break;
    }

    // Empty case labels (allowed to fall through)
    switch (x) {
    case 1:
    case 2:
    case 3:
        result = 123;
        break;
    default:
        result = 0;
        break;
    }

    // All empty cases falling to default
    switch (x) {
    case 1:
    case 2:
    case 3:
    default:
        result = 999;
        break;
    }

    // Single case with break
    switch (x) {
    case 1:
        result = 1;
        break;
    default:  // Empty default with only break - compliant
        break;
    }
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_empty_switch(void) {
    int x = 5;

    // Empty switch - no cases, so no violations
    switch (x) {
    }
}

void test_only_default(void) {
    int x = 5;
    int result = 0;

    // Only default case with break
    switch (x) {
    default:
        result = 0;
        break;
    }
}

void test_multiple_empty_cases(void) {
    int x = 5;
    int result = 0;

    // Multiple empty cases (all allowed to fall through)
    switch (x) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        result = 12345;
        break;
    default:
        result = 0;
        break;
    }
}

void test_enum_switch(void) {
    enum Status {
        OK,
        ERROR,
        PENDING
    };

    enum Status status = OK;
    int result = 0;

    // Enum switch with all breaks
    switch (status) {
    case OK:
        result = 1;
        break;
    case ERROR:
        result = -1;
        break;
    case PENDING:
        result = 0;
        break;
    }
}

void test_char_switch(void) {
    char c = 'A';
    int result = 0;

    // Character switch with breaks
    switch (c) {
    case 'A':
        result = 1;
        break;
    case 'B':
        result = 2;
        break;
    case 'C':
        result = 3;
        break;
    default:
        result = 0;
        break;
    }
}

void test_expression_in_case(void) {
    int x = 5;
    int result = 0;

    // Case with complex expressions
    switch (x) {
    case 1:
        result = (x > 0) ? x : -x;
        break;
    case 2:
        result = x * x;
        break;
    default:
        result = 0;
        break;
    }
}
