// RUN: %check_clang_tidy %s automotive-c25-req-10.1 %t -- -- -std=c11
// Test for automotive-c25-req-10.1: operands shall have appropriate essential types

#include <stdbool.h>

void test_bool_arithmetic_violation(bool flag) {
  // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: operand of essential type 'Boolean' is inappropriate for arithmetic operation [automotive-c25-req-10.1]
  int x = flag + 1;
  (void)x;
}

void test_bool_bitwise_violation(bool a, bool b) {
  // CHECK-MESSAGES: :[[@LINE+2]]:11: warning: operand of essential type 'Boolean' is inappropriate for bitwise operation [automotive-c25-req-10.1]
  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: operand of essential type 'Boolean' is inappropriate for bitwise operation [automotive-c25-req-10.1]
  int y = a & b;
  (void)y;
}

void test_char_arithmetic_violation(char c) {
  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: operand of essential type 'Character' is inappropriate for arithmetic operation [automotive-c25-req-10.1]
  char d = c + 1;
  (void)d;
}

void test_mixed_signed_unsigned_compliant(int s, unsigned int u) {
  // OK - mixing signed and unsigned is detected by a different check (10.4)
  int r = s + u;
  (void)r;
}

void test_appropriate_bool_use_compliant(bool a, bool b) {
  // OK - boolean operations on booleans
  bool result = a && b;
  bool or_result = a || b;
  bool not_result = !a;
  (void)result;
  (void)or_result;
  (void)not_result;
}

void test_appropriate_int_arithmetic_compliant(int a, int b) {
  // OK - integer arithmetic on integers
  int sum = a + b;
  int diff = a - b;
  int prod = a * b;
  (void)sum;
  (void)diff;
  (void)prod;
}

void test_explicit_cast_compliant(int s, unsigned int u) {
  // OK - explicit cast makes the operation clear
  int r = s + (int)u;
  (void)r;
}
