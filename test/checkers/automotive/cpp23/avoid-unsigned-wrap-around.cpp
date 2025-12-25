// RUN: %check_clang_tidy %s automotive-cpp23-req-7.2 %t -- -- -std=c++11
// Test for automotive-cpp23-req-7.2
// Related MISRA C++:2023 Rule: 7.2 - Unsigned integer operations shall not wrap around

// This test verifies that unsigned integer operations that may cause wrap-around
// are detected, particularly subtraction and operations near maximum values.

#include <climits>

//===----------------------------------------------------------------------===//
// Violation Cases - Subtraction (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_subtraction_basic() {
    unsigned int a = 10;
    unsigned int b = 20;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer subtraction '-' may wrap around
    unsigned int c = a - b;  // May underflow
}

void test_subtraction_variables() {
    unsigned int x = 5;
    unsigned int y = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unsigned integer subtraction '-' may wrap around
    unsigned int z = x - y;
}

void test_subtraction_unsigned_long() {
    unsigned long a = 100UL;
    unsigned long b = 200UL;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer subtraction '-' may wrap around
    unsigned long c = a - b;
}

void test_subtraction_unsigned_short() {
    unsigned short a = 10;
    unsigned short b = 20;
    // Result is promoted to int, but operation still flagged
    auto c = a - b;
}

void test_subtraction_size_t() {
    typedef unsigned long size_t;
    size_t a = 10;
    size_t b = 20;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unsigned integer subtraction '-' may wrap around
    size_t c = a - b;
}

void test_subtraction_in_expression() {
    unsigned int x = 5;
    unsigned int y = 10;
    unsigned int z = 15;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer subtraction '-' may wrap around
    unsigned int res = (x - y) + z;
}

void test_subtraction_chain() {
    unsigned int a = 10;
    unsigned int b = 5;
    unsigned int c = 3;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unsigned integer subtraction '-' may wrap around
    // CHECK-MESSAGES: :[[@LINE+2]]:22: warning: unsigned integer subtraction '-' may wrap around
    unsigned int d = a - b - c;
}

void test_subtraction_with_const() {
    const unsigned int a = 10;
    unsigned int b = 20;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer subtraction '-' may wrap around
    unsigned int c = a - b;
}

void test_subtraction_with_volatile() {
    volatile unsigned int a = 10;
    unsigned int b = 20;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer subtraction '-' may wrap around
    unsigned int c = a - b;
}

//===----------------------------------------------------------------------===//
// Violation Cases - Addition (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_addition_basic() {
    unsigned int a = UINT_MAX - 10;
    unsigned int b = 20;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer addition '+' may wrap around
    unsigned int c = a + b;  // May overflow
}

void test_addition_variables() {
    unsigned int x = 1000000;
    unsigned int y = 2000000;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer addition '+' may wrap around
    unsigned int z = x + y;
}

void test_addition_large_values() {
    unsigned long a = ULONG_MAX;
    unsigned long b = 1UL;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer addition '+' may wrap around
    unsigned long c = a + b;
}

void test_addition_in_expression() {
    unsigned int x = 100;
    unsigned int y = 200;
    unsigned int z = 300;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer addition '+' may wrap around
    // CHECK-MESSAGES: :[[@LINE+2]]:27: warning: unsigned integer addition '+' may wrap around
    unsigned int res = (x + y) + z;
}

//===----------------------------------------------------------------------===//
// Violation Cases - Multiplication (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_multiplication_basic() {
    unsigned int a = UINT_MAX / 2;
    unsigned int b = 3;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer multiplication '*' may wrap around
    unsigned int c = a * b;  // May overflow
}

void test_multiplication_variables() {
    unsigned int x = 1000000;
    unsigned int y = 5000;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer multiplication '*' may wrap around
    unsigned int z = x * y;
}

void test_multiplication_large_values() {
    unsigned long a = ULONG_MAX / 10;
    unsigned long b = 20UL;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer multiplication '*' may wrap around
    unsigned long c = a * b;
}

