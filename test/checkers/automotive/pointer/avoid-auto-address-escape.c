// RUN: %check_clang_tidy %s automotive-c23-req-18.6 %t -- -- -Wno-return-stack-address

// Test: Address of automatic object shall not escape (MISRA Rule 18.6)
// Check ID: automotive-c23-req-18.6

// Global/static pointers for testing
static int *global_ptr;
int *extern_global_ptr;

// ============= Violations: Return address of local =============

// Violation - returning address of local variable
int *test_return_local_address(void) {
    int local = 42;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: address of automatic variable 'local' returned; object will not exist after function returns [automotive-c23-req-18.6]
    return &local;
}

// Violation - returning address of function parameter
int *test_return_param_address(int param) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: address of automatic variable 'param' returned; object will not exist after function returns [automotive-c23-req-18.6]
    return &param;
}

// ============= Violations: Assign local address to global =============

// Violation - assigning local address to static pointer
void test_assign_to_static(void) {
    int local = 42;
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: address of automatic variable 'local' assigned to object 'global_ptr' with greater lifetime [automotive-c23-req-18.6]
    global_ptr = &local;
}

// Violation - assigning local address to extern global
void test_assign_to_extern_global(void) {
    int local = 100;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: address of automatic variable 'local' assigned to object 'extern_global_ptr' with greater lifetime [automotive-c23-req-18.6]
    extern_global_ptr = &local;
}

// Additional violation - direct address assignment to static
void test_assign_direct_to_static(void) {
    int value = 42;
    static int *ptr;
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: address of automatic variable 'value' assigned to object 'ptr' with greater lifetime [automotive-c23-req-18.6]
    ptr = &value;
    (void)ptr;
}

// ============= Compliant: Static storage =============

// Compliant - returning address of static variable
int *test_return_static_address(void) {
    static int static_var = 42;
    return &static_var;  // OK - static has program lifetime
}

// Compliant - returning address of static local
char *test_return_static_buffer(void) {
    static char buffer[256];
    return buffer;  // OK - static storage
}

// ============= Compliant: Global storage =============

int global_value = 0;
int *test_return_global_address(void) {
    return &global_value;  // OK - global has program lifetime
}

// ============= Compliant: Heap allocation =============

extern void *malloc(unsigned long);

int *test_return_heap(void) {
    int *heap_ptr = malloc(sizeof(int));
    return heap_ptr;  // OK - heap-allocated
}

// ============= Compliant: Output parameters =============

void test_output_param(int **out) {
    static int value = 42;
    *out = &value;  // OK - pointing to static
}

// ============= Compliant: Local pointer to local =============

void test_local_to_local(void) {
    int local = 42;
    int *local_ptr = &local;  // OK - both have same lifetime
    (void)local_ptr;
}

// Use globals to avoid warnings
void use_globals(void) {
    (void)global_ptr;
    (void)extern_global_ptr;
}
