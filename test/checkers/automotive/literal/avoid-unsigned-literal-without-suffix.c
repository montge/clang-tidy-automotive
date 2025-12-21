// RUN: %check_clang_tidy %s automotive-c23-req-7.2 %t
// Test for automotive-c23-req-7.2
// Related MISRA C:2025 Rule: 7.2

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_violations(void) {
    // Large hex constant without U suffix - gets unsigned type
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unsigned integer constant should have a 'U' suffix [automotive-c23-req-7.2]
    unsigned x = 0x80000000;

    // Large octal constant without U suffix
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unsigned integer constant should have a 'U' suffix [automotive-c23-req-7.2]
    unsigned y = 020000000000;

    (void)x; (void)y;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    // With U suffix - compliant
    unsigned a = 0x80000000U;
    unsigned b = 020000000000U;

    // Small hex literals that fit in signed type
    int c = 0xFF;
    int d = 0777;

    // Decimal literals
    unsigned e = 2147483648;  // Decimal - different rule applies

    (void)a; (void)b; (void)c; (void)d; (void)e;
}
