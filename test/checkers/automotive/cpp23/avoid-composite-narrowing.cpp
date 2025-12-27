// RUN: %check_clang_tidy %s automotive-cpp23-req-10.4 %t -- -- -std=c++17
// Test for automotive-cpp23-req-10.4
// Related MISRA C++:2023 Rule: 10.4

// This test verifies that composite expressions assigned to narrower types
// are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_long_to_int() {
    long a = 100000L;
    long b = 200000L;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: composite expression result of type 'long' shall not be assigned to narrower type 'int'
    int result = a + b;
}

void test_subtraction_narrowing() {
    long a = 5000L;
    long b = 3000L;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'long' shall not be assigned to narrower type 'short'
    short result = a - b;
}

void test_multiplication_narrowing() {
    int a = 100;
    int b = 200;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = a * b;
}

void test_division_narrowing() {
    int a = 10000;
    int b = 50;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = a / b;
}

void test_modulo_narrowing() {
    int a = 10000;
    int b = 97;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = a % b;
}

void test_bitwise_and_narrowing() {
    int a = 0x7FFF0000;
    int b = 0x0000FFFF;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = a & b;
}

void test_bitwise_or_narrowing() {
    int a = 0x1234;
    int b = 0x5678;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = a | b;
}

void test_left_shift_narrowing() {
    int a = 100;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = a << 2;
}

void test_right_shift_narrowing() {
    int a = 10000;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = a >> 2;
}

void test_assignment_operator() {
    int a = 1000;
    int b = 2000;
    short result = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    result = a + b;
}

void test_float_to_int_narrowing() {
    float a = 3.14f;
    float b = 2.71f;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: composite expression result of type 'float' shall not be assigned to narrower type 'int'
    int result = a + b;
}

void test_double_to_float_narrowing() {
    double a = 3.14159;
    double b = 2.71828;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'double' shall not be assigned to narrower type 'float'
    float result = a + b;
}

void test_with_parentheses() {
    int a = 1000;
    int b = 2000;
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = (a + b);
}

void test_unsigned_char_from_int() {
    int a = 100;
    int b = 50;
    // CHECK-MESSAGES: :[[@LINE+1]]:28: warning: composite expression result of type 'int' shall not be assigned to narrower type 'unsigned char'
    unsigned char result = a + b;
}

void test_short_promoted_to_int() {
    short a = 100;
    short b = 200;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = a + b;  // WARNING: short operands are promoted to int in arithmetic
}

void test_unsigned_short_promoted() {
    unsigned short a = 100;
    unsigned short b = 200;
    // CHECK-MESSAGES: :[[@LINE+1]]:29: warning: composite expression result of type 'int' shall not be assigned to narrower type 'unsigned short'
    unsigned short result = a + b;  // WARNING: unsigned short promoted to int
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_same_type_int() {
    int a = 1000;
    int b = 2000;
    int result = a + b;  // OK: same type
}

void test_wider_type() {
    int a = 100;
    int b = 200;
    long result = a + b;  // OK: assigning to wider type
}

void test_explicit_cast() {
    int a = 1000;
    int b = 2000;
    short result = static_cast<short>(a + b);  // OK: explicit cast
}

void test_c_style_cast() {
    int a = 1000;
    int b = 2000;
    short result = (short)(a + b);  // OK: explicit cast
}

void test_simple_assignment_no_operation() {
    int a = 1000;
    short result = a;  // OK: not a composite expression (single operand)
}

void test_comparison_not_composite() {
    int a = 1000;
    int b = 2000;
    bool result = a > b;  // OK: comparison, not arithmetic composite
}

void test_float_addition() {
    float a = 3.14f;
    float b = 2.71f;
    float result = a + b;  // OK: same type
}

void test_double_addition() {
    double a = 3.14159;
    double b = 2.71828;
    double result = a + b;  // OK: same type
}

void test_widening_to_double() {
    float a = 3.14f;
    float b = 2.71f;
    double result = a + b;  // OK: widening
}

void test_compound_assignment_operators() {
    int a = 1000;
    a += 500;  // OK: not assignment of composite expression to different variable
}
