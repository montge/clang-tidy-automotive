// RUN: %check_clang_tidy %s automotive-c25-req-11.1 %t -- -- -std=c11
// Test for automotive-c25-req-11.1: inappropriate casts shall not be used

#include <stdint.h>

void test_pointer_to_int_violation(void) {
  int x = 42;
  int *ptr = &x;

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: inappropriate cast
  intptr_t i = (intptr_t)ptr;  // Pointer to integer cast
  (void)i;
}

void test_int_to_pointer_violation(void) {
  intptr_t addr = 0x1000;

  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: inappropriate cast
  int *ptr = (int *)addr;  // Integer to pointer cast
  (void)ptr;
}

void test_function_pointer_cast_violation(void) {
  void (*func1)(int) = 0;

  // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: inappropriate cast
  void (*func2)(void) = (void (*)(void))func1;  // Function pointer cast
  (void)func2;
}

void test_incompatible_pointer_cast_violation(void) {
  int *iptr = 0;

  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: inappropriate cast
  float *fptr = (float *)iptr;  // Incompatible pointer types
  (void)fptr;
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
