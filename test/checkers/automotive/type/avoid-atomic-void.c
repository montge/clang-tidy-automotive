// RUN: %check_clang_tidy %s automotive-c23-req-11.10 %t -- -- -std=c11
// Test for automotive-c23-req-11.10
// Related MISRA C:2025 Rule: 11.10
// Note: _Atomic void is a compiler error, so this check is primarily
// a compile-time diagnostic. This test demonstrates the rule intent.

#include <stdatomic.h>

//===----------------------------------------------------------------------===//
// Compliant Cases - atomic with concrete types
//===----------------------------------------------------------------------===//

_Atomic int my_atomic_int;

void test_compliant(void) {
    _Atomic int local_atomic = 0;
    my_atomic_int = local_atomic;

    // Non-atomic void pointer - compliant
    void *regular_void_ptr;
    (void)regular_void_ptr;
}

// Note: The following would be a violation but causes a compiler error:
// _Atomic void *atomic_void_ptr;  // Error: _Atomic cannot be applied to incomplete type 'void'
