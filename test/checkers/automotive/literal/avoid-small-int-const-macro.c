// RUN: %check_clang_tidy %s automotive-c23-req-7.6 %t
// Test for automotive-c23-req-7.6
// Related MISRA C:2025 Rule: 7.6

// This test verifies that small integer constant macros are detected.

#include <stdint.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_small_int_const_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of small integer constant macro 'INT8_C' may produce implementation-defined behavior; consider using a larger type or explicit constant [automotive-c23-req-7.6]
    int8_t a = INT8_C(42);

    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: use of small integer constant macro 'INT16_C' may produce implementation-defined behavior; consider using a larger type or explicit constant [automotive-c23-req-7.6]
    int16_t b = INT16_C(1000);

    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: use of small integer constant macro 'UINT8_C' may produce implementation-defined behavior; consider using a larger type or explicit constant [automotive-c23-req-7.6]
    uint8_t c = UINT8_C(255);

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: use of small integer constant macro 'UINT16_C' may produce implementation-defined behavior; consider using a larger type or explicit constant [automotive-c23-req-7.6]
    uint16_t d = UINT16_C(65535);

    (void)a;
    (void)b;
    (void)c;
    (void)d;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    // Using INT32_C and larger is allowed
    int32_t a = INT32_C(42);
    int64_t b = INT64_C(1000);
    uint32_t c = UINT32_C(255);
    uint64_t d = UINT64_C(65535);

    // Using INTMAX_C is allowed
    intmax_t e = INTMAX_C(42);
    uintmax_t f = UINTMAX_C(255);

    // Plain integer literals are allowed
    int8_t g = 42;
    uint16_t h = 1000U;

    (void)a;
    (void)b;
    (void)c;
    (void)d;
    (void)e;
    (void)f;
    (void)g;
    (void)h;
}
