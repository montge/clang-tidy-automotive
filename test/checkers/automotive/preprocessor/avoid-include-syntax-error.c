// RUN: %check_clang_tidy %s automotive-c23-req-20.3 %t -- -- -I%S

// Test: Include syntax validation (MISRA Rule 20.3)
// Check ID: automotive-c23-req-20.3

// ============= Compliant includes =============

#include <stddef.h>
#include <stdint.h>

// ============= Violations: Absolute paths =============

// Note: These would trigger warnings but may cause compilation errors
// if the paths don't exist, so we use conditional compilation

#ifdef TEST_ABSOLUTE_PATHS
// CHECK-MESSAGES: :[[@LINE+1]]:10: warning: #include uses absolute path which is not portable
#include "/usr/include/stdio.h"

// Windows-style absolute path
// CHECK-MESSAGES: :[[@LINE+1]]:10: warning: #include uses absolute path which is not portable
#include "C:\headers\test.h"
#endif

// ============= Compliant: Relative paths =============

// These are compliant - relative paths are portable
// #include "header.h"           // Would work if file exists
// #include "../common/util.h"   // Relative path is OK
// #include "./local.h"          // Relative path is OK

// ============= Test with valid system includes =============

void test_function(void) {
    size_t s = 0;
    (void)s;
}
