// RUN: %check_clang_tidy %s automotive-c25-req-11.8 %t

// Test: Cast shall not remove const/volatile qualifier (MISRA C:2025 Rule 11.8)

// Violation - cast removes const qualifier
void test_remove_const(const int *const_ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast removes 'const' qualifier from pointer type [automotive-c25-req-11.8]
    int *ptr = (int *)const_ptr;
    (void)ptr;
}

// Violation - cast removes volatile qualifier
void test_remove_volatile(volatile int *volatile_ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast removes 'volatile' qualifier from pointer type [automotive-c25-req-11.8]
    int *ptr = (int *)volatile_ptr;
    (void)ptr;
}

// Violation - cast removes both qualifiers
void test_remove_both(const volatile int *cv_ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast removes 'const' and 'volatile' qualifiers from pointer type [automotive-c25-req-11.8]
    int *ptr = (int *)cv_ptr;
    (void)ptr;
}

// Violation - cast removes volatile from const volatile
void test_remove_volatile_only(const volatile int *cv_ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: cast removes 'volatile' qualifier from pointer type [automotive-c25-req-11.8]
    const int *ptr = (const int *)cv_ptr;
    (void)ptr;
}

// Violation - cast removes const from const volatile
void test_remove_const_only(const volatile int *cv_ptr) {
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: cast removes 'const' qualifier from pointer type [automotive-c25-req-11.8]
    volatile int *ptr = (volatile int *)cv_ptr;
    (void)ptr;
}

// Compliant - adding qualifiers is OK
void test_add_const(int *ptr) {
    const int *const_ptr = (const int *)ptr;
    (void)const_ptr;
}

// Compliant - adding volatile is OK
void test_add_volatile(int *ptr) {
    volatile int *volatile_ptr = (volatile int *)ptr;
    (void)volatile_ptr;
}

// Compliant - adding both qualifiers is OK
void test_add_both(int *ptr) {
    const volatile int *cv_ptr = (const volatile int *)ptr;
    (void)cv_ptr;
}

// Compliant - same qualifiers
void test_same_qualifiers(const int *const_ptr) {
    const int *another_const = (const int *)const_ptr;
    (void)another_const;
}

// Compliant - same qualifiers (volatile)
void test_same_qualifiers_volatile(volatile int *volatile_ptr) {
    volatile int *another_volatile = (volatile int *)volatile_ptr;
    (void)another_volatile;
}

// Compliant - same qualifiers (const volatile)
void test_same_qualifiers_both(const volatile int *cv_ptr) {
    const volatile int *another_cv = (const volatile int *)cv_ptr;
    (void)another_cv;
}

// Test with nested pointers - only check direct pointee
void test_nested_pointer(const int **pp) {
    // Outer pointer qualifiers don't matter, only the pointee
    int **p = (int **)pp;  // This should trigger on the inner const int*
    (void)p;
}

// Test with function pointers
// Note: check doesn't currently detect const removal from function pointers
void test_function_pointer(void (* const func_ptr)(void)) {
    void (*f)(void) = (void (*)(void))func_ptr;
    (void)f;
}

// Test with different types but same qualifiers (should not warn about qualifier removal)
void test_different_types(const char *char_ptr) {
    // This might warn about incompatible pointer cast in other checks,
    // but should warn here about removing const
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast removes 'const' qualifier from pointer type [automotive-c25-req-11.8]
    int *ptr = (int *)char_ptr;
    (void)ptr;
}
