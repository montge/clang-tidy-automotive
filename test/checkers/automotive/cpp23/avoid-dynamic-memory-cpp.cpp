// RUN: %check_clang_tidy %s automotive-cpp23-req-21.6 %t
// Test for automotive-cpp23-req-21.6: Dynamic memory prohibition
// Related MISRA C++:2023 Rule: 21.6

#include <memory>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_new_expression() {
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: dynamic memory allocation shall not be used [automotive-cpp23-req-21.6]
    int *p = new int;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: dynamic memory deallocation shall not be used [automotive-cpp23-req-21.6]
    delete p;
}

void test_new_array() {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: dynamic memory allocation shall not be used [automotive-cpp23-req-21.6]
    int *arr = new int[10];
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: dynamic memory deallocation shall not be used [automotive-cpp23-req-21.6]
    delete[] arr;
}

class MyClass {
public:
    int value;
};

void test_new_class() {
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: dynamic memory allocation shall not be used [automotive-cpp23-req-21.6]
    MyClass *obj = new MyClass();
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: dynamic memory deallocation shall not be used [automotive-cpp23-req-21.6]
    delete obj;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_stack_allocation() {
    int x = 42;
    int arr[10] = {};
    MyClass obj;
    (void)x;
    (void)arr;
    (void)obj;
}

// Static allocation
static int static_array[100];

// Placement new in pre-allocated buffer (if allowed by project)
// Note: Some safety-critical projects allow placement new

void test_containers() {
    // Note: Standard containers use dynamic memory internally
    // In safety-critical systems, custom allocators with static pools
    // or fixed-size containers are typically used
}
