// RUN: %check_clang_tidy %s automotive-c25-mand-21.18 %t

#include <string.h>

void test_size_t_argument(void) {
  char src[50];
  char dest[10];

  // Violation: size exceeds destination buffer
  memcpy(dest, src, 100);
  // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: size_t argument exceeds destination buffer size of 10 bytes

  // Violation: zero size has no effect
  memcpy(dest, src, 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: size_t argument of zero has no effect for function 'memcpy'

  // Compliant: size within bounds
  memcpy(dest, src, 10);  // OK
  memcpy(dest, src, 5);   // OK

  // Violation: size exceeds buffer
  memset(dest, 0, 50);
  // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: size_t argument exceeds destination buffer size of 10 bytes

  // Compliant: valid size
  memset(dest, 0, 10);  // OK

  // Test with strncpy
  strncpy(dest, src, 10);  // OK
  strncpy(dest, src, 0);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: size_t argument of zero has no effect for function 'strncpy'
}
