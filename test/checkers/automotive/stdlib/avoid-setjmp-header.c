// RUN: %check_clang_tidy %s automotive-c23-req-21.4 %t

// Test: setjmp/longjmp shall not be used (MISRA Rule 21.4)

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: use of <setjmp.h> is not permitted
#include <setjmp.h>

jmp_buf env;

// Violation - using setjmp
void test_setjmp(void) {
    if (setjmp(env) != 0) {
        // returned from longjmp
    }
}

// Violation - using longjmp
void test_longjmp(void) {
    longjmp(env, 1);
}

// Use functions to avoid unused warnings
void use_functions(void) {
    test_setjmp();
}
