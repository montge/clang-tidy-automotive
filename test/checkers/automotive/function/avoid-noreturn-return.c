// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-mand-17.9 %t

#include <stdlib.h>

// Helper functions (forward declaration)
void cleanup_resources(void);

// Violation: noreturn function with return statement
_Noreturn void fatal_error(void) {
  cleanup_resources();
  return;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: _Noreturn function 'fatal_error' shall not return to its caller
}

_Noreturn void another_bad_function(int x) {
  if (x > 0) {
    return;
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: _Noreturn function 'another_bad_function' shall not return to its caller
  }
  abort();
}

// Compliant: noreturn function without return statement
_Noreturn void proper_fatal(void) {
  cleanup_resources();
  abort();  // OK: calls abort, never returns
}

_Noreturn void infinite_loop(void) {
  while (1) {
    // OK: infinite loop, never returns
  }
}

_Noreturn void call_exit(void) {
  exit(1);  // OK: calls exit, never returns
}

// Helper functions
void cleanup_resources(void) {}
