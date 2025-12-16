// RUN: %check_clang_tidy %s automotive-c23-req-dir-4.10 %t -- -- -I%S/headers

// Test: Missing header guard protection (MISRA Directive 4.10)
// Check ID: automotive-c23-req-dir-4.10

// Include header without guard - should trigger warning on the header
#include "no-guard.h"

// Include header with proper guard - compliant
#include "with-guard.h"

// Include header with pragma once - compliant
#include "pragma-once.h"

// Note: The warning is emitted on the header file itself, not on the include

void test_functions(void) {
    no_guard_function();
    with_guard_function();
    pragma_once_function();
}
