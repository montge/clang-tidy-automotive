// Test file for: automotive-avoid-variable-length-array
// Related MISRA C:2025 Rule: 18.8
//
// This file tests the detection of variable-length arrays (VLAs)

// RUN: %check_clang_tidy %s automotive-avoid-variable-length-array %t -- -- -std=c99

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_vla_violations(void) {
    int n = 10;

    // VLA with variable size
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: avoid variable-length array [automotive-avoid-variable-length-array]
    int arr1[n];

    // VLA in function parameter
    int m = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: avoid variable-length array [automotive-avoid-variable-length-array]
    int arr2[m * 2];

    // Multi-dimensional VLA
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: avoid variable-length array [automotive-avoid-variable-length-array]
    int arr3[n][m];

    // VLA with expression
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: avoid variable-length array [automotive-avoid-variable-length-array]
    int arr4[n + m];
}

void function_with_vla_param(int n, int arr[n]) {
    // VLA as parameter - already detected by parameter
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: avoid variable-length array [automotive-avoid-variable-length-array]
    int local_vla[n];
}

void vla_in_loop(void) {
    for (int i = 1; i < 10; i++) {
        // VLA declared in loop
        // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid variable-length array [automotive-avoid-variable-length-array]
        int arr[i];
    }
}

void vla_with_typedef(void) {
    int n = 20;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: avoid variable-length array [automotive-avoid-variable-length-array]
    int (*ptr)[n];  // Pointer to VLA
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_arrays(void) {
    // Fixed-size arrays (constant expressions)
    int arr1[10];
    int arr2[5 + 5];
    int arr3[2 * 8];

    // Multi-dimensional fixed arrays
    int arr4[10][20];

    // Array with enum constant
    enum { SIZE = 100 };
    int arr5[SIZE];

    // Array with const (though this might still be VLA in C99 without static/extern)
    const int size = 50;
    // This is compliant in some contexts but may need static const
}

#define ARRAY_SIZE 256

void test_macro_size(void) {
    // Array with macro constant
    int arr[ARRAY_SIZE];
}

void test_literal_size(void) {
    // Literal sizes
    char buffer[1024];
    int data[64];
}

// Static arrays at file scope
static int global_array[100];

// Function with fixed-size array parameter
void function_with_fixed_array(int arr[10]) {
    int local_fixed[20];
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void edge_case_const_expr(void) {
    // Constant expression - compliant
    int arr1[sizeof(int) * 10];

    // Using sizeof for size
    int arr2[sizeof(int)];
}

// C99 allows static const int at file scope for array sizes
static const int FILE_SIZE = 128;

void edge_case_file_const(void) {
    // This is compliant - FILE_SIZE is a compile-time constant
    int arr[FILE_SIZE];
}
