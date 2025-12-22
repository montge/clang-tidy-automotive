// Test file for: automotive-c25-adv-9.7
// Related MISRA C:2025 Rule: 9.7
//
// This file tests the detection of partial array initialization
// where not all elements are explicitly initialized.

// RUN: %check_clang_tidy %s automotive-c25-adv-9.7 %t

#include <stddef.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_partial_init_basic(void) {
    // Basic partial initialization - only 2 of 5 elements initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: array has 5 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    int arr1[5] = {1, 2};

    // Only 1 of 10 elements initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: array has 10 elements but only 1 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    int arr2[10] = {42};

    // Partially initialized large array
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: array has 100 elements but only 3 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    int arr3[100] = {1, 2, 3};
}

void test_partial_init_various_types(void) {
    // Character array partially initialized (not string literal)
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: array has 20 elements but only 3 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    char arr1[20] = {'a', 'b', 'c'};

    // Float array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: array has 5 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    float arr2[5] = {1.0, 2.0};

    // Double array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: array has 4 elements but only 3 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    double arr3[4] = {1.1, 2.2, 3.3};

    // Unsigned array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: array has 6 elements but only 4 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    unsigned int arr4[6] = {1, 2, 3, 4};

    // Long array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: array has 8 elements but only 5 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    long arr5[8] = {1L, 2L, 3L, 4L, 5L};

    // Short array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: array has 3 elements but only 1 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    short arr6[3] = {100};
}

void test_partial_init_multidimensional(void) {
    // Multi-dimensional array with partial outer dimension init
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: array has 3 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    int arr1[3][2] = {{1, 2}, {3, 4}};

    // 2D array with only one row initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: array has 5 elements but only 1 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    int arr2[5][3] = {{1, 2, 3}};

    // 3D array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: array has 2 elements but only 1 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    int arr3[2][2][2] = {{{1, 2}}};
}

void test_partial_init_with_qualifiers(void) {
    // Const array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: array has 5 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    const int arr1[5] = {10, 20};

    // Volatile array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: array has 4 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    volatile int arr2[4] = {1, 2};

    // Static array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: array has 6 elements but only 3 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    static int arr3[6] = {5, 6, 7};
}

void test_partial_init_designated(void) {
    // Designated initializers with gaps
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: array has 10 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    int arr1[10] = {[0] = 1, [5] = 6};

    // Designated initializer not filling all elements
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: array has 5 elements but only 3 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    int arr2[5] = {[0] = 10, [2] = 30, [3] = 40};
}

void test_partial_init_structs(void) {
    struct Point {
        int x;
        int y;
    };

    // Array of structs partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: array has 3 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    struct Point pts[3] = {{1, 2}, {3, 4}};

    // Array of structs with only first element initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: array has 5 elements but only 1 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    struct Point pts2[5] = {{10, 20}};
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_fully_initialized(void) {
    // Fully initialized arrays - all elements specified
    int arr1[5] = {1, 2, 3, 4, 5};
    int arr2[3] = {10, 20, 30};
    char arr3[4] = {'a', 'b', 'c', 'd'};
    float arr4[2] = {1.5f, 2.5f};
    double arr5[3] = {1.1, 2.2, 3.3};

    // Multi-dimensional fully initialized
    int arr6[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int arr7[2][2] = {{1, 2}, {3, 4}};

    // Single element array fully initialized
    int arr8[1] = {42};
}

void test_size_inferred(void) {
    // Array size inferred from initializer - always compliant
    int arr1[] = {1, 2, 3, 4, 5};
    char arr2[] = {'x', 'y', 'z'};
    float arr3[] = {1.0f, 2.0f};
    double arr4[] = {1.1};
}

void test_zero_initialization(void) {
    // Empty initializer - explicit zero-initialization
    int arr1[10] = {};
    char arr2[50] = {};
    float arr3[5] = {};
    double arr4[3] = {};

    // Single zero initializer - special case for zero-init
    int arr5[10] = {0};
    char arr6[20] = {0};
    float arr7[8] = {0};
    double arr8[6] = {0};
}

void test_string_initialization(void) {
    // String literals are allowed (special case)
    char str1[10] = "Hello";
    char str2[20] = "Test string";
    char str3[6] = "World";

    // Wide string
    wchar_t wstr1[10] = L"Wide";
    wchar_t wstr2[15] = L"Another";

    // Empty string
    char str4[5] = "";
}

void test_designated_fully_init(void) {
    // Designated initializers filling all elements
    int arr1[3] = {[0] = 10, [1] = 20, [2] = 30};
    int arr2[5] = {[0] = 1, [1] = 2, [2] = 3, [3] = 4, [4] = 5};
}

void test_struct_arrays_compliant(void) {
    struct Point {
        int x;
        int y;
    };

    // Array of structs fully initialized
    struct Point pts1[2] = {{1, 2}, {3, 4}};
    struct Point pts2[3] = {{10, 20}, {30, 40}, {50, 60}};

    // Single element struct array
    struct Point pts3[1] = {{100, 200}};
}

void test_const_static_fully_init(void) {
    // Const arrays fully initialized
    const int arr1[3] = {100, 200, 300};
    const char arr2[5] = "Test";

    // Static arrays fully initialized
    static int arr3[4] = {1, 2, 3, 4};
    static char arr4[10] = "Static";

    // Volatile arrays fully initialized
    volatile int arr5[2] = {99, 88};
}

void test_no_initializer(void) {
    // Arrays without initializers - not subject to this rule
    int arr1[10];
    char arr2[20];
    float arr3[5];
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_typedef_arrays(void) {
    typedef int IntArray5[5];

    // Fully initialized typedef'd array
    IntArray5 arr1 = {1, 2, 3, 4, 5};

    // Partially initialized typedef'd array
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: array has 5 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    IntArray5 arr2 = {1, 2};
}

void test_nested_structs(void) {
    struct Inner {
        int data[3];
    };
    struct Outer {
        struct Inner inner;
    };

    // Struct with nested array - testing inner array initialization
    struct Outer obj1 = {{1, 2, 3}};
}

void test_complex_types(void) {
    // Pointer array fully initialized
    int *ptrs[3] = {NULL, NULL, NULL};

    // Pointer array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: array has 5 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-c25-adv-9.7]
    int *ptrs2[5] = {NULL, NULL};
}
