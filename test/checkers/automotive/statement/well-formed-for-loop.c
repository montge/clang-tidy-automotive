// RUN: %check_clang_tidy %s automotive-c25-req-14.2 %t

// Test: Well-formed for loop (MISRA Rule 14.2)
// Check ID: automotive-c25-req-14.2

// ============= Compliant: Standard well-formed for loop =============

void test_compliant_basic(void) {
    for (int i = 0; i < 10; i++) {
        // OK - well-formed for loop
    }
}

void test_compliant_decrement(void) {
    for (int i = 10; i > 0; i--) {
        // OK - decrement is valid modification
    }
}

void test_compliant_compound_assign(void) {
    for (int i = 0; i < 100; i += 2) {
        // OK - compound assignment is valid modification
    }
}

void test_compliant_multiple_counters(void) {
    for (int i = 0, j = 10; i < j; i++, j--) {
        // OK - multiple counters, both modified in increment
    }
}

// ============= Violations: Missing initialization =============

void test_missing_init(int i) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: for loop is missing initialization
    for (; i < 10; i++) {
        // Missing init statement
    }
}

// ============= Violations: Missing condition =============

void test_missing_condition(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: for loop is missing condition
    for (int i = 0; ; i++) {
        if (i >= 10) break;
    }
}

// ============= Violations: Missing increment =============

void test_missing_increment(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: for loop is missing increment expression
    for (int i = 0; i < 10; ) {
        i++;  // Counter modified in body instead
    }
}

// ============= Violations: Counter not in condition =============

void test_counter_not_in_condition(int limit) {
    // CHECK-MESSAGES: :[[@LINE+1]]:27: warning: for loop condition does not reference the loop counter
    for (int i = 0; limit < 10; i++) {
    }
}

// ============= Violations: Counter not modified in increment =============

void test_counter_not_modified_in_increment(int j) {
    // CHECK-MESSAGES: :[[@LINE+1]]:30: warning: for loop increment expression does not modify the loop counter
    for (int i = 0; i < 10; j++) {
    }
}

// ============= Violations: Counter modified in body =============

void test_counter_modified_in_body_increment(void) {
    for (int i = 0; i < 10; i++) {
        // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: loop counter modified in for loop body
        i++;
    }
}

void test_counter_modified_in_body_assign(void) {
    for (int i = 0; i < 10; i++) {
        // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: loop counter modified in for loop body
        i = 5;
    }
}

void test_counter_modified_in_body_compound_assign(void) {
    for (int i = 0; i < 100; i += 1) {
        // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: loop counter modified in for loop body
        i += 5;
    }
}

void test_counter_modified_in_body_decrement(void) {
    for (int i = 0; i < 10; i++) {
        // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: loop counter modified in for loop body
        i--;
    }
}

// ============= Compliant: Nested loops are independent =============

void test_nested_loops_independent(void) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            // Both loops are well-formed and independent
        }
    }
}

// ============= Compliant: Reading counter in body is allowed =============

void test_read_counter_in_body(void) {
    int arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = i * 2;  // OK - reading counter is allowed
    }
}

// ============= Compliant: Other variables can be modified =============

void test_other_var_modified_in_body(void) {
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += i;  // OK - modifying non-counter variables is allowed
    }
}

// ============= Edge case: Pointer increment =============

void test_pointer_loop(int *arr, int n) {
    for (int *p = arr; p < arr + n; p++) {
        // Well-formed pointer-based for loop
    }
}
