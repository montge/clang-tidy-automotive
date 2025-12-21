// RUN: %check_clang_tidy %s automotive-c23-req-17.10 %t -- -- -std=c11
// Test for automotive-c23-req-17.10
// Related MISRA C:2025 Rule: 17.10

#include <stdnoreturn.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:15: warning: _Noreturn function shall have void return type [automotive-c23-req-17.10]
_Noreturn int noreturn_int(void) {
    while(1) {}
}

// CHECK-MESSAGES: :[[@LINE+1]]:14: warning: _Noreturn function shall have void return type [automotive-c23-req-17.10]
noreturn int noreturn_macro_int(void) {
    while(1) {}
}

// CHECK-MESSAGES: :[[@LINE+1]]:17: warning: _Noreturn function shall have void return type [automotive-c23-req-17.10]
_Noreturn char *noreturn_ptr(void) {
    while(1) {}
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// _Noreturn with void return type - compliant
_Noreturn void proper_abort(void) {
    while(1) {}
}

noreturn void proper_exit(void) {
    while(1) {}
}

// Regular functions with non-void return
int regular_function(void) {
    return 42;
}

void regular_void_function(void) {
}
