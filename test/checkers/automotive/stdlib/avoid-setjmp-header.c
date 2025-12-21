// RUN: clang-tidy %s --checks='-*,automotive-avoid-setjmp-header' -- 2>&1 | FileCheck %s

// Test: setjmp/longjmp shall not be used (MISRA Rule 21.4)

// CHECK-DAG: warning: inclusion of <setjmp.h> is not allowed in safety-critical code
#include <setjmp.h>

jmp_buf env;

// setjmp is a macro - now detected by the enhanced check
void test_setjmp(void) {
    // CHECK-DAG: warning: use of 'setjmp' is not allowed in safety-critical code
    if (setjmp(env) != 0) {
        // returned from longjmp
    }
}

// Violation - using longjmp (function call)
void test_longjmp(void) {
    // CHECK-DAG: warning: use of 'longjmp' is not allowed in safety-critical code
    longjmp(env, 1);
}

// Use functions to avoid unused warnings
void use_functions(void) {
    test_setjmp();
}
