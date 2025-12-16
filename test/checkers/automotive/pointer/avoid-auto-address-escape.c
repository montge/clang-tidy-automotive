// RUN: %check_clang_tidy %s automotive-c23-req-18.6 %t

// Test: Address of automatic object shall not escape (MISRA Rule 18.6)

static int *global_ptr;

// Violation - returning address of local variable
int *test_return_local_address(void) {
    int local = 42;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: address of automatic variable
    return &local;
}

// Violation - assigning local address to global pointer
void test_assign_to_global(void) {
    int local = 42;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: address of automatic variable
    global_ptr = &local;
}

// Compliant - returning address of static variable
int *test_return_static_address(void) {
    static int static_var = 42;
    return &static_var;
}

// Compliant - returning heap-allocated memory
int *test_return_heap(void) {
    extern void *malloc(unsigned long);
    int *heap_ptr = malloc(sizeof(int));
    return heap_ptr;
}

// Use global_ptr to avoid unused warning
void use_global(void) {
    (void)global_ptr;
}
