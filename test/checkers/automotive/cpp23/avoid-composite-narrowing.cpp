// RUN: %check_clang_tidy %s automotive-cpp23-req-10.4 %t -- -- -std=c++17
// Test for automotive-cpp23-req-10.4
// Related MISRA C++:2023 Rule: 10.4

// This test verifies that composite expressions assigned to narrower types
// are detected.

#include <cstdint>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_int32_to_int16() {
    int32_t a = 1000;
    int32_t b = 2000;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a + b;
}

void test_int64_to_int32() {
    int64_t x = 100000;
    int64_t y = 200000;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'long{{( long)?}}' shall not be assigned to narrower type 'int'
    int32_t result = x + y;
}

void test_subtraction_narrowing() {
    int32_t a = 5000;
    int32_t b = 3000;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a - b;
}

void test_multiplication_narrowing() {
    int32_t a = 100;
    int32_t b = 200;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a * b;
}

void test_division_narrowing() {
    int32_t a = 10000;
    int32_t b = 50;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a / b;
}

void test_modulo_narrowing() {
    int32_t a = 10000;
    int32_t b = 97;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a % b;
}

void test_bitwise_and_narrowing() {
    int32_t a = 0xFFFF0000;
    int32_t b = 0x0000FFFF;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a & b;
}

void test_bitwise_or_narrowing() {
    int32_t a = 0x1234;
    int32_t b = 0x5678;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a | b;
}

void test_bitwise_xor_narrowing() {
    int32_t a = 0xAAAA;
    int32_t b = 0x5555;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a ^ b;
}

void test_left_shift_narrowing() {
    int32_t a = 100;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a << 2;
}

void test_right_shift_narrowing() {
    int32_t a = 10000;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a >> 2;
}

void test_assignment_operator() {
    int32_t a = 1000;
    int32_t b = 2000;
    int16_t result = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    result = a + b;
}

void test_unsigned_to_signed_narrowing() {
    uint32_t a = 1000U;
    uint32_t b = 2000U;
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: composite expression result of type 'unsigned int' shall not be assigned to narrower type 'int'
    int32_t result = a + b;
}

void test_signed_to_unsigned_narrowing() {
    int32_t a = 1000;
    int32_t b = 2000;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: composite expression result of type 'int' shall not be assigned to narrower type 'unsigned int'
    uint32_t result = a + b;
}

void test_float_to_int_narrowing() {
    float a = 3.14f;
    float b = 2.71f;
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: composite expression result of type 'float' shall not be assigned to narrower type 'int'
    int result = a + b;
}

void test_double_to_float_narrowing() {
    double a = 3.14159;
    double b = 2.71828;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'double' shall not be assigned to narrower type 'float'
    float result = a + b;
}

void test_long_to_short_narrowing() {
    long a = 100000L;
    long b = 200000L;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'long{{( long)?}}' shall not be assigned to narrower type 'short'
    short result = a + b;
}

void test_uint64_to_uint32_narrowing() {
    uint64_t a = 1000000ULL;
    uint64_t b = 2000000ULL;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: composite expression result of type 'unsigned long{{( long)?}}' shall not be assigned to narrower type 'unsigned int'
    uint32_t result = a + b;
}

void test_with_parentheses() {
    int32_t a = 1000;
    int32_t b = 2000;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = (a + b);
}

void test_unsigned_char_from_int() {
    int a = 100;
    int b = 50;
    // CHECK-MESSAGES: :[[@LINE+1]]:28: warning: composite expression result of type 'int' shall not be assigned to narrower type 'unsigned char'
    unsigned char result = a + b;
}

