// RUN: %check_clang_tidy %s automotive-cpp23-adv-6.0 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-6.0
// Related MISRA C++:2023 Rule: 6.0

// This test verifies that uninitialized array declarations are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_uninitialized_arrays() {
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: array 'arr1' is not explicitly initialized
    int arr1[10];

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: array 'arr2' is not explicitly initialized
    float arr2[5];

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: array 'arr3' is not explicitly initialized
    char arr3[100];

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: array 'matrix' is not explicitly initialized
    double matrix[3][3];

    (void)arr1;
    (void)arr2;
    (void)arr3;
    (void)matrix;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_initialized_arrays() {
    // Value-initialized (zero-initialized)
    int arr1[10] = {};

    // Explicitly initialized with values
    int arr2[5] = {1, 2, 3, 4, 5};

    // Partially initialized (remaining elements zero-initialized)
    int arr3[10] = {1, 2, 3};

    // Character array from string literal
    char str[] = "hello";

    // Brace-initialized
    float arr4[3] = {1.0f, 2.0f, 3.0f};

    // Multi-dimensional initialized
    int matrix[2][2] = {{1, 2}, {3, 4}};

    (void)arr1;
    (void)arr2;
    (void)arr3;
    (void)str;
    (void)arr4;
    (void)matrix;
}

// Extern declarations are OK (not definitions)
extern int external_array[10];

// Static class members declared but not defined here are OK
class MyClass {
    static int static_array[5];
};

void test_const_arrays() {
    // const arrays must be initialized
    const int arr[3] = {1, 2, 3};
    (void)arr;
}
