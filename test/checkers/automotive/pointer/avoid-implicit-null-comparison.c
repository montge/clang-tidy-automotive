// RUN: %check_clang_tidy %s automotive-c23-req-11.11 %t
// Test for automotive-c23-req-11.11
// Related MISRA C:2025 Rule: 11.11

#include <stdbool.h>
#include <stddef.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_violations(int *p) {
    // Implicit pointer to boolean conversion
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: pointer shall be explicitly compared to NULL [automotive-c23-req-11.11]
    bool b = p;

    (void)b;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(int *p) {
    // Explicit comparison with NULL
    bool a = (p != NULL);
    bool b = (p == NULL);

    // Explicit comparison with 0
    bool c = (p != 0);

    // Boolean to boolean
    bool flag = true;
    bool d = flag;

    (void)a; (void)b; (void)c; (void)d;
}
