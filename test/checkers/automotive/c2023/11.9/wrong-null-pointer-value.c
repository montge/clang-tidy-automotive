// RUN: %check_clang_tidy %s automotive-wrong-null-pointer-value %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-wrong-null-pointer-value %t -- -- -std=c11

// MISRA C:2025 Rule 11.9
// The value of an object with pointer type shall not be used if it is not
// known to point to a valid location. Using integer 0 instead of NULL is
// error-prone.

#include <stddef.h>

void test() {
    int *p1 = 0;
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: Use NULL instead of 0 for null pointer assignment. [automotive-wrong-null-pointer-value]
    int *p2 = (void *)0;  // Compliant
    int *p3 = NULL;       // Compliant

    if (p1 == 0) { }
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: Use NULL instead of 0 for null pointer assignment. [automotive-wrong-null-pointer-value]
    if (p2 == (void *)0) { } // Compliant
    if (p3 == NULL) { }      // Compliant

    p1 = 0;
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: Use NULL instead of 0 for null pointer assignment. [automotive-wrong-null-pointer-value]
    p2 = (void *)0;  // Compliant

    p1 = (p2 == NULL) ? p3 : 0;
    // CHECK-MESSAGES: :[[@LINE-1]]:30: warning: Use NULL instead of 0 for null pointer assignment. [automotive-wrong-null-pointer-value]
    p2 = (p1 == 0) ? NULL : p3;
    // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: Use NULL instead of 0 for null pointer assignment. [automotive-wrong-null-pointer-value]
}
