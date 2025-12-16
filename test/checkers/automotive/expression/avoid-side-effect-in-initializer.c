// RUN: %check_clang_tidy %s automotive-c23-req-13.1 %t

// Test: Side effects in initializer list (MISRA Rule 13.1)

int global_counter = 0;

int side_effect_func(void) {
    return ++global_counter;
}

// Violation - side effect in initializer list
void test_side_effect_in_init_list(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: initializer list element contains side effects
    int arr[3] = { side_effect_func(), 2, 3 };
    (void)arr;
}

// Violation - increment in initializer
void test_increment_in_init(void) {
    int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: initializer list element contains side effects
    int arr[3] = { x++, 2, 3 };
    (void)arr;
}

// Compliant - no side effects
void test_compliant(void) {
    int arr[3] = { 1, 2, 3 };
    (void)arr;
}

// Use global_counter to avoid warning
void use_global(void) {
    (void)global_counter;
}
