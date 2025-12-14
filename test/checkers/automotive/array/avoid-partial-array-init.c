// Test file for: automotive-avoid-partial-array-init
// Related MISRA C:2025 Rule: 9.3
//
// This file tests the detection of partially initialized arrays

// RUN: %check_clang_tidy %s automotive-avoid-partial-array-init %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_partial_array_init(void) {
    // Array with 5 elements, only 3 initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    int arr1[5] = {1, 2, 3};

    // Array with 10 elements, only 1 initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    int arr2[10] = {42};

    // Array with 8 elements, only 6 initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    int arr3[8] = {1, 2, 3, 4, 5, 6};

    // Large array with few initializers
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    int arr4[100] = {1, 2, 3};

    // Character array partially initialized (not string literal)
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    char arr5[20] = {'a', 'b', 'c'};

    // Float array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    float arr6[5] = {1.0, 2.0};

    // Double array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    double arr7[4] = {1.1, 2.2, 3.3};

    // Unsigned array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]>::9: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    unsigned int arr8[6] = {1, 2, 3, 4};
}

void test_partial_multidim_array(void) {
    // Multi-dimensional array with partial init
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    int arr1[3][2] = {{1, 2}, {3, 4}};

    // 2D array with only one row initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    int arr2[5][3] = {{1, 2, 3}};

    // 3D array partially initialized
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    int arr3[2][2][2] = {{{1, 2}}};
}

void test_struct_with_array(void) {
    struct Data {
        int id;
        int values[10];
    };

    // Struct with partially initialized array member
    // Note: This tests if the check catches the array member
    struct Data d1 = {1, {1, 2, 3}};  // values array is partially initialized
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_fully_initialized_arrays(void) {
    // Fully initialized arrays
    int arr1[5] = {1, 2, 3, 4, 5};
    int arr2[3] = {10, 20, 30};
    char arr3[4] = {'a', 'b', 'c', 'd'};
    float arr4[2] = {1.5f, 2.5f};
    double arr5[3] = {1.1, 2.2, 3.3};

    // Multi-dimensional fully initialized
    int arr6[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int arr7[2][2] = {{1, 2}, {3, 4}};

    // Array with size inferred from initializer
    int arr8[] = {1, 2, 3, 4, 5};
    char arr9[] = {'x', 'y', 'z'};
}

void test_zero_initialization(void) {
    // Zero initialization is allowed (special case)
    int arr1[10] = {0};
    char arr2[50] = {0};
    float arr3[5] = {0};
    double arr4[3] = {0};

    // Empty initialization list (zero-initializes all)
    int arr5[5] = {};
    char arr6[10] = {};
}

void test_string_initialization(void) {
    // String literals are allowed
    char str1[10] = "Hello";
    char str2[20] = "Test string";
    char str3[6] = "World";  // Exactly fits with null terminator

    // Wide string
    wchar_t wstr[10] = L"Wide";
}

void test_const_arrays(void) {
    // Const arrays fully initialized
    const int arr1[3] = {100, 200, 300};
    const char arr2[5] = "Test";

    // Static arrays fully initialized
    static int arr3[4] = {1, 2, 3, 4};
    static char arr4[10] = "Static";
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_single_element_array(void) {
    // Single element array fully initialized - compliant
    int arr1[1] = {42};
    char arr2[1] = {'x'};
}

void test_typedef_arrays(void) {
    typedef int IntArray5[5];

    // Fully initialized typedef'd array
    IntArray5 arr1 = {1, 2, 3, 4, 5};

    // Partially initialized typedef'd array - should warn
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    IntArray5 arr2 = {1, 2};
}

void test_designated_initializers(void) {
    // Designated initializers with gaps - partial initialization
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    int arr1[10] = {[0] = 1, [5] = 6};

    // Designated initializers filling all elements - compliant
    int arr2[3] = {[0] = 10, [1] = 20, [2] = 30};
}

void test_volatile_arrays(void) {
    // Volatile array fully initialized
    volatile int arr1[3] = {1, 2, 3};

    // Volatile array partially initialized - should warn
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    volatile int arr2[5] = {1, 2};
}

void test_array_of_structs(void) {
    struct Point {
        int x;
        int y;
    };

    // Array of structs fully initialized
    struct Point points1[2] = {{1, 2}, {3, 4}};

    // Array of structs partially initialized - should warn
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: avoid partially initialized array [automotive-avoid-partial-array-init]
    struct Point points2[3] = {{1, 2}, {3, 4}};
}