void test_multiplication_chain() {
    unsigned int a = 100;
    unsigned int b = 200;
    unsigned int c = 300;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unsigned integer multiplication '*' may wrap around
    // CHECK-MESSAGES: :[[@LINE+2]]:22: warning: unsigned integer multiplication '*' may wrap around
    unsigned int d = a * b * c;
}

//===----------------------------------------------------------------------===//
// Violation Cases - Compound Assignment (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_compound_subtract() {
    unsigned int a = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: unsigned integer compound assignment '-=' may cause wrap-around
    a -= 20;
}

void test_compound_subtract_variable() {
    unsigned int x = 5;
    unsigned int y = 15;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: unsigned integer compound assignment '-=' may cause wrap-around
    x -= y;
}

void test_compound_add() {
    unsigned int a = UINT_MAX - 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: unsigned integer compound assignment '+=' may cause wrap-around
    a += 20;
}

void test_compound_add_variable() {
    unsigned int x = 1000000;
    unsigned int y = 2000000;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: unsigned integer compound assignment '+=' may cause wrap-around
    x += y;
}

void test_compound_multiply() {
    unsigned int a = UINT_MAX / 2;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: unsigned integer compound assignment '*=' may cause wrap-around
    a *= 3;
}

void test_compound_multiply_variable() {
    unsigned int x = 100000;
    unsigned int y = 50000;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: unsigned integer compound assignment '*=' may cause wrap-around
    x *= y;
}

void test_loop_with_compound_subtract() {
    for (unsigned int i = 100; i > 0; ) {
        // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: unsigned integer compound assignment '-=' may cause wrap-around
        i -= 5;
    }
}

//===----------------------------------------------------------------------===//
// Violation Cases - Mixed Operations
//===----------------------------------------------------------------------===//

void test_mixed_operations() {
    unsigned int a = 100;
    unsigned int b = 200;
    unsigned int c = 50;

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unsigned integer subtraction '-' may wrap around
    // CHECK-MESSAGES: :[[@LINE+2]]:24: warning: unsigned integer addition '+' may wrap around
    unsigned int d = (a - b) + c;

    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer multiplication '*' may wrap around
    // CHECK-MESSAGES: :[[@LINE+2]]:27: warning: unsigned integer subtraction '-' may wrap around
    unsigned int e = (a * b) - c;
}

