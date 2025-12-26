// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-12.2 %t -- -- -std=c++11
// Test for automotive-cpp23-req-12.2
// Related MISRA C++:2023 Rule: 12.2 (Partial)

// This test verifies that pointer subtraction between unrelated pointers
// is detected (pattern-based detection).

// Use built-in type instead of ptrdiff_t to avoid header dependencies
typedef long ptrdiff_t;

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_different_variables() {
    int a = 10;
    int b = 20;
    int* p1 = &a;
    int* p2 = &b;

    // Note: This pattern (through intermediate variables) is not currently detected
    // The check focuses on direct address-of expressions and new allocations
    ptrdiff_t diff = p1 - p2;
}

void test_direct_address_of() {
    int x = 1;
    int y = 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: pointer subtraction between pointers to different objects 'x' and 'y' results in undefined behavior
    ptrdiff_t d = &x - &y;
}

void test_different_members() {
    struct S {
        int a;
        int b;
    };

    S obj;

    // Direct address-of on different members
    // CHECK-MESSAGES: :[[@LINE+1]]:29: warning: pointer subtraction between pointers to different objects 'a' and 'b' results in undefined behavior
    ptrdiff_t diff = &obj.a - &obj.b;
}

void test_different_new_allocations() {
    // Direct subtraction of new expressions
    // CHECK-MESSAGES: :[[@LINE+1]]:34: warning: pointer subtraction between pointers from different 'new' expressions results in undefined behavior
    ptrdiff_t diff = new int(10) - new int(20);
}

void test_new_vs_addressof() {
    int x = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:34: warning: pointer subtraction between 'new' allocation and address-of expression results in undefined behavior
    ptrdiff_t diff = new int(20) - &x;
}

void test_addressof_vs_new() {
    int x = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: pointer subtraction between 'new' allocation and address-of expression results in undefined behavior
    ptrdiff_t diff = &x - new int(20);
}

void test_with_casts() {
    int a = 10;
    int b = 20;

    // Casts are stripped to reveal the underlying address-of expressions
    // CHECK-MESSAGES: :[[@LINE+1]]:44: warning: pointer subtraction between pointers to different objects 'a' and 'b' results in undefined behavior
    ptrdiff_t diff = static_cast<int*>(&a) - static_cast<int*>(&b);
}

void test_global_variables() {
    static int g1 = 1;
    static int g2 = 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: pointer subtraction between pointers to different objects 'g1' and 'g2' results in undefined behavior
    ptrdiff_t diff = &g1 - &g2;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_same_array() {
    int arr[10] = {0};
    int* p1 = &arr[0];
    int* p2 = &arr[5];

    // Valid: pointers to elements of the same array
    ptrdiff_t diff = p2 - p1;  // Should be 5
}

void test_array_pointer_arithmetic() {
    int arr[10] = {0};
    int* p1 = arr;
    int* p2 = arr + 5;

    // Valid: same array
    ptrdiff_t diff = p2 - p1;  // Should be 5
}

void test_same_variable() {
    int x = 10;
    int* p1 = &x;
    int* p2 = &x;

    // Valid: both point to the same variable
    ptrdiff_t diff = p1 - p2;  // Should be 0
}

void test_array_subscript() {
    int arr[10] = {0};

    // Valid: same array, different indices
    ptrdiff_t diff = &arr[7] - &arr[3];  // Should be 4
}

void test_string_literal() {
    const char* p1 = "hello";
    const char* p2 = p1 + 3;

    // Valid: same string literal
    ptrdiff_t diff = p2 - p1;  // Should be 3
}

void test_struct_array_members() {
    struct S {
        int arr[10];
    };

    S obj;
    int* p1 = &obj.arr[0];
    int* p2 = &obj.arr[5];

    // Valid: same array within struct
    ptrdiff_t diff = p2 - p1;  // Should be 5
}

void test_one_past_end() {
    int arr[10] = {0};
    int* begin = arr;
    int* end = arr + 10;  // One past the end

    // Valid: one-past-the-end pointer
    ptrdiff_t size = end - begin;  // Should be 10
}

void test_dynamic_array() {
    int* arr = new int[10];
    int* p1 = arr;
    int* p2 = arr + 5;

    // Valid: same dynamically allocated array
    ptrdiff_t diff = p2 - p1;  // Should be 5

    delete[] arr;
}

void test_pointer_from_same_new() {
    // This is actually undefined behavior (new int creates a single object,
    // not an array), but the check cannot detect this - it only detects
    // different new expressions
    int* p = new int(10);
    ptrdiff_t diff = (p + 1) - p;  // No warning (same new expression)
    delete p;
}

void test_function_parameters(int* p1, int* p2) {
    // The check cannot determine if p1 and p2 are related,
    // so no warning is issued
    ptrdiff_t diff = p1 - p2;
}

void test_unknown_pointers() {
    int arr[10] = {0};
    int* p1 = arr;
    int* p2;

    if (arr[0] > 0) {
        p2 = &arr[5];
    } else {
        p2 = &arr[2];
    }

    // No warning: same array, even though p2 has conditional initialization
    ptrdiff_t diff = p2 - p1;
}

void test_complex_expressions() {
    int arr[10] = {0};
    int i = 3;
    int j = 7;

    // Valid: same array with computed indices
    ptrdiff_t diff = &arr[j] - &arr[i];  // Should be 4
}

// Multidimensional array
void test_multidimensional_array() {
    int arr[5][10];
    int* p1 = &arr[0][0];
    int* p2 = &arr[0][5];

    // Valid: same contiguous array in memory
    ptrdiff_t diff = p2 - p1;  // Should be 5
}

// Test with different types (implicit conversion)
void test_void_pointers() {
    int arr[10] = {0};
    void* p1 = &arr[0];
    void* p2 = &arr[5];

    // This is actually a compile error (cannot subtract void pointers),
    // but if it compiles, it would be from the same array
    // ptrdiff_t diff = p2 - p1;  // Compile error
}

// Test with multidimensional array (same memory region)
void test_same_region_multidim() {
    int arr[5][10];
    int* p1 = &arr[0][0];
    int* p2 = &arr[1][5];

    // Valid: same contiguous memory region
    ptrdiff_t diff = p2 - p1;
}
