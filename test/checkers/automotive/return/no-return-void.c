// Test file for: automotive-missing-return-void
//
// This file tests that functions with [[noreturn]] should have void return type

// RUN: %check_clang_tidy %s automotive-missing-return-void %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Noreturn function with int return type
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use 'void' as return type
int bad_function1(void) __attribute__((noreturn)) {
    while(1);
}

// Noreturn function with pointer return type
// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: use 'void' as return type
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
