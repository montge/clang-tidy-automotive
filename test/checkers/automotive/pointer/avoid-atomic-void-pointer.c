// RUN: %check_clang_tidy %s automotive-c23-req-11.9 %t

// Test: Atomic void pointer (MISRA Rule 11.9)

#include <stdatomic.h>

// Violation - atomic void pointer
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: atomic void pointer should be avoided
_Atomic(void *) atomic_void_ptr;

// Compliant - atomic typed pointer
_Atomic(int *) atomic_int_ptr;

// Use variables to avoid unused warnings
void use_vars(void) {
    (void)atomic_void_ptr;
    (void)atomic_int_ptr;
}
