// RUN: %check_clang_tidy %s automotive-c25-req-21.7 %t -- -- -std=c11
// Test for automotive-c25-req-21.7: atof, atoi, atol, atoll shall not be used

#include <stdlib.h>

void test_atoi_violation(const char *str) {
  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: Avoid function 'atoi' that has undefined behavior
  int x = atoi(str);
  (void)x;
}

void test_atol_violation(const char *str) {
  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: Avoid function 'atol' that has undefined behavior
  long y = atol(str);
  (void)y;
}

void test_atoll_violation(const char *str) {
  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: Avoid function 'atoll' that has undefined behavior
  long long z = atoll(str);
  (void)z;
}

void test_atof_violation(const char *str) {
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: Avoid function 'atof' that has undefined behavior
  double d = atof(str);
  (void)d;
}

void test_strtol_compliant(const char *str) {
  // OK - strtol provides error handling
  char *endptr;
  long val = strtol(str, &endptr, 10);
  (void)val;
}

void test_strtod_compliant(const char *str) {
  // OK - strtod provides error handling
  char *endptr;
  double val = strtod(str, &endptr);
  (void)val;
}
