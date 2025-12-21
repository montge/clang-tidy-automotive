// Test file for: automotive-missing-return-void
// Related MISRA C:2025 Rule: 15.6
//
// This file tests that functions with [[noreturn]] should have void return type

// RUN: %check_clang_tidy %s automotive-missing-return-void %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Noreturn function with int return type
// CHECK-MESSAGES: :[[@LINE+2]]:5: warning: use 'void' as return type
// CHECK-MESSAGES: :[[@LINE+1]]:40: warning: GCC does not allow 'noreturn' attribute in this position on a function definition
int bad_function1(void) __attribute__((noreturn)) {
    while(1);
}

// Noreturn function with pointer return type
// CHECK-MESSAGES: :[[@LINE+2]]:7: warning: use 'void' as return type
// CHECK-MESSAGES: :[[@LINE+1]]:42: warning: GCC does not allow 'noreturn' attribute in this position on a function definition
void* bad_function2(void) __attribute__((noreturn)) {
    while(1);
}

// Using _Noreturn keyword (not detected by current check)
// TODO: This should be detected but _Noreturn is not currently handled
int bad_function3(void) {
    while(1);
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Noreturn function with void return type - OK
// CHECK-MESSAGES: :[[@LINE+1]]:42: warning: GCC does not allow 'noreturn' attribute in this position on a function definition
void good_function1(void) __attribute__((noreturn)) {
    while(1);
}

// Normal function with non-void return type - OK
int good_function2(void) {
    return 42;
}

// Normal void function - OK
void good_function3(void) {
    return;
}
