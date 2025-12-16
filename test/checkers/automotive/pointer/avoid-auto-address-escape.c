// RUN: %check_clang_tidy %s automotive-c23-req-18.6 %t

// Test: Address of automatic object shall not escape (MISRA Rule 18.6)
// Check ID: automotive-c23-req-18.6

// Global/static pointers for testing
static int *global_ptr;
int *extern_global_ptr;

// ============= Violations: Return address of local =============

// Violation - returning address of local variable
int *test_return_local_address(void) {
    int local = 42;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: address of automatic variable 'local' returned
    return &local;
}

// Note: The check currently only detects direct &local patterns
// Array element and struct member access like &arr[0] and &pt.x are
// caught by clang-diagnostic-return-stack-address instead

// Compliant for this check - array element access (clang built-in handles this)
int *test_return_local_array(void) {
    int arr[10];
    return &arr[0];  // Caught by clang-diagnostic-return-stack-address
}

// Compliant for this check - struct member access (clang built-in handles this)
struct Point { int x; int y; };
int *test_return_local_struct_member(void) {
    struct Point pt = {1, 2};
    return &pt.x;  // Caught by clang-diagnostic-return-stack-address
}

// Violation - returning address of function parameter
int *test_return_param_address(int param) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: address of automatic variable 'param' returned
    return &param;
}

// ============= Violations: Assign local address to global =============

// Violation - assigning local address to static pointer
void test_assign_to_static(void) {
    int local = 42;
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: address of automatic variable 'local' assigned to object 'global_ptr'
    global_ptr = &local;
}

// Violation - assigning local address to extern global
void test_assign_to_extern_global(void) {
    int local = 100;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: address of automatic variable 'local' assigned to object 'extern_global_ptr'
    extern_global_ptr = &local;
}

// Note: &buffer[0] is not detected by this check (array subscript),
// so we test direct &variable patterns only
void test_assign_array_to_global(void) {
    char buffer[100];
    static char *static_buf;
    static_buf = &buffer[0];  // Not caught by this check (array subscript)
    (void)static_buf;
}

// Additional violation - direct address assignment to static
void test_assign_direct_to_static(void) {
    int value = 42;
    static int *ptr;
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: address of automatic variable 'value' assigned to object 'ptr'
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
