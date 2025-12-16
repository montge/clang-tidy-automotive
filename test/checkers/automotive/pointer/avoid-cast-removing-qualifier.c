// RUN: %check_clang_tidy %s automotive-c23-req-11.8 %t

// Test: Cast shall not remove const/volatile qualifier (MISRA Rule 11.8)

// Violation - cast removes const qualifier
void test_remove_const(const int *const_ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast removes const qualifier
    int *ptr = (int *)const_ptr;
    (void)ptr;
}

// Violation - cast removes volatile qualifier
void test_remove_volatile(volatile int *volatile_ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast removes volatile qualifier
    int *ptr = (int *)volatile_ptr;
    (void)ptr;
}

// Violation - cast removes both qualifiers
void test_remove_both(const volatile int *cv_ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast removes
    int *ptr = (int *)cv_ptr;
    (void)ptr;
}

// Compliant - adding qualifiers is OK
void test_add_const(int *ptr) {
    const int *const_ptr = (const int *)ptr;
    (void)const_ptr;
}

// Compliant - same qualifiers
void test_same_qualifiers(const int *const_ptr) {
    const int *another_const = (const int *)const_ptr;
    (void)another_const;
}
