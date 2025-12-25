// RUN: %check_clang_tidy %s automotive-cpp23-req-6.0.4 %t -- -- -std=c++11
// Test for automotive-cpp23-req-6.0.4
// Related MISRA C++:2023 Rule: 6.0.4

// This test verifies that unary minus operator applied to unsigned types
// is detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_unary_minus_unsigned_int() {
    unsigned int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int y = -x;

    unsigned int a = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    unsigned int b = -a;
}

void test_unary_minus_uint32() {
    unsigned int val = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    auto neg = -val;
}

void test_unary_minus_unsigned_long() {
    unsigned long x = 100UL;
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    unsigned long y = -x;
}

void test_unary_minus_unsigned_long_long() {
    unsigned long long x = 1000ULL;
    // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    unsigned long long y = -x;
}

void test_unary_minus_unsigned_short() {
    unsigned short x = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int y = -x;
}

void test_unary_minus_unsigned_char() {
    unsigned char x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int y = -x;
}

void test_unary_minus_size_t() {
    typedef unsigned long size_t;
    size_t s = 100;
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    size_t neg = -s;
}

void test_unary_minus_in_expression() {
    unsigned int x = 10;
    unsigned int y = 20;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int result = -x + y;

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int result2 = x + -y;
}

void test_unary_minus_with_const() {
    const unsigned int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int y = -x;
}

void test_unary_minus_with_volatile() {
    volatile unsigned int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int y = -x;
}

void test_unary_minus_return() {
    unsigned int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    return -x;
}

void test_unary_minus_in_function_call() {
    void process(int);
    unsigned int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    process(-x);
}

void test_unary_minus_in_array_subscript() {
    int arr[10];
    unsigned int idx = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int val = arr[-idx];  // Also dangerous for array access!
}

void test_unary_minus_with_parentheses() {
    unsigned int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int y = -(x);

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: unary minus operator shall not be applied to an expression whose underlying type is unsigned
    int z = -((x));
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_unary_minus_signed_int() {
    int x = 5;
    int y = -x;  // OK - signed type

    signed int a = 10;
    signed int b = -a;  // OK - signed type
}

void test_unary_minus_signed_long() {
    long x = 100L;
    long y = -x;  // OK - signed type

    long long xx = 1000LL;
    long long yy = -xx;  // OK - signed type
}

void test_unary_minus_signed_short() {
    short x = 10;
    short y = -x;  // OK - signed type
}

void test_unary_minus_signed_char() {
    signed char x = 5;
    signed char y = -x;  // OK - signed type
}

void test_unary_minus_float() {
    float x = 5.0f;
    float y = -x;  // OK - floating point type
}

void test_unary_minus_double() {
    double x = 5.0;
    double y = -x;  // OK - floating point type
}

void test_unary_minus_literal() {
    int x = -5;  // OK - literal constant
    unsigned int y = -10;  // OK - literal constant, not unary on unsigned variable
}

void test_explicit_cast_then_negate() {
    unsigned int u = 5;
    int y = -static_cast<int>(u);  // OK - explicit cast to signed first

    unsigned int u2 = 10;
    int y2 = -(int)u2;  // OK - C-style cast to signed first (though C-style cast may violate other rules)
}

void test_binary_minus() {
    unsigned int x = 10;
    unsigned int y = 5;
    unsigned int result = x - y;  // OK - binary subtraction, not unary minus
}

void test_subtraction_from_zero() {
    unsigned int x = 5;
    int y = 0 - x;  // OK - binary subtraction (though semantically similar)
}

void test_const_signed() {
    const int x = 5;
    int y = -x;  // OK - const qualified signed type
}

void test_volatile_signed() {
    volatile int x = 5;
    int y = -x;  // OK - volatile qualified signed type
}

// Enums should not trigger warnings even if they have unsigned underlying type
enum UnsignedEnum : unsigned int {
    VALUE1 = 1,
    VALUE2 = 2
};

void test_enum() {
    UnsignedEnum e = VALUE1;
    // Enums are excluded from the check
    int val = -e;  // OK - enum type (not directly unsigned)
}

void test_typedef_signed() {
    typedef int MyInt;
    MyInt x = 5;
    MyInt y = -x;  // OK - typedef of signed type
}

void test_multiple_operations() {
    int x = 5;
    int y = 3;
    int result = -x + -y;  // OK - both are signed
    int result2 = -(x + y);  // OK - signed expression
}

void test_pointer_arithmetic() {
    int arr[10];
    int *ptr = arr + 5;
    int *ptr2 = ptr - 3;  // OK - binary subtraction in pointer arithmetic
}

void test_negation_of_signed_in_unsigned_context() {
    int x = 5;
    unsigned int y = -x;  // OK - negating a signed value (result assigned to unsigned)
}
