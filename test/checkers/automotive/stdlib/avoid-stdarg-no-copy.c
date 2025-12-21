// Test file for: automotive-avoid-stdarg-header
// Related MISRA C:2025 Rule: 17.1 - The features of <stdarg.h> shall not be used
//
// This test verifies detection of stdarg.h inclusion and macro usage

// RUN: clang-tidy %s --checks='-*,automotive-avoid-stdarg-header' -- 2>&1 | FileCheck %s

// CHECK-DAG: warning: inclusion of <stdarg.h> is not allowed in safety-critical code
#include <stdarg.h>

void f(int n, ...) {
  va_list args;
  // CHECK-DAG: warning: use of 'va_start' is not allowed in safety-critical code
  va_start(args, n);

  // CHECK-DAG: warning: use of 'va_arg' is not allowed in safety-critical code
  int val = va_arg(args, int);

  // CHECK-DAG: warning: use of 'va_end' is not allowed in safety-critical code
  va_end(args);
  (void)val;
}
