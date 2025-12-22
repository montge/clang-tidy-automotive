// RUN: %check_clang_tidy %s automotive-c23-req-12.2 %t

// Test: Shift operands must be appropriate (MISRA Rule 12.2)

// Violation - negative shift amount
// Note: Not detected by automotive-c23-req-12.2
void test_negative_shift(void) {
    int x = 1;
    int neg = -1;
    int r = x << neg;
    (void)r;
}

// Violation - shift amount exceeds bit width
void test_excessive_shift(void) {
    int x = 1;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: shift amount (32) is greater than or equal to the width of the type (32 bits)
    int r = x << 32;
    (void)r;
}

// Violation - shifting negative value
// Note: Not detected by automotive-c23-req-12.2
void test_negative_value_shift(void) {
    int x = -1;
    int r = x << 2;
    (void)r;
}

// Compliant - valid shift operations
void test_valid_shifts(void) {
    unsigned int x = 1;
    unsigned int r1 = x << 4;
    unsigned int r2 = x >> 4;
    int y = 16;
    int r3 = y >> 2;
    (void)r1; (void)r2; (void)r3;
}
