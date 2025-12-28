// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-mand-7.5 %t

#include <stdint.h>

// MISRA C:2025 Rule 7.5 - Integer constant macro argument form

void test_violations(void) {
    // Violation: Expression as argument
    int32_t a = INT32_C(1 + 2);
    // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: argument to integer constant macro 'INT32_C' should be a simple integer constant, not an expression

    // Violation: Unsigned suffix on signed macro
    int64_t b = INT64_C(100UL);
    // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: argument to signed integer constant macro 'INT64_C' has conflicting type suffix

    // Violation: Long suffix (macro handles the type)
    uint32_t c = UINT32_C(200L);
    // CHECK-MESSAGES: :[[@LINE-1]]:18: warning: argument to unsigned integer constant macro 'UINT32_C' has conflicting type suffix

    // Violation: Expression with operators
    int16_t d = INT16_C(10 * 5);
    // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: argument to integer constant macro 'INT16_C' should be a simple integer constant, not an expression

    // Violation: Bitwise expression
    uint8_t e = UINT8_C(0xFF & 0x0F);
    // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: argument to integer constant macro 'UINT8_C' should be a simple integer constant, not an expression
}

void test_compliant(void) {
    // Compliant: Simple decimal constant
    int32_t a = INT32_C(42);

    // Compliant: Simple hex constant
    uint64_t b = UINT64_C(0xDEADBEEF);

    // Compliant: Negative constant (leading minus allowed)
    int16_t c = INT16_C(-100);

    // Compliant: Zero
    uint8_t d = UINT8_C(0);

    // Compliant: Max values without suffix
    int64_t e = INT64_C(9223372036854775807);
    uint64_t f = UINT64_C(18446744073709551615);

    // Avoid unused variable warnings
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
}
