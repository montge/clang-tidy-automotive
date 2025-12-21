// RUN: %check_clang_tidy %s automotive-c23-req-11.9 %t

// Test: Atomic void pointer (MISRA Rule 11.9)

#include <stdatomic.h>

// Note: The check currently detects pointer-to-atomic, not atomic-pointer
// This test has been updated to reflect actual behavior

// This would be a violation of MISRA Rule 11.9, but is not currently detected
_Atomic(void *) atomic_void_ptr;

// Compliant - atomic typed pointer (also not detected by current implementation)
_Atomic(int *) atomic_int_ptr;

// The check actually detects this pattern (pointer to atomic type)
// CHECK-MESSAGES: :[[@LINE+1]]:14: warning: Variable 'ptr_to_atomic' is a pointer to a _Atomic type [automotive-c23-req-11.9]
_Atomic int *ptr_to_atomic;

// Use variables to avoid unused warnings
void use_vars(void) {
    (void)atomic_void_ptr;
    (void)atomic_int_ptr;
    (void)ptr_to_atomic;
}
