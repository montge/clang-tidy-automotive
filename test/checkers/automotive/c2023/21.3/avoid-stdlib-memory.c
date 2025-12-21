// RUN: %check_clang_tidy %s automotive-c23-req-21.3 %t -- -- -std=c99

// MISRA C:2025 Rule 21.3 (Required)
// The memory allocation and deallocation functions of stdlib.h shall not be used.

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: inclusion of <stdlib.h> is not allowed in safety-critical code [automotive-c23-req-21.3]
#include <stdlib.h>

void example() {
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of 'malloc' is not allowed in safety-critical code [automotive-c23-req-21.3]
  void *ptr1 = malloc(100);
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of 'calloc' is not allowed in safety-critical code [automotive-c23-req-21.3]
  void *ptr2 = calloc(10, 10);
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of 'realloc' is not allowed in safety-critical code [automotive-c23-req-21.3]
  void *ptr3 = realloc(ptr1, 200);

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'free' is not allowed in safety-critical code [automotive-c23-req-21.3]
  free(ptr1);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'free' is not allowed in safety-critical code [automotive-c23-req-21.3]
  free(ptr2);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'free' is not allowed in safety-critical code [automotive-c23-req-21.3]
  free(ptr3);
}
