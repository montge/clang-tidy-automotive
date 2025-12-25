// RUN: %check_clang_tidy %s automotive-c25-adv-11.4 %t -- -- -std=c11
// Test for automotive-c25-adv-11.4: casts between pointer and integer types

#include <stdint.h>

void test_pointer_to_int_violation(void) {
  int x = 42;
  int *ptr = &x;

  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from pointer type 'int *' to integer type 'intptr_t' (aka 'long') [automotive-c25-adv-11.4]
  intptr_t i = (intptr_t)ptr;  // Pointer to integer cast
  (void)i;
}

void test_int_to_pointer_violation(void) {
  intptr_t addr = 0x1000;

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: cast from integer type 'intptr_t' (aka 'long') to pointer type 'int *' [automotive-c25-adv-11.4]
  int *ptr = (int *)addr;  // Integer to pointer cast
  (void)ptr;
}

void test_numeric_cast_compliant(void) {
  // OK - numeric conversions
  int i = 42;
  double d = (double)i;
  long l = (long)i;
  (void)d;
  (void)l;
}

void test_void_pointer_cast_compliant(void) {
  // OK - casting to/from void pointer
  int x = 42;
  void *vptr = &x;
  int *iptr = (int *)vptr;
  (void)iptr;
}

void test_const_cast_compliant(void) {
  // OK - adding const (removing const should be flagged by different check)
  int x = 42;
  const int *cptr = (const int *)&x;
  (void)cptr;
}
