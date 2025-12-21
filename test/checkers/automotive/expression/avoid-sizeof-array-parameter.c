// RUN: %check_clang_tidy %s automotive-c23-req-12.5 %t
// Test for automotive-c23-req-12.5
// Related MISRA C:2025 Rule: 12.5

// This test verifies that sizeof on array parameters is detected.

#include <stddef.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_array_param_complete(int arr[10]) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: sizeof applied to parameter 'arr' declared as array
    size_t s = sizeof(arr);
    (void)s;
}

void test_array_param_incomplete(int arr[]) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: sizeof applied to parameter 'arr' declared as array
    size_t s = sizeof(arr);
    (void)s;
}

void test_multidim_array(int arr[5][10]) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: sizeof applied to parameter 'arr' declared as array
    size_t s = sizeof(arr);
    (void)s;
}

void test_array_in_expression(char buf[100]) {
    // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: sizeof applied to parameter 'buf' declared as array
    size_t remaining = sizeof(buf) - 1;
    (void)remaining;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_pointer_param(int *ptr) {
    // Explicitly declared as pointer - no warning
    size_t s = sizeof(ptr);
    (void)s;
}

void test_sizeof_type(int arr[10]) {
    // sizeof on type, not on the parameter - no warning
    size_t s = sizeof(int[10]);
    (void)s;
}

void test_sizeof_element(int arr[10]) {
    // sizeof on dereferenced element - no warning
    size_t s = sizeof(*arr);
    (void)s;
}

void test_sizeof_element_type(int arr[10]) {
    // sizeof on element type - no warning
    size_t s = sizeof(int);
    (void)s;
}

void test_local_array(void) {
    // Local array (not a parameter) - no warning
    int local[10];
    size_t s = sizeof(local);
    (void)s;
}

void test_static_array(void) {
    // Static array - no warning
    static int arr[10];
    size_t s = sizeof(arr);
    (void)s;
}

int global_array[10];

void test_global_array(void) {
    // Global array - no warning
    size_t s = sizeof(global_array);
    (void)s;
}

struct Data {
    int arr[10];
};

void test_struct_member(struct Data *d) {
    // Struct member array - no warning (sizeof on member, not param)
    size_t s = sizeof(d->arr);
    (void)s;
}

void test_sizeof_pointer_to_pointer(int **pp) {
    // Double pointer - no warning
    size_t s = sizeof(pp);
    (void)s;
}
