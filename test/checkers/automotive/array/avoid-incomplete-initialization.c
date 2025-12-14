// Test file for: automotive-avoid-incomplete-initialization
// Related MISRA C:2025 Rule: 9.3
//
// This file tests the detection of incomplete initialization of arrays and structures

// RUN: %check_clang_tidy %s automotive-avoid-incomplete-initialization %t

#include <stdint.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_array_incomplete_init(void) {
    // Array with 5 elements, only 3 initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: array has 5 elements but only 3 initializers provided; all elements should be explicitly initialized [automotive-avoid-incomplete-initialization]
    int arr1[5] = {1, 2, 3};

    // Array with 10 elements, only 1 initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: array has 10 elements but only 1 initializers provided; all elements should be explicitly initialized [automotive-avoid-incomplete-initialization]
    int arr2[10] = {42};

    // Multi-dimensional array with incomplete init
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: array has 3 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-avoid-incomplete-initialization]
    int arr3[3][2] = {{1, 2}, {3, 4}};

    // Character array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: array has 20 elements but only 5 initializers provided; all elements should be explicitly initialized [automotive-avoid-incomplete-initialization]
    char str[20] = {'H', 'e', 'l', 'l', 'o'};

    // Array with designated initializers (partial)
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: array has 10 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-avoid-incomplete-initialization]
    int arr4[10] = {[0] = 1, [5] = 6};
}

void test_struct_incomplete_init(void) {
    // Struct with 3 fields, only 2 initialized
    struct Point {
        int x;
        int y;
        int z;
    };
    // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: aggregate has 3 members but only 2 initializers provided; all members should be explicitly initialized [automotive-avoid-incomplete-initialization]
    struct Point p1 = {10, 20};

    // Struct with 5 fields, only 3 initialized
    struct Person {
        char name[50];
        int age;
        int id;
        float salary;
        char department[20];
    };
    // CHECK-MESSAGES: :[[@LINE+1]]:28: warning: aggregate has 5 members but only 3 initializers provided; all members should be explicitly initialized [automotive-avoid-incomplete-initialization]
    struct Person person = {"John", 30, 1001};

    // Nested struct with partial initialization
    struct Outer {
        int a;
        struct Inner {
            int b;
            int c;
        } inner;
        int d;
    };
    // CHECK-MESSAGES: :[[@LINE+1]]:27: warning: aggregate has 3 members but only 1 initializers provided; all members should be explicitly initialized [automotive-avoid-incomplete-initialization]
    struct Outer outer = {1};
}

void test_mixed_types(void) {
    // Array of structs with partial init
    struct Data {
        int value;
        int status;
    };
    // CHECK-MESSAGES: :[[@LINE+1]]:28: warning: array has 5 elements but only 2 initializers provided; all elements should be explicitly initialized [automotive-avoid-incomplete-initialization]
    struct Data arr[5] = {{1, 0}, {2, 1}};

    // Struct containing arrays with partial init
    struct Container {
        int header;
        int data[10];
        int footer;
    };
    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: aggregate has 3 members but only 2 initializers provided; all members should be explicitly initialized [automotive-avoid-incomplete-initialization]
    struct Container cont = {0xFF, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_complete_initialization(void) {
    // Fully initialized array
    int arr1[5] = {1, 2, 3, 4, 5};

    // Multi-dimensional array fully initialized
    int arr2[2][3] = {{1, 2, 3}, {4, 5, 6}};

    // Character array initialized as string (with null terminator)
    char str1[6] = "Hello";  // Includes null terminator

    // Zero initialization using {0} - special case allowed
    int arr3[10] = {0};
    char str2[100] = {0};

    // Fully initialized struct
    struct Point {
        int x;
        int y;
        int z;
    };
    struct Point p1 = {10, 20, 30};

    // Struct with zero initialization
    struct Point p2 = {0};  // Special case allowed

    // Array without size (size inferred from initializer)
    int arr4[] = {1, 2, 3, 4};

    // Struct with designated initializers (all fields)
    struct RGB {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    struct RGB color = {.r = 255, .g = 128, .b = 64};
}

void test_empty_initialization(void) {
    // Empty initialization list {} - zero-initializes all
    int arr1[5] = {};

    struct Data {
        int a;
        int b;
    };
    struct Data d1 = {};
}

void test_string_literals(void) {
    // String literals are OK when they fit
    char msg[10] = "Hello";  // Has room for null terminator

    // String literal exactly fits
    char exact[6] = "Hello";  // 5 chars + null = 6
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_edge_cases(void) {
    // Single element array fully initialized
    int single[1] = {42};

    // Two element array fully initialized
    int pair[2] = {1, 2};

    // Union initialization (only first member initialized by design)
    union Value {
        int i;
        float f;
        char c;
    };
    union Value v1 = {42};  // Unions only initialize first member - this is OK

    // Flexible array member (last member, incomplete type)
    struct FlexArray {
        int count;
        int data[];  // Flexible array member
    };
    struct FlexArray fa = {5};  // Only count initialized - data is flexible

    // Typedef'd array
    typedef int IntArray[10];
    IntArray ta = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Const array
    const int ca[3] = {1, 2, 3};

    // Static array
    static int sa[4] = {10, 20, 30, 40};

    // Volatile array
    volatile int va[2] = {100, 200};
}

void test_nested_edge_cases(void) {
    // Deeply nested struct
    struct Level3 {
        int c;
    };
    struct Level2 {
        int b;
        struct Level3 l3;
    };
    struct Level1 {
        int a;
        struct Level2 l2;
    };
    struct Level1 nested = {1, {2, {3}}};  // Fully initialized

    // Array of arrays
    int matrix[2][2] = {{1, 2}, {3, 4}};  // Fully initialized
}
