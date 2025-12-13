// Test file for: automotive-avoid-ascii-to-number
// Related MISRA C:2025 Rule: 21.7
//
// This file tests the detection of atoi/atof/atol function usage

// RUN: %check_clang_tidy %s automotive-avoid-ascii-to-number %t

#include <stdlib.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_atoi_violations(void) {
    const char *str = "123";

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: use of atoi
    int i = atoi(str);

    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of atol
    long l = atol(str);

    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: use of atof
    double d = atof("3.14");

    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: use of atoll
    long long ll = atoll(str);

    (void)i; (void)l; (void)d; (void)ll;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    const char *str = "123";
    char *endptr;

    // Use strtol instead of atoi/atol
    long l = strtol(str, &endptr, 10);
    if (*endptr != '\0') {
        // Handle conversion error
    }

    // Use strtod instead of atof
    double d = strtod("3.14", &endptr);
    if (*endptr != '\0') {
        // Handle conversion error
    }

    (void)l; (void)d;
}
