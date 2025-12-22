// RUN: %check_clang_tidy %s automotive-cpp23-req-7.11 %t -- -- -std=c++11

// Test: Null pointer literal assignment (MISRA C++:2023 Rule 7.11)
// Check ID: automotive-cpp23-req-7.11

// ============= Compliant: Using nullptr =============

int* good_ptr = nullptr;
char* good_char_ptr = nullptr;
void* good_void_ptr = nullptr;

struct Foo {
    int* member = nullptr;
};

void compliant_examples() {
    int* p = nullptr;
    p = nullptr;

    double* dp = nullptr;

    // Function returning pointer
    auto f = []() -> int* { return nullptr; };
}

void takes_ptr(int* p);
void call_with_nullptr() {
    takes_ptr(nullptr);
}

// ============= Violations: Using literal 0 =============

// CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use 'nullptr' instead of literal '0' when initializing pointer
int* bad_ptr = 0;

// CHECK-MESSAGES: :[[@LINE+1]]:18: warning: use 'nullptr' instead of literal '0' when initializing pointer
char* bad_char = 0;

// CHECK-MESSAGES: :[[@LINE+1]]:18: warning: use 'nullptr' instead of literal '0' when initializing pointer
void* bad_void = 0;

void non_compliant_assignments() {
    int* p = nullptr;
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: use 'nullptr' instead of literal '0' when assigning to pointer
    p = 0;

    double* dp = nullptr;
    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: use 'nullptr' instead of literal '0' when assigning to pointer
    dp = 0;
}

int* return_zero() {
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: use 'nullptr' instead of literal '0' when returning from pointer function
    return 0;
}

void call_with_zero() {
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: use 'nullptr' instead of literal '0' when passing to pointer parameter
    takes_ptr(0);
}

// ============= Edge cases =============

// Non-pointer integer initialization with 0 is fine
int regular_int = 0;
long regular_long = 0L;

// Pointer arithmetic is fine (this is a different value)
void pointer_arithmetic() {
    int arr[10];
    int* p = arr + 0;  // This is pointer arithmetic, not null assignment
}
