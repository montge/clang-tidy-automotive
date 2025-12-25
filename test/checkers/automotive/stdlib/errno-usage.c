// RUN: %check_clang_tidy %s automotive-c25-req-22.8 %t -- -- -std=c11

// Test for errno usage requirements (Rules 22.8 and 22.9)
// These rules require:
// - errno set to 0 before calling errno-setting functions (22.8)
// - errno tested after calling errno-setting functions (22.9)

#include <errno.h>
#include <stdlib.h>
#include <math.h>

void test_strtol(void) {
  char *endptr;
  const char *str = "12345";

  // This call should be flagged - user should verify errno handling
  long result = strtol(str, &endptr, 10);
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: call to errno-setting function 'strtol'

  (void)result;
}

void test_math_functions(void) {
  double x = -1.0;

  // Math functions that can set errno
  double r1 = sqrt(x);
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: call to errno-setting function 'sqrt'

  double r2 = log(x);
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: call to errno-setting function 'log'

  double r3 = exp(1000.0);
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: call to errno-setting function 'exp'

  (void)r1; (void)r2; (void)r3;
}

void test_memory_functions(void) {
  // Memory allocation can set errno
  void *p = malloc(1000);
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: call to errno-setting function 'malloc'

  (void)p;
}

// Proper usage pattern (for documentation)
void proper_strtol_usage(void) {
  char *endptr;
  const char *str = "12345";

  // Proper usage: set errno to 0, call function, then check errno
  errno = 0;
  long result = strtol(str, &endptr, 10);
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: call to errno-setting function 'strtol'
  // Note: The check still flags this as a reminder; full dataflow
  // verification requires manual review

  if (errno != 0) {
    // Handle error
  }

  (void)result;
}
