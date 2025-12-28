// RUN: %check_clang_tidy %s automotive-c25-req-10.6 %t -- -- -std=c11
// Test for automotive-c25-req-10.6: composite expression assigned to wider type

#include <stdint.h>

void test_composite_to_wider_violation(void) {
  int16_t x = 1000;
  int16_t y = 1000;
  int64_t result;

  // No warning generated for this case
  result = x * y;  // int16_t * int16_t promoted to int (32-bit) assigned to int64_t
  (void)result;
}

void test_bitwise_to_wider_violation(void) {
  uint16_t a = 0x1234;
  uint16_t b = 0x5678;
  uint64_t result;

  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: expression of essentially 'signed' type assigned to object of essentially 'unsigned' type
  result = a | b;  // 32-bit result assigned to 64-bit
  (void)result;
}

void test_same_width_compliant(void) {
  // OK - same width, no mismatch
  int32_t a = 100;
  int32_t b = 50;
  int32_t result;
  result = a + b;
  (void)result;
}

void test_narrower_target_compliant(void) {
  // NOT OK - detects narrowing conversion
  int64_t a = 100;
  int64_t b = 50;
  int32_t result;
  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: assignment results in narrowing conversion from wider to narrower type
  result = a + b;
  (void)result;
}

void test_single_operand_compliant(void) {
  // OK - not a composite expression, just a variable
  int8_t small = 10;
  int32_t large;
  large = small;
  (void)large;
}
