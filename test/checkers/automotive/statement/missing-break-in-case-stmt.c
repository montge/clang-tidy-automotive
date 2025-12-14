// Test file for: automotive-missing-break-in-case-stmt
// Related MISRA C:2025 Rule: 16.3
//
// This file tests the detection of switch case statements without break

// RUN: %check_clang_tidy %s automotive-missing-break-in-case-stmt %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_missing_break_violations(void) {
    int x = 5;
    int result = 0;

    // Case without break (falls through to next case)
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case-stmt]
    case 1:
        result = 1;
    case 2:
        result = 2;
        break;
    default:
        result = 0;
        break;
    }

    // Multiple cases without break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case-stmt]
    case 1:
        result = 1;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case-stmt]
    case 2:
        result = 2;
    case 3:
        result = 3;
        break;
    default:
        break;
    }

    // Default case without break
    switch (x) {
    case 1:
        result = 1;
        break;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case-stmt]
    default:
        result = 0;
    }

    // Case with statement but no break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case-stmt]
    case 1:
        result = x * 2;
    case 2:
        break;
    default:
        break;
    }
}

void test_complex_missing_break(void) {
    int x = 5;
    int y = 0;

    // Case with multiple statements but no break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case-stmt]
    case 1:
        y = x + 1;
        y = y * 2;
        y = y - 3;
    case 2:
        y = 10;
        break;
    default:
        break;
    }

    // Case with if statement but no break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case-stmt]
    case 1:
        if (x > 0) {
            y = 1;
        }
    case 2:
        y = 2;
        break;
    default:
        break;
    }

    // Case with loop but no break
    switch (x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: every non-empty switch case must end with a break statement [automotive-missing-break-in-case-stmt]
    case 1:
        for (int i = 0; i < 5; i++) {
            y += i;
        }
    case 2:
        break;
    default:
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
    default:
        break;
    }
}

void test_return_instead_of_break(void) {
    int x = 5;

    // Cases with return are OK (terminates execution)
    // Note: This might still trigger if the check doesn't recognize return
    // as a terminator, but it's a valid pattern
    switch (x) {
    case 1:
        return;
    case 2:
        return;
    default:
        return;
    }
}

void test_compound_statements(void) {
    int x = 5;
    int result = 0;

    // Cases with compound statements and break
    switch (x) {
    case 1: {
        int temp = x * 2;
        result = temp;
        break;
    }
    case 2: {
        int temp = x * 3;
        result = temp;
        break;
    }
    default: {
        result = 0;
        break;
    }
    }
}

void test_nested_switch(void) {
    int x = 5;
    int y = 3;
    int result = 0;

    // Nested switch with all breaks
    switch (x) {
    case 1:
        switch (y) {
        case 1:
            result = 11;
            break;
        case 2:
            result = 12;
            break;
        default:
            break;
        }
        break;
    case 2:
        result = 2;
        break;
    default:
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

void test_goto_in_case(void) {
    int x = 5;
    int result = 0;

    // Case with goto instead of break
    // This may or may not be compliant depending on implementation
    switch (x) {
    case 1:
        result = 1;
        goto exit;
    case 2:
        result = 2;
        break;
    default:
        break;
    }

exit:
    return;
}

void test_continue_in_case_with_loop(void) {
    int x = 5;

    // Switch inside a loop
    for (int i = 0; i < 10; i++) {
        switch (x) {
        case 1:
            // continue affects the loop, not the switch
            // This case still needs a break
            if (i > 5) {
                continue;
            }
            break;
        case 2:
            break;
        default:
            break;
        }
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

void test_case_with_label(void) {
    int x = 5;
    int result = 0;

    // Case with label and break
    switch (x) {
    case 1:
label1:
        result = 1;
        break;
    case 2:
        result = 2;
        break;
    default:
        break;
    }

    if (result == 0) {
        goto label1;
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
