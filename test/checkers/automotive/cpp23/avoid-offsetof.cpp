// RUN: %check_clang_tidy %s automotive-cpp23-req-21.2.4 %t -- -- -std=c++11

// Test for automotive-cpp23-req-21.2.4: offsetof prohibition
// Related MISRA C++:2023 Rule: 21.2.4
//
// The offsetof macro shall not be used in C++.

// Forward declarations to simulate <cstddef>
typedef unsigned long size_t;

// Define offsetof macro for testing (simulates the standard macro)
#define offsetof(type, member) __builtin_offsetof(type, member)

struct StandardLayout {
    int a;
    int b;
    double c;
};

struct NonStandardLayout {
    int a;
private:
    int b;
public:
    virtual void foo() {}
};

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_offsetof_usage() {
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: the macro offsetof shall not be used [automotive-cpp23-req-21.2.4]
    size_t offset1 = offsetof(StandardLayout, a);
    (void)offset1;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: the macro offsetof shall not be used [automotive-cpp23-req-21.2.4]
    size_t offset2 = offsetof(StandardLayout, b);
    (void)offset2;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: the macro offsetof shall not be used [automotive-cpp23-req-21.2.4]
    size_t offset3 = offsetof(StandardLayout, c);
    (void)offset3;

    // Even with non-standard-layout types (which would be UB)
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: the macro offsetof shall not be used [automotive-cpp23-req-21.2.4]
    size_t offset4 = offsetof(NonStandardLayout, a);
    (void)offset4;
}

void test_offsetof_in_expression() {
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: the macro offsetof shall not be used [automotive-cpp23-req-21.2.4]
    if (sizeof(int) < offsetof(StandardLayout, b)) {
        // Some code
    }

    // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: the macro offsetof shall not be used [automotive-cpp23-req-21.2.4]
    size_t array[1] = {offsetof(StandardLayout, c)};
    (void)array;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Alternative: use member pointers or other C++ mechanisms
void compliant_alternative() {
    // Instead of offsetof, use proper C++ constructs
    StandardLayout s{};

    // Use pointer arithmetic with member pointers (C++ way)
    auto ptr_to_b = &StandardLayout::b;
    (void)ptr_to_b;

    // Or use direct member access
    int *b_addr = &s.b;
    (void)b_addr;
}
