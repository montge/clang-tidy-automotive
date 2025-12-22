// RUN: %check_clang_tidy %s automotive-avoid-comma-operator %t -- -- -std=c99

void test() {
  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: avoid comma operator
  int a = (1, 2);                               /* not complaint */

  // CHECK-MESSAGES: :[[@LINE+1]]:37: warning: avoid comma operator
  for (int i = 0, j = 1; i < 10; ++i, ++j) {    /* Not complaint */
  }
}

