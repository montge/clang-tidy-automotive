// RUN: %check_clang_tidy %s automotive-c25-req-10.6 %t -- -- -std=c11
// Test for automotive-c25-req-10.6: narrowing compound assignments shall not be used

#include <stdint.h>

void test_narrowing_addition_violation(void) {
  int8_t small = 10;
  int32_t large = 1000;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: narrowing compound assignment
  small += large;  // int32_t value assigned to int8_t
  (void)small;
}

void test_narrowing_multiplication_violation(void) {
  uint8_t byte = 10;
  uint32_t word = 50;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: narrowing compound assignment
  byte *= word;  // Result may overflow
  (void)byte;
}

void test_narrowing_subtraction_violation(void) {
  int16_t s = 100;
  int64_t l = 50;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: narrowing compound assignment
  s -= l;
  (void)s;
}

void test_same_type_compliant(void) {
  // OK - same type, no narrowing
  int32_t a = 100;
  int32_t b = 50;
  a += b;
  (void)a;
}

void test_widening_compliant(void) {
  // OK - widening is safe
  int64_t large = 100;
  int32_t small = 50;
  large += small;
  (void)large;
}

void test_same_size_signed_unsigned_violation(void) {
  int32_t s = 100;
  uint32_t u = 50;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: narrowing compound assignment
  s += u;  // Signed/unsigned mismatch
  (void)s;
}

void test_explicit_cast_compliant(void) {
  // OK - explicit cast shows intent
  int8_t small = 10;
  int32_t large = 5;
  small = (int8_t)(small + large);  // Explicit cast
  (void)small;
}
