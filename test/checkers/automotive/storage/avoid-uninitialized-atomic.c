// RUN: %check_clang_tidy %s automotive-c25-mand-9.7 %t

#include <stdatomic.h>

// Violations: uninitialized atomic objects
_Atomic int global_atomic;
// CHECK-MESSAGES: :[[@LINE-1]]:13: warning: atomic object 'global_atomic' shall be initialized before being accessed

void test_atomic_initialization(void) {
  _Atomic int local_atomic;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: atomic object 'local_atomic' shall be initialized before being accessed

  atomic_int another_atomic;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: atomic object 'another_atomic' shall be initialized before being accessed

  // Compliant: initialized atomic objects
  _Atomic int initialized1 = ATOMIC_VAR_INIT(0);  // OK
  _Atomic int initialized2 = 42;  // OK (C11 allows direct initialization)

  atomic_int initialized3 = ATOMIC_VAR_INIT(5);  // OK

  // Compliant: extern declaration (initialized elsewhere)
  extern _Atomic int external_atomic;  // OK
}

static _Atomic int static_atomic;
// CHECK-MESSAGES: :[[@LINE-1]]:20: warning: atomic object 'static_atomic' shall be initialized before being accessed

static _Atomic int static_initialized = 0;  // OK
