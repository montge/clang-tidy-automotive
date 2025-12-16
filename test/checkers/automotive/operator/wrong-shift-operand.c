// RUN: %check_clang_tidy %s automotive-c23-req-12.2 %t

// Test: Shift operands must be appropriate (MISRA Rule 12.2)

// Violation - negative shift amount
void test_negative_shift(void) {
    int x = 1;
    int neg = -1;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: shift count is negative
    int r = x << neg;
    (void)r;
}

// Violation - shift amount exceeds bit width
void test_excessive_shift(void) {
    int x = 1;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: shift count >= width of type
    int r = x << 32;
    (void)r;
}

// Violation - shifting negative value
void test_negative_value_shift(void) {
    int x = -1;
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: left shift of negative value
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
