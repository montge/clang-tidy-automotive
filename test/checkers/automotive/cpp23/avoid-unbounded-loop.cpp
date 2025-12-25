// RUN: %check_clang_tidy %s automotive-cpp23-req-6.3 %t

// Test: Loop counters should have well-defined bounds (MISRA C++:2023 Rule 6.3)
// Check ID: automotive-cpp23-req-6.3

// ============= Compliant: Well-defined loop bounds =============

void test_compliant_for_loop() {
    for (int i = 0; i < 10; i++) {
        // OK - clear termination condition
    }
}

void test_compliant_while_loop() {
    int count = 0;
    while (count < 10) {
        count++;
        // OK - condition depends on variable that changes
    }
}

void test_compliant_do_while() {
    int x = 0;
    do {
        x++;
    } while (x < 5);
    // OK - condition depends on variable that changes
}

void test_compliant_for_with_condition() {
    int max = 100;
    for (int i = 0; i < max; i++) {
        // OK - bounded by variable
    }
}

void test_compliant_while_with_break() {
    int counter = 0;
    while (counter < 1000) {
        counter++;
        if (counter > 100) {
            break;
        }
    }
    // OK - has termination condition
}

// ============= Violations: For loop with no condition =============

void test_for_no_condition() {
    int count = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: loop has no condition; ensure loop has well-defined termination bounds
    for (;;) {
        count++;
        if (count > 10) break;
    }
}

void test_for_no_condition_infinite() {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: loop has no condition; ensure loop has well-defined termination bounds
    for (;;) {
        // Infinite loop
    }
}

// ============= Violations: While loop with constant true =============

void test_while_true_literal() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: while loop has constant true condition; ensure loop has well-defined termination bounds
    while (true) {
        // Unbounded loop
    }
}

void test_while_one() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: while loop has constant true condition; ensure loop has well-defined termination bounds
    while (1) {
        // Unbounded loop with integer literal
    }
}

void test_while_true_with_break() {
    int x = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: while loop has constant true condition; ensure loop has well-defined termination bounds
    while (true) {
        x++;
        if (x > 10) break;
        // Even with break, constant true condition is flagged
    }
}

void test_while_constant_expr() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: while loop has constant true condition; ensure loop has well-defined termination bounds
    while (2 + 2) {
        // Constant expression that evaluates to true
    }
}

// ============= Violations: Do-while loop with constant true =============

void test_do_while_true() {
    // CHECK-MESSAGES: :[[@LINE+3]]:14: warning: do-while loop has constant true condition; ensure loop has well-defined termination bounds
    do {
        // Unbounded loop
    } while (true);
}

void test_do_while_one() {
    // CHECK-MESSAGES: :[[@LINE+3]]:14: warning: do-while loop has constant true condition; ensure loop has well-defined termination bounds
    do {
        // Unbounded loop
    } while (1);
}

void test_do_while_constant() {
    int counter = 0;
    // CHECK-MESSAGES: :[[@LINE+4]]:14: warning: do-while loop has constant true condition; ensure loop has well-defined termination bounds
    do {
        counter++;
        if (counter > 5) break;
    } while (3 > 2);
}

// ============= Violations: For loop with constant true condition =============

void test_for_true_condition() {
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: for loop has constant true condition; ensure loop has well-defined termination bounds
    for (int i = 0; true; i++) {
        if (i > 10) break;
    }
}

void test_for_one_condition() {
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: for loop has constant true condition; ensure loop has well-defined termination bounds
    for (int i = 0; 1; i++) {
        if (i > 10) break;
    }
}

void test_for_constant_expr_condition() {
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: for loop has constant true condition; ensure loop has well-defined termination bounds
    for (int i = 0; 5 > 3; i++) {
        // Constant expression
    }
}

// ============= Compliant: False conditions (dead code, but not unbounded) =============

void test_while_false() {
    while (false) {
        // Dead code, but not an unbounded loop issue
        // This would be caught by dead code checks
    }
}

void test_for_false() {
    for (int i = 0; false; i++) {
        // Dead code
    }
}

// ============= Compliant: Non-constant expressions =============

void test_while_variable(bool condition) {
    while (condition) {
        // OK - condition is a variable
    }
}

void test_while_function_call(int getValue()) {
    while (getValue() < 100) {
        // OK - condition involves function call
    }
}

void test_for_variable_condition(bool running) {
    for (int i = 0; running; i++) {
        // OK - condition is a variable
    }
}

// ============= Compliant: Template-dependent conditions =============

template<typename T>
void test_template_loop(T value) {
    while (value) {
        // OK - value-dependent expression
    }
}

template<bool B>
void test_template_constant_loop() {
    while (B) {
        // OK - template parameter dependent
    }
}

// ============= Edge cases =============

void test_nested_loops() {
    for (int i = 0; i < 10; i++) {
        // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: while loop has constant true condition; ensure loop has well-defined termination bounds
        while (true) {
            if (i > 5) break;
        }
    }
}

void test_parenthesized_condition() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: while loop has constant true condition; ensure loop has well-defined termination bounds
    while ((true)) {
        // Parenthesized constant
    }
}

void test_compound_constant() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: while loop has constant true condition; ensure loop has well-defined termination bounds
    while (1 && 1) {
        // Compound constant expression
    }
}
