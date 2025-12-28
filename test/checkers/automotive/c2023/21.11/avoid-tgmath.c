// RUN: %check_clang_tidy %s automotive-c23-req-21.11 %t -- -- -std=c99

// MISRA C:2012 Rule 21.11 (Required)
// The standard header file <tgmath.h> shall not be used.
//
// This rule prohibits the header, NOT the standard math functions.
// Functions like sin(), cos(), sqrt() are fine when used from <math.h>.

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: inclusion of <tgmath.h> is not allowed in safety-critical code [automotive-c23-req-21.11]
#include <tgmath.h>

void example() {
  double x = 2.0;
  // These function calls are OK - we only flag the header inclusion
  double y = sqrt(x);
  double z = sin(x);
  (void)y;
  (void)z;
}
