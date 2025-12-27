// RUN: %check_clang_tidy -std=c++11 %s automotive-cpp23-adv-7.6.1 %t -- -- -Wno-sizeof-array-argument

// Test for automotive-cpp23-adv-7.6.1: sizeof on array parameter
// Related MISRA C++:2023 Rule: 7.6.1 (Advisory)
//
// The sizeof operator should not be used on an array parameter.
// When an array is passed to a function, it decays to a pointer,
// so sizeof will return the pointer size rather than the array size.

typedef unsigned long size_t;

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_array_param_complete(int arr[10]) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: sizeof applied to parameter 'arr' declared as array; this gives the size of a pointer, not the array
    size_t s = sizeof(arr);
    (void)s;
}

void test_array_param_incomplete(int arr[]) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: sizeof applied to parameter 'arr' declared as array; this gives the size of a pointer, not the array
    size_t s = sizeof(arr);
    (void)s;
}

void test_multidim_array(int arr[5][10]) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: sizeof applied to parameter 'arr' declared as array; this gives the size of a pointer, not the array
    size_t s = sizeof(arr);
    (void)s;
}

void test_array_in_expression(char buf[100]) {
    // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: sizeof applied to parameter 'buf' declared as array; this gives the size of a pointer, not the array
    size_t remaining = sizeof(buf) - 1;
    (void)remaining;
}

// C++ specific: const qualified array parameter
void test_const_array(const int arr[10]) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: sizeof applied to parameter 'arr' declared as array; this gives the size of a pointer, not the array
    size_t s = sizeof(arr);
    (void)s;
}

// C++ specific: reference to array (this is different from array parameter)
// Note: This should NOT trigger because the reference prevents decay
void test_array_reference(int (&arr)[10]) {
    // No warning - array reference does not decay
    size_t s = sizeof(arr);
    (void)s;
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

void test_local_array() {
    // Local array (not a parameter) - no warning
    int local[10];
    size_t s = sizeof(local);
    (void)s;
}

void test_static_array() {
    // Static array - no warning
    static int arr[10];
    size_t s = sizeof(arr);
    (void)s;
}

int global_array[10];

void test_global_array() {
    // Global array - no warning
    size_t s = sizeof(global_array);
    (void)s;
}

struct Data {
    int arr[10];
};

void test_struct_member(Data *d) {
    // Struct member array - no warning (sizeof on member, not param)
    size_t s = sizeof(d->arr);
    (void)s;
}

void test_sizeof_pointer_to_pointer(int **pp) {
    // Double pointer - no warning
    size_t s = sizeof(pp);
    (void)s;
}

// C++ specific: template function
template<typename T>
void test_template_pointer(T *ptr) {
    // Template pointer - no warning
    size_t s = sizeof(ptr);
    (void)s;
}
