// RUN: %check_clang_tidy %s automotive-avoid-restrict-type %t -- -- -std=c11

// MISRA C:2025 Rule 8.14 (Required)
// The restrict type qualifier shall not be used.

void f(int *restrict ptr) {
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: avoid restrict type [automotive-avoid-restrict-type]
}

void g(char *restrict ptr) {
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: avoid restrict type [automotive-avoid-restrict-type]
}
