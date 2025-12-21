// RUN: %check_clang_tidy %s automotive-c23-req-21.10 %t -- -- -std=c99

// MISRA C:2025 Rule 21.10 (Required)
// The standard library time and date routines shall not be used.

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: inclusion of <time.h> is not allowed in safety-critical code [automotive-c23-req-21.10]
#include <time.h>

void example() {
  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: use of 'time' is not allowed in safety-critical code [automotive-c23-req-21.10]
  time_t now = time(NULL);

  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: use of 'clock' is not allowed in safety-critical code [automotive-c23-req-21.10]
  clock_t c = clock();

  // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: use of 'localtime' is not allowed in safety-critical code [automotive-c23-req-21.10]
  struct tm *t = localtime(&now);

  char buf[64];
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: use of 'strftime' is not allowed in safety-critical code [automotive-c23-req-21.10]
  strftime(buf, sizeof(buf), "%Y-%m-%d", t);

  (void)c;
  (void)t;
}
