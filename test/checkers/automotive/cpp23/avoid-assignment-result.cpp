// RUN: %check_clang_tidy %s automotive-cpp23-req-7.4 %t

// Test: The result of an assignment shall not be used (MISRA C++:2023 Rule 7.4)
// Check ID: automotive-cpp23-req-7.4

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

// ============= Compliant: Chained assignment =============

void test_chained_assignment() {
    int x, y, z;
    x = y = z = 5;  // OK - chained assignment is allowed
}

void test_chained_assignment_two() {
    int x, y;
    x = y = 10;  // OK - chained assignment is allowed
}

// ============= Violations: Assignment in if condition =============

void test_if_condition(int getValue()) {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: the result of an assignment shall not be used
    if ((x = getValue())) {
    }
}

void test_if_condition_comparison(int getValue()) {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: the result of an assignment shall not be used
    if ((x = getValue()) != 0) {
    }
}

// ============= Violations: Assignment in while condition =============

void test_while_condition(int getValue()) {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: the result of an assignment shall not be used
    while ((x = getValue()) != 0) {
    }
}

void test_do_while_condition(int getValue()) {
    int x;
    do {
        // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: the result of an assignment shall not be used
    } while ((x = getValue()) != 0);
}

// ============= Violations: Assignment in for condition =============

void test_for_condition_assignment() {
    int i;
    int j;
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: the result of an assignment shall not be used
    for (i = 0; j = i, i < 10; i++) {
    }
}

// ============= Violations: Assignment in binary expression =============

void test_binary_expression() {
    int x, y;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: the result of an assignment shall not be used
    y = (x = 5) + 1;
}

void test_logical_expression() {
    int x;
    bool result;
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: the result of an assignment shall not be used
    result = (x = 5) && true;
}

void test_arithmetic_expression() {
    int x, y, z;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: the result of an assignment shall not be used
    z = (x = y) * 2;
}

// ============= Violations: Assignment in ternary condition =============

void test_ternary_condition() {
    int x;
    int y;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: the result of an assignment shall not be used
    y = (x = 1) ? 10 : 20;
}

// ============= Violations: Assignment in return =============

int test_return_assignment() {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: the result of an assignment shall not be used
    return (x = 5);
}

// ============= Violations: Assignment in function call =============

void someFunction(int value);

void test_function_call_argument() {
    int x;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: the result of an assignment shall not be used
    someFunction(x = 42);
}

// ============= Violations: Assignment in array subscript =============

void test_array_subscript() {
    int arr[10];
    int x, y;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: the result of an assignment shall not be used
    arr[(x = y)] = 5;
}

// ============= Compliant: For loop init and increment =============

void test_compliant_for_init() {
    for (int i = 0; i < 10; i++) {
        // OK - assignment in for init is allowed
    }
}

void test_compliant_for_init_assign() {
    int i;
    for (i = 0; i < 10; i++) {
        // OK - assignment in for init is allowed
    }
}

void test_compliant_for_increment() {
    int i;
    for (i = 0; i < 10; i = i + 1) {
        // OK - assignment in for increment is allowed
    }
}

// ============= Violations: Assignment in compound assignment context =============

void test_compound_non_assignment() {
    int x, y, z;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: the result of an assignment shall not be used
    z += (x = y);
}

// ============= Violations: Assignment in member access =============

struct S {
    int value;
};

void test_member_access() {
    S s;
    int x = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: the result of an assignment shall not be used
    s.value = (x = 5) + 1;
}

// ============= Compliant: Chained assignment with extra parentheses =============

void test_parentheses() {
    int x, y;
    y = ((x = 5));  // OK - This is effectively a chained assignment
}
