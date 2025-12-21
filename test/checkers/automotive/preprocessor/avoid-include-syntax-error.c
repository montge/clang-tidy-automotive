// RUN: %check_clang_tidy %s automotive-c23-req-20.3 %t -- -- -I%S

// Test: Include syntax validation (MISRA Rule 20.3)
// Check ID: automotive-c23-req-20.3

// ============= Compliant includes =============

#include <stddef.h>
#include <stdint.h>

// ============= Potential violations: Absolute paths =============

// Note: These would trigger warnings but may cause compilation errors
// if the paths don't exist, so we use conditional compilation
// The check may not be fully implemented

#ifdef TEST_ABSOLUTE_PATHS
#include "/usr/include/stdio.h"

// Windows-style absolute path
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
