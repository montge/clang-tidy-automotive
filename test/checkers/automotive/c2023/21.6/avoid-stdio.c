// RUN: %check_clang_tidy %s automotive-c23-req-21.6 %t -- -- -std=c99

// MISRA C:2025 Rule 21.6 (Required)
// The standard library input/output routines shall not be used.

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: inclusion of <stdio.h> is not allowed in safety-critical code [automotive-c23-req-21.6]
#include <stdio.h>

void example() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'printf' is not allowed in safety-critical code [automotive-c23-req-21.6]
  printf("Hello, world!\n");

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'puts' is not allowed in safety-critical code [automotive-c23-req-21.6]
  puts("Hello");

  int x;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'scanf' is not allowed in safety-critical code [automotive-c23-req-21.6]
  scanf("%d", &x);

  FILE *f;
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: use of 'fopen' is not allowed in safety-critical code [automotive-c23-req-21.6]
  f = fopen("test.txt", "r");

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'fclose' is not allowed in safety-critical code [automotive-c23-req-21.6]
  fclose(f);
}
