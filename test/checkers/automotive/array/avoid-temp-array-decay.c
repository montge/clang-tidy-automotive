// Test file for: automotive-c25-req-18.9
// Related MISRA C:2025 Rule: 18.9
//
// This file tests the detection of array-to-pointer conversion of objects
// with temporary lifetime (compound literals with array type)

// RUN: %check_clang_tidy %s automotive-c25-req-18.9 %t -- -- -std=c99

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_compound_literal_decay_violations(void) {
    // Direct assignment of compound literal array to pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    int *p1 = (int[]){1, 2, 3};

    // Compound literal with explicit size
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    int *p2 = (int[5]){1, 2, 3, 4, 5};

    // Char array compound literal
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    char *str = (char[]){"hello"};

    // Float array compound literal
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    float *fp = (float[]){1.0f, 2.0f, 3.0f};

    // Double array compound literal
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    double *dp = (double[3]){1.0, 2.0, 3.0};
}

void function_taking_pointer(int *ptr) {
    (void)ptr;
}

void test_compound_literal_as_argument(void) {
    // Passing compound literal array as pointer argument
    // CHECK-MESSAGES: :[[@LINE+1]]:29: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    function_taking_pointer((int[]){1, 2, 3});

    // Compound literal with specified size as argument
    // CHECK-MESSAGES: :[[@LINE+1]]:29: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    function_taking_pointer((int[10]){0});
}

void test_pointer_assignment_from_compound_literal(void) {
    int *ptr;

    // Assignment to existing pointer variable
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    ptr = (int[]){4, 5, 6};

    // Multiple assignments
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    ptr = (int[3]){7, 8, 9};
}

void test_multidimensional_array_compound_literal(void) {
    // 2D array compound literal decay
    // Note: This decays from int[2][3] to int (*)[3], which is still array decay
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    int (*p)[3] = (int[][3]){{1, 2, 3}, {4, 5, 6}};
}

struct Point {
    int x;
    int y;
};

void test_struct_array_compound_literal(void) {
    // Array of structures as compound literal
    // CHECK-MESSAGES: :[[@LINE+1]]:28: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    struct Point *points = (struct Point[]){{1, 2}, {3, 4}, {5, 6}};
}

void test_const_pointer_from_compound_literal(void) {
    // Const pointer to compound literal array
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    const int *cp = (int[]){10, 20, 30};
}

void test_return_compound_literal_as_pointer(void) {
    int *result;
    // Compound literal in conditional expression
    // CHECK-MESSAGES: :[[@LINE+2]]:18: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    // CHECK-MESSAGES: :[[@LINE+1]]:34: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    result = 1 ? (int[]){1, 2} : (int[]){3, 4};
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_array_usage(void) {
    // Regular array declaration - no decay yet
    int arr1[5] = {1, 2, 3, 4, 5};

    // Pointer to array element (not array-to-pointer decay of compound literal)
    int *p1 = &arr1[0];

    // Pointer via array decay of regular array (not compound literal)
    int *p2 = arr1;

    // String literal (not a compound literal)
    char *str = "hello";
    const char *cstr = "world";
}

void test_compliant_compound_literal_no_decay(void) {
    // Note: Array subscript on compound literals still involves array-to-pointer
    // decay, so these will trigger warnings
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    int value1 = (int[]){1, 2, 3}[0];
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    int value2 = (int[]){4, 5, 6}[1];

    // Compound literal address taken (creates pointer to array, not decay)
    int (*arr_ptr)[3] = &(int[3]){1, 2, 3};

    // Use the variables to avoid unused warnings
    (void)value1;
    (void)value2;
    (void)arr_ptr;
}

void test_compliant_static_storage(void) {
    // In function scope, even with static storage class, compound literals
    // still have automatic storage duration in C99
    // This would be a violation but has a compiler error in strict C99
    // static int *sp = (int[]){1, 2, 3};
}

// File scope compound literal (static storage duration) - compliant
// File-scope compound literals have static storage duration, not temporary lifetime
int *global_ptr = (int[]){10, 20, 30};

void test_compliant_array_initialization(void) {
    // Regular array initialization with initializer list
    int arr[] = {1, 2, 3};

    // Array of arrays
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};

    // Use to avoid warnings
    (void)arr;
    (void)matrix;
}

void test_compliant_sizeof_compound_literal(void) {
    // Using sizeof on compound literal (no decay)
    unsigned long size1 = sizeof((int[]){1, 2, 3});
    unsigned long size2 = sizeof((char[]){"test"});

    (void)size1;
    (void)size2;
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void edge_case_nested_compound_literals(void) {
    // Nested compound literal (outer array decays)
    // CHECK-MESSAGES: :[[@LINE+3]]:16: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    // CHECK-MESSAGES: :[[@LINE+2]]:25: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    // CHECK-MESSAGES: :[[@LINE+1]]:37: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
    int **pp = (int*[]){(int[]){1}, (int[]){2}};
}

void edge_case_cast_scenarios(void) {
    // Explicit cast to pointer from compound literal
    // The implicit array-to-pointer decay still occurs
    // Note: The compound literal is inside the cast, so column might vary
    int *p = (int *)((int[]){1, 2, 3});
}

void edge_case_typedef_pointer(void) {
    typedef int* IntPtr;

    // Compound literal decay via typedef
    IntPtr ip = (int[]){5, 6, 7};
    // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: object with temporary lifetime shall not undergo array-to-pointer conversion [automotive-c25-req-18.9]
}