void test_nested_expressions() {
    unsigned int a = 10;
    unsigned int b = 20;
    unsigned int c = 30;
    unsigned int d = 40;

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unsigned integer subtraction '-' may wrap around
    // CHECK-MESSAGES: :[[@LINE+2]]:28: warning: unsigned integer subtraction '-' may wrap around
    // CHECK-MESSAGES: :[[@LINE+3]]:32: warning: unsigned integer addition '+' may wrap around
    unsigned int e = (a - b) + (c - d) + 50;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_signed_subtraction() {
    int a = 10;
    int b = 20;
    int c = a - b;  // OK - signed arithmetic
}

void test_signed_addition() {
    int a = 100;
    int b = 200;
    int c = a + b;  // OK - signed arithmetic
}

void test_signed_multiplication() {
    int a = 1000;
    int b = 2000;
    int c = a * b;  // OK - signed arithmetic
}

void test_subtraction_with_guard() {
    unsigned int a = 20;
    unsigned int b = 10;
    // Even with guard, the check still flags (static analysis limitation)
    if (a >= b) {
        // CHECK-MESSAGES: :[[@LINE+1]]:27: warning: unsigned integer subtraction '-' may wrap around
        unsigned int c = a - b;  // Still flagged - tool cannot verify guard
    }
}

void test_constant_expressions() {
    // Constant expressions are not flagged (compiler handles these)
    unsigned int a = 100 - 50;  // OK - compile-time constant
    unsigned int b = 200 + 300;  // OK - compile-time constant
    unsigned int c = 10 * 20;   // OK - compile-time constant
}

void test_literal_operations() {
    unsigned int a = 10;
    unsigned int b = a - 5;  // Still flagged - not both constants
    // CHECK-MESSAGES: :[[@LINE-1]]:23: warning: unsigned integer subtraction '-' may wrap around
}

void test_float_operations() {
    float a = 10.0f;
    float b = 20.0f;
    float c = a - b;  // OK - floating point
}

void test_double_operations() {
    double a = 100.0;
    double b = 200.0;
    double c = a - b;  // OK - double precision
}

void test_signed_compound_assignment() {
    int a = 10;
    a -= 20;  // OK - signed type
    a += 30;  // OK - signed type
    a *= 2;   // OK - signed type
}

void test_explicit_cast_to_signed() {
    unsigned int a = 10;
    unsigned int b = 20;
    int c = static_cast<int>(a) - static_cast<int>(b);  // OK - signed after cast
}

void test_mixed_signed_unsigned() {
    int a = 10;
    unsigned int b = 20;
    // Result type depends on promotion rules, but if result is signed, OK
    auto c = a - static_cast<int>(b);
}

void test_bitwise_operations() {
    unsigned int a = 10;
    unsigned int b = 20;
    unsigned int c = a & b;  // OK - bitwise AND (not arithmetic)
    unsigned int d = a | b;  // OK - bitwise OR (not arithmetic)
    unsigned int e = a ^ b;  // OK - bitwise XOR (not arithmetic)
}

void test_shift_operations() {
    unsigned int a = 10;
    unsigned int b = a << 2;  // OK - shift operations (not arithmetic)
    unsigned int c = a >> 1;  // OK - shift operations (not arithmetic)
}

void test_comparison_operations() {
    unsigned int a = 10;
    unsigned int b = 20;
    bool c = a < b;   // OK - comparison (not arithmetic)
    bool d = a == b;  // OK - equality (not arithmetic)
}

void test_increment_decrement() {
    unsigned int a = 10;
    a++;  // OK - increment (not flagged by this check)
    ++a;  // OK - pre-increment (not flagged by this check)
    a--;  // OK - decrement (not flagged by this check)
    --a;  // OK - pre-decrement (not flagged by this check)
}

void test_compound_zero_subtract() {
    unsigned int a = 10;
    a -= 0;  // OK - subtracting zero is safe
}

void test_division_modulo() {
    unsigned int a = 100;
    unsigned int b = 20;
    unsigned int c = a / b;  // OK - division (not flagged)
    unsigned int d = a % b;  // OK - modulo (not flagged)
}

void test_pointer_arithmetic() {
    int arr[10];
    int *ptr = arr + 5;  // OK - pointer arithmetic
    int *ptr2 = ptr - 2; // OK - pointer arithmetic
}

void test_enum_operations() {
    enum UnsignedEnum : unsigned int {
        VALUE1 = 10,
        VALUE2 = 20
    };

    // Enum operations may still be flagged depending on promotion
    UnsignedEnum a = VALUE1;
    UnsignedEnum b = VALUE2;
    // Enums typically promote to int for arithmetic
}

void test_sizeof_operations() {
    int arr[10];
    unsigned int size = sizeof(arr);  // OK - sizeof expression
    unsigned int elem_size = sizeof(int);  // OK - sizeof expression
}

void test_bool_operations() {
    bool a = true;
    bool b = false;
    // Bool operations are not flagged (different check)
}

// Test with different unsigned types
void test_unsigned_char_operations() {
    unsigned char a = 10;
    unsigned char b = 20;
    // Result is promoted to int, so not flagged as unsigned result
    auto c = a - b;  // Promoted to int
}

void test_unsigned_short_operations() {
    unsigned short a = 100;
    unsigned short b = 200;
    // Result is promoted to int, so not flagged as unsigned result
    auto c = a - b;  // Promoted to int
}

// Edge cases
void test_zero_operations() {
    unsigned int a = 0;
    unsigned int b = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer subtraction '-' may wrap around
    unsigned int c = a - b;  // Wraps to UINT_MAX - 9
}

void test_max_value_operations() {
    unsigned int a = UINT_MAX;
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: unsigned integer addition '+' may wrap around
    unsigned int b = a + 1;  // Wraps to 0
}
