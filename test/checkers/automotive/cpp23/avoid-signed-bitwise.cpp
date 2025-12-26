// RUN: %check_clang_tidy %s automotive-cpp23-req-8.8.1 %t -- -- -std=c++11
// Test for automotive-cpp23-req-8.8.1
// Related MISRA C++:2023 Rule: 8.8.1

// Bitwise operations on signed operands should be avoided.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_signed_bitwise_and() {
    int x = 10;
    int y = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: bitwise operation '&' performed on signed operands
    int result = x & y;
    (void)result;
}

void test_signed_bitwise_or() {
    int x = 10;
    int y = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: bitwise operation '|' performed on signed operands
    int result = x | y;
    (void)result;
}

void test_signed_bitwise_xor() {
    int x = 10;
    int y = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: bitwise operation '^' performed on signed operands
    int result = x ^ y;
    (void)result;
}

void test_signed_left_shift() {
    int x = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: bitwise operation '<<' performed on signed operands
    int result = x << 2;
    (void)result;
}

void test_signed_right_shift() {
    int x = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: bitwise operation '>>' performed on signed operands
    int result = x >> 2;
    (void)result;
}

void test_signed_bitwise_not() {
    int x = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: bitwise NOT operation '~' performed on signed operand
    int result = ~x;
    (void)result;
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
    (void)x;
}

void test_mixed_signed_unsigned() {
    int x = 10;
    unsigned int y = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: bitwise operation '&' performed with signed left operand
    int result1 = x & y;
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: bitwise operation '|' performed with signed right operand
    int result2 = y | x;
    (void)result1;
    (void)result2;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_unsigned_operations() {
    unsigned int x = 10u;
    unsigned int y = 5u;

    // All operands are unsigned - compliant
    unsigned int result1 = x & y;
    unsigned int result2 = x | y;
    unsigned int result3 = x ^ y;
    unsigned int result4 = x << 2u;
    unsigned int result5 = x >> 1u;
    unsigned int result6 = ~x;

    // Compound assignments with unsigned
    x &= 5u;
    x |= 3u;
    x ^= 7u;
    x <<= 2u;
    x >>= 1u;

    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    (void)result5;
    (void)result6;
}

void test_logical_operations() {
    int x = 10;
    int y = 5;

    // Logical operators are allowed (not bitwise)
    bool result1 = x && y;
    bool result2 = x || y;
    bool result3 = !x;

    (void)result1;
    (void)result2;
    (void)result3;
}
