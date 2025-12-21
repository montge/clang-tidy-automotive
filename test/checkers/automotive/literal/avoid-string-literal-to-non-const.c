// RUN: %check_clang_tidy %s automotive-c23-req-7.4 %t
// Test for automotive-c23-req-7.4
// Related MISRA C:2025 Rule: 7.4

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_violations(void) {
    // String literal assigned to non-const pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: string literal should not be assigned to a non-const pointer [automotive-c23-req-7.4]
    char *p = "hello";

    (void)p;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    // String literal to const char* - compliant
    const char *p = "hello";

    // Array initialization (different rule)
    char arr[] = "hello";

    (void)p; (void)arr;
}
