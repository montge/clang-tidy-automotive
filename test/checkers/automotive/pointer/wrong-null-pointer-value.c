// RUN: %check_clang_tidy %s automotive-c23-req-11.9 %t

// Test: Wrong null pointer value (MISRA Rule 11.9)

#include <stddef.h>

// Violation - using 0 instead of NULL
void test_zero_as_null(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use NULL or nullptr
    int *ptr = 0;
    (void)ptr;
}

// Violation - using integer literal for null
void test_int_literal(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: use NULL or nullptr
    char *str = (char *)0;
    (void)str;
}

// Compliant - using NULL macro
void test_null_macro(void) {
    int *ptr = NULL;
    (void)ptr;
}

// Compliant - valid pointer assignment
void test_valid_assignment(void) {
    int x = 42;
    int *ptr = &x;
    (void)ptr;
}

// Compliant - comparison with NULL
int test_comparison(int *ptr) {
    if (ptr == NULL) {
        return 1;
    }
    return 0;
}
