// RUN: %check_clang_tidy %s automotive-c23-req-14.3 %t

// Test: Invariant control expressions (MISRA Rule 14.3)

// Violation - constant controlling expression
void test_constant_if(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: controlling expression is invariant
    if (1) {
        // always executed
    }
}

// Violation - while(true) without break
void test_constant_while(void) {
    int count = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: controlling expression is invariant
    while (1) {
        count++;
        if (count > 10) break;
    }
}

// Compliant - variable controlling expression
void test_variable_if(int x) {
    if (x > 0) {
        // conditionally executed
    }
}

// Compliant - variable controlling while
void test_variable_while(int *arr, int len) {
    int i = 0;
    while (i < len) {
        arr[i] = 0;
        i++;
    }
}
