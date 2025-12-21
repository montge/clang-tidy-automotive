// RUN: clang-tidy %s --checks='-*,automotive-avoid-stdarg-header' -- -std=c99 2>&1 | FileCheck %s
// RUN: clang-tidy %s --checks='-*,automotive-avoid-stdarg-header' -- -std=c11 2>&1 | FileCheck %s

// Test: va_copy macro detection (MISRA Rule 17.1)

// CHECK-DAG: warning: inclusion of <stdarg.h> is not allowed in safety-critical code
#include <stdarg.h>

void g(int n, ...) {
  va_list args1, args2;
  // CHECK-DAG: warning: use of 'va_start' is not allowed in safety-critical code
  va_start(args1, n);

  // CHECK-DAG: warning: use of 'va_copy' is not allowed in safety-critical code
  va_copy(args2, args1);

  // CHECK-DAG: warning: use of 'va_arg' is not allowed in safety-critical code
  int val = va_arg(args2, int);

  // CHECK-DAG: warning: use of 'va_end' is not allowed in safety-critical code
  va_end(args1);

  // CHECK-DAG: warning: use of 'va_end' is not allowed in safety-critical code
  va_end(args2);
  (void)val;
}
