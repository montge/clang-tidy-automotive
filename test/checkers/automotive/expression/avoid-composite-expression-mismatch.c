// RUN: %check_clang_tidy %s automotive-c23-req-10.6 %t

// Test: Composite expression type mismatch (MISRA Rules 10.6, 10.7, 10.8)

#include <stdint.h>

// Rule 10.6: Composite expression with narrower type assigned to wider type

void test_assignment_widening(void) {
    int16_t a = 100;
    int16_t b = 200;
    int32_t result32;
    int64_t result64;

    // Violation: int16_t + int16_t is int (32-bit), assigned to int64_t
    result64 = a + b;
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: composite expression with narrower type assigned to object with wider type

    // Violation: int16_t * int16_t is int, assigned to int64_t
    result64 = a * b;
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: composite expression with narrower type assigned to object with wider type

    // Compliant: int32_t arithmetic assigned to int32_t
    int32_t x = 100;
    int32_t y = 200;
    result32 = x + y;

    // Use variables to avoid unused warnings
    (void)result32;
    (void)result64;
}

void test_bitwise_widening(void) {
    uint8_t a = 0xFF;
    uint8_t b = 0x0F;
    uint32_t result32;
    uint64_t result64;

    // Violation: uint8_t & uint8_t promoted to int, assigned to uint64_t
    result64 = a & b;
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: composite expression with narrower type assigned to object with wider type

    // Violation: uint8_t | uint8_t promoted to int, assigned to uint64_t
    result64 = a | b;
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: composite expression with narrower type assigned to object with wider type

    // Violation: uint8_t ^ uint8_t promoted to int, assigned to uint64_t
    result64 = a ^ b;
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: composite expression with narrower type assigned to object with wider type

    // Compliant: same-width operations
    uint32_t x = 0xFFFF;
    uint32_t y = 0x0F0F;
    result32 = x & y;

    (void)result32;
    (void)result64;
}

void test_shift_widening(void) {
    uint8_t a = 1;
    int shift = 4;
    uint32_t result32;
    uint64_t result64;

    // Violation: uint8_t << int is int, assigned to uint64_t
    result64 = a << shift;
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: composite expression with narrower type assigned to object with wider type

    // Violation: uint8_t >> int is int, assigned to uint64_t
    result64 = a >> shift;
    // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: composite expression with narrower type assigned to object with wider type

    // Compliant: same-width operations
    uint32_t x = 1;
    result32 = x << shift;

    (void)result32;
    (void)result64;
}

// Rule 10.8: Cast of composite expression to wider type

void test_cast_widening(void) {
    int16_t a = 100;
    int16_t b = 200;
    int64_t result;

    // Violation: casting narrow composite to wider type
    result = (int64_t)(a + b);
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: cast of composite expression to wider type

    result = (int64_t)(a * b);
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: cast of composite expression to wider type

    result = (int64_t)(a - b);
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: cast of composite expression to wider type

    // Compliant: casting to same width
    int32_t result32 = (int32_t)(a + b);

    // Compliant: casting to narrower type (different rule)
    int16_t result16 = (int16_t)(a + b);

    (void)result;
    (void)result32;
    (void)result16;
}

// Compliant cases

void test_compliant_assignments(void) {
    int32_t a = 100;
    int32_t b = 200;
    int32_t result32;
    int64_t c = 100;
    int64_t d = 200;
    int64_t result64;

    // Compliant: same-width assignment
    result32 = a + b;
    result64 = c + d;

    // Compliant: narrowing (different issue but not this rule)
    int16_t narrow = a + b;

    (void)result32;
    (void)result64;
    (void)narrow;
}
