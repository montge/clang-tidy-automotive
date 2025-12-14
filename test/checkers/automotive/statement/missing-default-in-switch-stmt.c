// Test file for: automotive-missing-default-in-switch-stmt
// Related MISRA C:2025 Rule: 16.4
//
// This file tests the detection of switch statements without default case

// RUN: %check_clang_tidy %s automotive-missing-default-in-switch-stmt %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_missing_default_violations(void) {
    int x = 5;

    // Switch without default case
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
    switch (x) {
    case 1:
        break;
    case 2:
        break;
    }

    // Switch with multiple cases but no default
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
    switch (x) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    }

    // Empty switch statement (no cases at all)
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
    switch (x) {
    }

    // Switch with only one case, no default
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
    switch (x) {
    case 0:
        break;
    }
}

void test_enum_switch_violations(void) {
    enum Color {
        RED,
        GREEN,
        BLUE
    };

    enum Color color = RED;

    // Enum switch without default (even if all values covered)
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
    switch (color) {
    case RED:
        break;
    case GREEN:
        break;
    case BLUE:
        break;
    }

    // Enum switch with partial coverage, no default
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
    switch (color) {
    case RED:
        break;
    case GREEN:
        break;
    }
}

void test_char_switch_violations(void) {
    char c = 'A';

    // Character switch without default
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
    switch (c) {
    case 'A':
        break;
    case 'B':
        break;
    case 'C':
        break;
    }
}

void test_nested_switch_violations(void) {
    int x = 1;
    int y = 2;

    // Outer switch has default, inner switch doesn't
    switch (x) {
    case 1:
        // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
        switch (y) {
        case 1:
            break;
        case 2:
            break;
        }
        break;
    default:
        break;
    }

    // Outer switch doesn't have default, inner switch has default
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
    switch (x) {
    case 1:
        switch (y) {
        case 1:
            break;
        default:
            break;
        }
        break;
    case 2:
        break;
    }

    // Both switches missing default
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
    switch (x) {
    case 1:
        // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: missing default statement in switch statement [automotive-missing-default-in-switch-stmt]
        switch (y) {
        case 1:
            break;
        }
        break;
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_switches(void) {
    int x = 5;

    // Switch with default case
    switch (x) {
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }

    // Switch with default as first case
    switch (x) {
    default:
        break;
    case 1:
        break;
    case 2:
        break;
    }

    // Switch with default in middle
    switch (x) {
    case 1:
        break;
    default:
        break;
    case 2:
        break;
    }

    // Switch with only default case
    switch (x) {
    default:
        break;
    }

    // Empty default case
    switch (x) {
    case 1:
        break;
    case 2:
        break;
    default:
        // Empty default is OK
        break;
    }
}

void test_enum_switch_with_default(void) {
    enum Status {
        OK,
        ERROR,
        PENDING
    };

    enum Status status = OK;

    // Enum switch with default
    switch (status) {
    case OK:
        break;
    case ERROR:
        break;
    case PENDING:
        break;
    default:
        break;
    }

    // Enum switch with default even when all values covered
    switch (status) {
    case OK:
        break;
    case ERROR:
        break;
    case PENDING:
        break;
    default:
        // Handle unexpected values (defensive programming)
        break;
    }
}

void test_fallthrough_with_default(void) {
    int x = 5;

    // Cases with fallthrough, but has default
    switch (x) {
    case 1:
    case 2:
        break;
    case 3:
    case 4:
        break;
    default:
        break;
    }

    // All cases fall through to default
    switch (x) {
    case 1:
    case 2:
    case 3:
    default:
        break;
    }
}

void test_nested_switch_compliant(void) {
    int x = 1;
    int y = 2;

    // Both switches have default
    switch (x) {
    case 1:
        switch (y) {
        case 1:
            break;
        case 2:
            break;
        default:
            break;
        }
        break;
    case 2:
        break;
    default:
        break;
    }
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_switch_with_compound_statement(void) {
    int x = 5;

    // Switch with compound statements in cases
    switch (x) {
    case 1: {
        int temp = x * 2;
        break;
    }
    case 2: {
        int temp = x * 3;
        break;
    }
    default: {
        int temp = 0;
        break;
    }
    }
}

void test_switch_with_declarations(void) {
    int x = 5;

    // Switch with declarations in default
    switch (x) {
    case 1:
        break;
    case 2:
        break;
    default: {
        int default_value = 100;
        // Use default_value
        break;
    }
    }
}

void test_switch_no_break(void) {
    int x = 5;

    // Switch with default but no breaks (still compliant - break is different rule)
    switch (x) {
    case 1:
        // No break
    case 2:
        // No break
    default:
        // Has default, so compliant for this check
        break;
    }
}

void test_switch_with_return(void) {
    int x = 5;

    // Switch with return instead of break
    switch (x) {
    case 1:
        return;
    case 2:
        return;
    default:
        return;
    }
}

void test_switch_with_goto(void) {
    int x = 5;

    // Switch with goto in default
    switch (x) {
    case 1:
        break;
    default:
        goto exit;
    }

exit:
    return;
}

void test_unsigned_switch(void) {
    unsigned int u = 5;

    // Unsigned value switch with default
    switch (u) {
    case 0:
        break;
    case 1:
        break;
    default:
        break;
    }
}

void test_long_switch(void) {
    long value = 100L;

    // Long value switch with default
    switch (value) {
    case 0L:
        break;
    case 100L:
        break;
    case 200L:
        break;
    default:
        break;
    }
}

void test_expression_in_switch(void) {
    int a = 5;
    int b = 3;

    // Expression as switch condition
    switch (a + b) {
    case 8:
        break;
    case 10:
        break;
    default:
        break;
    }
}

void test_default_with_comment(void) {
    int x = 5;

    // Default with comment explaining why it's empty
    switch (x) {
    case 1:
        break;
    case 2:
        break;
    default:
        // Intentionally empty - all valid values handled
        break;
    }
}
