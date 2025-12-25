// RUN: %check_clang_tidy %s automotive-cpp23-req-8.8.1 %t -- -- -std=c++11
// Test for automotive-cpp23-req-8.8.1
// Related MISRA C++:2023 Rule: 8.8.1

// This test verifies that bitwise operations on signed operands are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_signed_bitwise_and() {
    int x = 10;
    int y = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bitwise operation '&' performed on signed operands
    int result = x & y;

    signed int sx = 10;
    signed int sy = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bitwise operation '&' performed on signed operands
    signed int sr = sx & sy;
}

void test_signed_bitwise_or() {
    int x = 10;
    int y = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bitwise operation '|' performed on signed operands
    int result = x | y;
}

void test_signed_bitwise_xor() {
    int x = 10;
    int y = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bitwise operation '^' performed on signed operands
    int result = x ^ y;
}

void test_signed_left_shift() {
    int x = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bitwise operation '<<' performed on signed operands
    int result = x << 2;

    // Negative value shifted (undefined behavior)
    int neg = -1;
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: bitwise operation '<<' performed on signed operands
    int negShift = neg << 1;
}

void test_signed_right_shift() {
    int x = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bitwise operation '>>' performed on signed operands
    int result = x >> 2;

    // Implementation-defined behavior with negative values
    int neg = -8;
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: bitwise operation '>>' performed on signed operands
    int negShift = neg >> 1;
}

void test_signed_bitwise_not() {
    int x = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bitwise NOT operation '~' performed on signed operand
    int result = ~x;

    signed int sx = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bitwise NOT operation '~' performed on signed operand
    signed int sr = ~sx;
}

void test_compound_assignments() {
    int x = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: bitwise operation '&=' performed on signed operands
    x &= 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: bitwise operation '|=' performed on signed operands
    x |= 3;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: bitwise operation '^=' performed on signed operands
    x ^= 7;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: bitwise operation '<<=' performed on signed operands
    x <<= 2;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: bitwise operation '>>=' performed on signed operands
    x >>= 1;
}

void test_mixed_signed_unsigned() {
    int x = 10;
    unsigned int y = 5;

    // One operand is signed
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bitwise operation '&' performed with signed left operand
    int result1 = x & y;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bitwise operation '|' performed with signed right operand
    int result2 = y | x;
}

void test_signed_long_types() {
    long x = 100L;
    long y = 50L;

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: bitwise operation '&' performed on signed operands
    long result = x & y;

    long long xx = 1000LL;
    long long yy = 500LL;
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: bitwise operation '|' performed on signed operands
    long long result2 = xx | yy;
}

void test_signed_short_types() {
    short x = 10;
    short y = 5;

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: bitwise operation '&' performed on signed operands
    int result = x & y;  // Integral promotion to int
}

void test_negative_values() {
    int x = -1;
    int y = -2;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: bitwise operation '&' performed on signed operands
    int result = x & y;

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bitwise NOT operation '~' performed on signed operand
    int notResult = ~x;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_unsigned_operations() {
    unsigned int x = 10;
    unsigned int y = 5;

    // All operands are unsigned - compliant
    unsigned int result1 = x & y;
    unsigned int result2 = x | y;
    unsigned int result3 = x ^ y;
    unsigned int result4 = x << 2;
    unsigned int result5 = x >> 1;
    unsigned int result6 = ~x;

    // Compound assignments with unsigned
    x &= 5;
    x |= 3;
    x ^= 7;
    x <<= 2;
    x >>= 1;
}

void test_unsigned_long_types() {
    unsigned long x = 100UL;
    unsigned long y = 50UL;

    unsigned long result = x & y;

    unsigned long long xx = 1000ULL;
    unsigned long long yy = 500ULL;
    unsigned long long result2 = xx | yy;
}

void test_unsigned_short_types() {
    unsigned short x = 10;
    unsigned short y = 5;

    // Even though promoted to int, the types themselves are unsigned
    unsigned int result = x & y;
}

void test_logical_operations() {
    int x = 10;
    int y = 5;

    // Logical operators are allowed (not bitwise)
    bool result1 = x && y;
    bool result2 = x || y;
    bool result3 = !x;
}

void test_arithmetic_operations() {
    int x = 10;
    int y = 5;

    // Arithmetic operations are allowed
    int result1 = x + y;
    int result2 = x - y;
    int result3 = x * y;
    int result4 = x / y;
    int result5 = x % y;
}

void test_relational_operations() {
    int x = 10;
    int y = 5;

    // Relational operators are allowed
    bool result1 = x < y;
    bool result2 = x > y;
    bool result3 = x <= y;
    bool result4 = x >= y;
    bool result5 = x == y;
    bool result6 = x != y;
}

// Enum types should not trigger warnings (used for flags)
enum Flags {
    FLAG_NONE = 0,
    FLAG_READ = 1,
    FLAG_WRITE = 2,
    FLAG_EXEC = 4
};

void test_enum_operations() {
    Flags f1 = FLAG_READ;
    Flags f2 = FLAG_WRITE;

    // Enums are not flagged (common pattern for flags)
    int combined = f1 | f2;
    int masked = f1 & FLAG_EXEC;
}
