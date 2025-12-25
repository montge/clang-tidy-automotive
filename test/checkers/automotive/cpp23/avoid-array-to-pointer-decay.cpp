// RUN: %check_clang_tidy %s automotive-cpp23-req-5.10 %t

#include <cstddef>

void sink(int *p);
void sink_const(const int *p);

// Test violations - implicit array to pointer decay

void test_function_call() {
    int arr[10];
    sink(arr);
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: array 'int[10]' decays to pointer
}

void test_assignment() {
    int arr[10];
    int *p = arr;
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: array 'int[10]' decays to pointer
}

void test_pointer_arithmetic() {
    int arr[10];
    int *p = arr + 1;
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: array 'int[10]' decays to pointer
}

void test_comparison() {
    int arr[10];
    int *p = nullptr;
    bool b = (arr == p);
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: array 'int[10]' decays to pointer
}

// Test compliant cases - no warning expected

void test_subscript() {
    int arr[10];
    int x = arr[0];  // Subscript is OK
}

void test_sizeof() {
    int arr[10];
    size_t s = sizeof(arr);  // sizeof is OK
}

void test_address_of() {
    int arr[10];
    int (*p)[10] = &arr;  // Address-of is OK
}

void test_string_literal() {
    const char *s = "hello";  // String literal initialization is OK
}

void test_array_reference() {
    int arr[10];
    int (&ref)[10] = arr;  // Reference to array is OK
}
