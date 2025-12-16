// Test: Wrong null pointer value (MISRA Rule 11.9)
// Check ID: automotive-wrong-null-pointer-value

#include <stddef.h>

// ============= Assignment Tests =============

// Violation - using 0 instead of NULL in assignment
void test_zero_as_null(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: Use NULL instead of 0 for null pointer assignment.
    int *ptr = 0;
    (void)ptr;
}

// Violation - using 0 in pointer initialization
void test_init_with_zero(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: Use NULL instead of 0 for null pointer assignment.
    char *str1 = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: Use NULL instead of 0 for null pointer assignment.
    void *ptr2 = 0;
    (void)str1;
    (void)ptr2;
}

// Violation - function pointer with 0
void test_function_pointer(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: Use NULL instead of 0 for null pointer assignment.
    void (*func_ptr)(void) = 0;
    (void)func_ptr;
}

// ============= Comparison Tests =============
// Note: Due to implicit cast, comparisons with 0 are reported as assignments

// Violation - comparing pointer with 0
int test_comparison_with_zero(int *ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: Use NULL instead of 0 for null pointer assignment.
    if (ptr == 0) {
        return 1;
    }
    return 0;
}

// Violation - inequality comparison with 0
int test_inequality_comparison(char *str) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: Use NULL instead of 0 for null pointer assignment.
    if (str == 0) {
        return 0;
    }
    return 1;
}

// ============= Ternary Tests =============
// Note: Ternary with 0 also has implicit cast

// Violation - ternary with 0 as true expression
void *test_ternary_true(int cond, void *ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: Use NULL instead of 0 for null pointer assignment.
    return cond ? 0 : ptr;
}

// Violation - ternary with 0 as false expression
void *test_ternary_false(int cond, void *ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: Use NULL instead of 0 for null pointer assignment.
    return cond ? ptr : 0;
}

// ============= Compliant Cases =============

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
int test_comparison_with_null(int *ptr) {
    if (ptr == NULL) {
        return 1;
    }
    return 0;
}

// Compliant - integer 0 (not pointer context)
void test_integer_zero(void) {
    int x = 0;
    (void)x;
}
