// RUN: %check_clang_tidy %s automotive-avoid-signal-header %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-signal-header %t -- -- -std=c11

// MISRA C:2025 Rule 21.5 (Required)
// The standard header file signal.h shall not be used.

#include <stdlib.h>
#include <signal.h>

void example() {
  // Standard signal functions from signal.h

  void (*handler)(int) = signal(SIGINT, SIG_DFL);
  // CHECK-MESSAGES: :[[@LINE-1]]:26: warning: Avoid function 'signal' from <signal.h> [automotive-avoid-signal-header]

  raise(SIGTERM);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: Avoid function 'raise' from <signal.h> [automotive-avoid-signal-header]
}
