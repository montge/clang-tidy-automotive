// Test file for: automotive-c25-mand-18.10
// Related MISRA C:2025 Rule: 18.10
//
// This file tests the detection of pointers to variably-modified array types

// RUN: %check_clang_tidy %s automotive-c25-mand-18.10 %t -- -- -std=c99

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_pointer_to_vla_violations(void) {
    int n = 10;
    int m = 5;

    // Pointer to VLA with variable size
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
    int (*p1)[n];

    // Pointer to VLA with expression
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
    int (*p2)[n + 5];

    // Pointer to multi-dimensional VLA
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
    int (*p3)[n][m];

    // Pointer to VLA with complex expression
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
    int (*p4)[n * 2 + m];
}

void function_with_vla_pointer_param(int size) {
    // Local pointer to VLA
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
    int (*local_ptr)[size];

    // Pointer to 2D VLA
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
    int (*ptr_2d)[size][10];
}

void pointer_to_vla_in_loop(void) {
    for (int i = 1; i < 10; i++) {
        // Pointer to VLA declared in loop
        // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
        int (*arr_ptr)[i];
    }
}

void nested_scope_vla_pointer(void) {
    int outer_n = 20;
    {
        int inner_m = 15;
        // Pointer to VLA using outer scope variable
        // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
        int (*p1)[outer_n];

        // Pointer to VLA using inner scope variable
        // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
        int (*p2)[inner_m];
    }
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_array_pointers(void) {
    // Pointer to fixed-size array (constant expression)
    int (*p1)[10];
    int (*p2)[5 + 5];
    int (*p3)[2 * 8];

    // Pointer to multi-dimensional fixed array
    int (*p4)[10][20];

    // Pointer to array with enum constant
    enum { SIZE = 100 };
    int (*p5)[SIZE];

    // Pointer to array with sizeof expression
    int (*p6)[sizeof(int) * 10];
}

#define ARRAY_SIZE 256

void test_pointer_with_macro_size(void) {
    // Pointer to array with macro constant
    int (*ptr)[ARRAY_SIZE];
}

void test_pointer_with_literal_size(void) {
    // Pointer to arrays with literal sizes
    char (*buffer_ptr)[1024];
    int (*data_ptr)[64];
}

// Pointer to fixed-size array at file scope
static int (*global_array_ptr)[100];

void function_with_fixed_array_pointer(void) {
    // Pointer to fixed-size array - compliant
    int (*local_fixed_ptr)[20];
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void edge_case_const_expr_pointer(void) {
    // Pointer to array with constant expression - compliant
    int (*p1)[sizeof(int) * 10];

    // Pointer to array using sizeof for size - compliant
    int (*p2)[sizeof(int)];
}

// C99 allows static const int at file scope for array sizes
static const int FILE_SIZE = 128;

void edge_case_file_const_pointer(void) {
    // Note: FILE_SIZE is not a constant expression in C99 for array bounds
    // This may still be considered a VLA pointer in strict C99
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer to variably-modified array type shall not be used [automotive-c25-mand-18.10]
    int (*p)[FILE_SIZE];
}

void compliant_pointer_to_incomplete_array(void) {
    // Pointer to incomplete array type (not VLA) - compliant
    int (*p)[];
}

void compliant_simple_pointer(void) {
    // Regular pointer to int (not array pointer) - compliant
    int *simple_ptr;
    int n = 10;
    // Regular pointer initialized with variable - still compliant (not a pointer to VLA)
    int *ptr = &n;
}
