// RUN: %check_clang_tidy %s automotive-unused-type %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-unused-type %t -- -- -std=c11

// MISRA C:2025 Rule 2.3 (Advisory)
// A project should not contain unused type declarations.

#include <stdint.h>

typedef int16_t type1;  /* Not compliant */
// CHECK-MESSAGES: :[[@LINE-1]]:17: warning: unused type declaration 'type1' [automotive-unused-type]
typedef int32_t type2;  /* Compliant */


void f(void) {
  typedef int8_t type3;  /* Not compliant */
// CHECK-MESSAGES: :[[@LINE-1]]:18: warning: unused type declaration 'type3' [automotive-unused-type]
}

void g(void) {
  typedef uint8_t type4; /* Compliant */
  type2 x;
  type4 y;
}
