// RUN: %check_clang_tidy %s automotive-cpp23-req-5.7 %t -- -- -std=c++11

// Test: Variable scope minimization (MISRA C++:2023 Rule 5.7)
// Check ID: automotive-cpp23-req-5.7

// ============= Compliant: Variables declared in minimal scope =============

void compliant_minimal_scope() {
    if (true) {
        int x = 10;  // OK - declared in minimal scope
        x++;
    }
}

void compliant_used_throughout() {
    int x = 10;  // OK - used in multiple blocks at same level
    if (true) {
        x++;
    }
    if (false) {
        x--;
    }
}

void compliant_multiple_uses() {
    int counter = 0;  // OK - used before and inside block
    counter++;
    if (counter > 0) {
        counter += 5;
    }
}

// ============= Violations: Variables in wider scope than needed =============

void violation_single_block_usage() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: variable 'x' is declared in a wider scope than necessary
    int x = 10;

    if (true) {
        x++;  // Only used in this block
    }
}

void violation_nested_block() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: variable 'y' is declared in a wider scope than necessary
    int y = 20;

    {
        y += 5;  // Only used in nested block
    }
}

void violation_if_else_single_branch() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: variable 'z' is declared in a wider scope than necessary
    int z = 30;

    if (true) {
        z *= 2;  // Only used in if branch
    } else {
        // z not used here
    }
}

void violation_loop_only() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: variable 'i' is declared in a wider scope than necessary
    int i = 0;

    for (; i < 10; i++) {
        // i only used in loop
    }
}

// ============= Complex cases =============

void complex_nested_blocks() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: variable 'deep' is declared in a wider scope than necessary
    int deep = 100;

    if (true) {
        if (true) {
            deep++;  // Used only in deeply nested block
        }
    }
}

void multiple_variables() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: variable 'a' is declared in a wider scope than necessary
    int a = 1;
    int b = 2;  // OK - used at function scope

    b++;

    if (true) {
        a++;  // a only used here
    }
}

// ============= Edge cases =============

void unused_variable() {
    int unused = 42;  // OK - let unused-variable check handle this
}

void parameter_usage(int param) {
    // Parameters are exempt from this check
    if (true) {
        param++;
    }
}

void static_variable() {
    static int s = 0;  // Static variables are exempt
    if (true) {
        s++;
    }
}

class TestClass {
    int member;

    void member_function() {
        // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: variable 'local' is declared in a wider scope than necessary
        int local = 10;

        if (member > 0) {
            local++;  // Only used in this block
        }
    }
};

// ============= Loop examples =============

void while_loop_scope() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: variable 'n' is declared in a wider scope than necessary
    int n = 0;

    while (n < 5) {
        n++;
    }
}

void for_loop_correct() {
    // This is correct - loop variable properly scoped
    for (int i = 0; i < 10; i++) {
        // OK
    }
}

// ============= Switch statement cases =============

void switch_case_scope() {
    int selector = 1;  // OK - used for switch

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: variable 'result' is declared in a wider scope than necessary
    int result = 0;

    switch (selector) {
        case 1:
            result = 10;
            break;
        default:
            result = 20;
            break;
    }
}

// ============= Lambda captures =============

void lambda_example() {
    int outer = 5;  // OK - captured by lambda

    auto f = [&outer]() {
        outer++;
    };

    f();
}

// ============= Try-catch blocks =============

void exception_handling() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: variable 'error_code' is declared in a wider scope than necessary
    int error_code = 0;

    try {
        error_code = 42;
    } catch (...) {
        // error_code only used in try block
    }
}
