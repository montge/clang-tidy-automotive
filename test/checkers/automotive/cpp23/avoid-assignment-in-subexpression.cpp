// RUN: %check_clang_tidy %s automotive-cpp23-req-6.4 %t

// Test: Assignment in sub-expression (MISRA C++:2023 Rule 6.4)
// Check ID: automotive-cpp23-req-6.4

// ============= Compliant: Assignment as statement =============

void test_compliant_basic() {
    int x;
    x = 5;  // OK - assignment is the entire statement
}

void test_compliant_multiple() {
    int x, y;
    x = 5;
    y = 10;  // OK - separate assignment statements
}

void test_compliant_in_for_init() {
    for (int i = 0; i < 10; i++) {
        // OK - assignment in for init is allowed
    }
}

void test_compliant_in_for_init_assign() {
    int i;
    for (i = 0; i < 10; i++) {
        // OK - assignment in for init is allowed
    }
}

void test_compliant_in_for_increment() {
    int i;
    int j;
    for (i = 0; i < 10; i = i + 1) {
        // OK - assignment in for increment is allowed
    }
}

// ============= Violations: Assignment in if condition =============

void test_if_condition(int getValue()) {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: assignment operator used as sub-expression
    if ((x = getValue())) {
    }
}

void test_if_condition_comparison(int getValue()) {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: assignment operator used as sub-expression
    if ((x = getValue()) != 0) {
    }
}

// ============= Violations: Assignment in while condition =============

void test_while_condition(int getValue()) {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: assignment operator used as sub-expression
    while ((x = getValue()) != 0) {
    }
}

// ============= Violations: Assignment in for condition =============

void test_for_condition_assignment() {
    int i;
    int j;
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: assignment operator used as sub-expression
    for (i = 0; j = i, i < 10; i++) {
    }
}

// ============= Violations: Assignment in binary expression =============

void test_binary_expression() {
    int x, y;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: assignment operator used as sub-expression
    y = (x = 5) + 1;
}

void test_logical_expression() {
    int x;
    bool result;
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: assignment operator used as sub-expression
    result = (x = 5) && true;
}

// ============= Violations: Assignment in ternary condition =============

void test_ternary_condition() {
    int x;
    int y;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: assignment operator used as sub-expression
    y = (x = 1) ? 10 : 20;
}

// ============= Violations: Assignment in return =============

int test_return_assignment() {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: assignment operator used as sub-expression
    return (x = 5);
}

// ============= Compliant: Chained assignment =============

void test_chained_assignment() {
    int x, y;
    x = y = 5;  // This is chained assignment, not sub-expression - might be flagged
}

// ============= Compliant: Comma operator in expression statement =============

void test_comma_expression() {
    int x, y;
    x = 1, y = 2;  // Comma operator at statement level - might be OK depending on interpretation
}
