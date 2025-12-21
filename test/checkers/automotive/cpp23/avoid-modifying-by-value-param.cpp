// RUN: %check_clang_tidy %s automotive-cpp23-req-13.3 %t
// Test for automotive-cpp23-req-13.3
// Related MISRA C++:2023 Rule: 13.3

// This test verifies that modifications to by-value parameters are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_assignment(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: assignment of parameter 'x' passed by value
    x = 10;
}

void test_compound_assignment(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: assignment of parameter 'x' passed by value
    x += 5;
}

void test_increment(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: increment of parameter 'x' passed by value
    x++;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: increment of parameter 'x' passed by value
    ++x;
}

void test_decrement(int x) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: decrement of parameter 'x' passed by value
    x--;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: decrement of parameter 'x' passed by value
    --x;
}

void test_multiple_params(int a, int b) {
    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: assignment of parameter 'a' passed by value
    a = 1;
    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: assignment of parameter 'b' passed by value
    b = 2;
}

struct Point {
    int x, y;
};

void test_struct_param(Point p) {
    // Modifying member of by-value struct - not currently detected
    // (limitation: only direct parameter modifications are caught)
    p.x = 10;
    (void)p;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_reference_param(int& x) {
    // Reference parameter - modification is intentional
    x = 10;
    x++;
}

void test_pointer_param(int* p) {
    // Pointer parameter - modification of pointee is intentional
    *p = 10;
    (*p)++;
}

void test_const_ref_param(const int& x) {
    // Can't modify const ref anyway
    int y = x + 1;
    (void)y;
}

void test_read_only(int x) {
    // Using but not modifying - no warning
    int y = x + 1;
    int z = x * 2;
    (void)y;
    (void)z;
}

void test_local_variable_modification() {
    int x = 0;
    // Modifying local variable, not a parameter - no warning
    x = 10;
    x++;
}

void test_output_pointer_param(int* out) {
    // Modifying the pointer itself is allowed (by-value pointer)
    // But this pattern is unusual
    int y;
    out = &y;  // Modifying pointer value (harmless but pointless)
    (void)out;
}
