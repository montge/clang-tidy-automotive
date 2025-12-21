// RUN: %check_clang_tidy %s automotive-c23-req-21.11 %t -- -- -std=c99

// MISRA C:2025 Rule 21.11 (Required)
// The <tgmath.h> header shall not be used.

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: inclusion of <tgmath.h> is not allowed in safety-critical code [automotive-c23-req-21.11]
#include <tgmath.h>

void example() {
  double x = 2.0;
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: use of 'sqrt' is not allowed in safety-critical code [automotive-c23-req-21.11]
  double y = sqrt(x);
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: use of 'sin' is not allowed in safety-critical code [automotive-c23-req-21.11]
  double z = sin(x);
  (void)y;
  (void)z;
}