void test_int8_from_int16() {
    int16_t a = 100;
    int16_t b = 50;
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: composite expression result of type 'short' shall not be assigned to narrower type '{{(signed )?}}char'
    int8_t result = a + b;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_same_type_int32() {
    int32_t a = 1000;
    int32_t b = 2000;
    int32_t result = a + b;  // OK: same type
}

void test_same_type_int16() {
    int16_t a = 100;
    int16_t b = 200;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    int16_t result = a + b;  // WARNING: int16_t operands are promoted to int in arithmetic
}

void test_wider_type() {
    int16_t a = 100;
    int16_t b = 200;
    int32_t result = a + b;  // OK: assigning to wider type
}

void test_explicit_cast() {
    int32_t a = 1000;
    int32_t b = 2000;
    int16_t result = static_cast<int16_t>(a + b);  // OK: explicit cast
}

void test_c_style_cast() {
    int32_t a = 1000;
    int32_t b = 2000;
    int16_t result = (int16_t)(a + b);  // OK: explicit cast (though C-style cast may violate other rules)
}

void test_simple_assignment_no_operation() {
    int32_t a = 1000;
    int16_t result = a;  // OK: not a composite expression (single operand)
}

void test_comparison_not_composite() {
    int32_t a = 1000;
    int32_t b = 2000;
    bool result = a > b;  // OK: comparison, not arithmetic composite
}

void test_logical_and_not_composite() {
    bool a = true;
    bool b = false;
    bool result = a && b;  // OK: logical operation, not arithmetic composite
}

void test_logical_or_not_composite() {
    bool a = true;
    bool b = false;
    bool result = a || b;  // OK: logical operation, not arithmetic composite
}

void test_same_signedness_same_size() {
    int32_t a = 1000;
    int32_t b = 2000;
    int32_t result = a + b;  // OK: exact match
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

void test_increment_decrement() {
    int32_t a = 1000;
    int16_t result = a++;  // OK: not a binary operation
}

void test_unary_operations() {
    int32_t a = 1000;
    int16_t result = -a;  // OK: not a composite (binary) expression
}

void test_ternary_not_composite() {
    int32_t a = 1000;
    int32_t b = 2000;
    bool condition = true;
    int16_t result = condition ? a : b;  // OK: ternary is not a composite expression in this check
}

void test_function_call_not_composite() {
    int32_t getValue();
    int16_t result = getValue();  // OK: function call, not composite expression
}

void test_uint32_same_type() {
    uint32_t a = 1000U;
    uint32_t b = 2000U;
    uint32_t result = a + b;  // OK: same type
}

void test_int64_same_type() {
    int64_t a = 1000LL;
    int64_t b = 2000LL;
    int64_t result = a + b;  // OK: same type
}

void test_short_addition() {
    short a = 100;
    short b = 200;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: composite expression result of type 'int' shall not be assigned to narrower type 'short'
    short result = a + b;  // WARNING: short operands are promoted to int in arithmetic
}

void test_unsigned_short_addition() {
    unsigned short a = 100;
    unsigned short b = 200;
    // CHECK-MESSAGES: :[[@LINE+1]]:29: warning: composite expression result of type 'int' shall not be assigned to narrower type 'unsigned short'
    unsigned short result = a + b;  // WARNING: unsigned short promoted to int
}

void test_widening_to_int64() {
    int32_t a = 1000;
    int32_t b = 2000;
    int64_t result = a + b;  // OK: widening
}

void test_widening_to_double() {
    float a = 3.14f;
    float b = 2.71f;
    double result = a + b;  // OK: widening
}

void test_compound_assignment_operators() {
    int32_t a = 1000;
    a += 500;  // OK: not assignment of composite expression to different variable
}

void test_array_subscript_not_composite() {
    int arr[10] = {0};
    int idx = 5;
    int result = arr[idx];  // OK: not a composite expression
}

void test_pointer_arithmetic_not_composite() {
    int arr[10];
    int *ptr = arr + 5;  // OK: pointer arithmetic handled differently
}

void test_member_access_not_composite() {
    struct S { int value; };
    S s;
    int result = s.value;  // OK: member access, not composite
}

void test_same_size_uint_to_int_allowed() {
    // Note: This tests an edge case - different signedness of same size
    // Based on the implementation, this SHOULD trigger a warning
}

void test_explicit_narrowing_with_cast() {
    int64_t a = 100000LL;
    int64_t b = 200000LL;
    int32_t result = static_cast<int32_t>(a + b);  // OK: explicit cast shows intent
}
