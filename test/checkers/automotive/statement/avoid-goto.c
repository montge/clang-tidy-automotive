// RUN: %check_clang_tidy %s automotive-avoid-goto %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-avoid-goto %t -- -- -std=c11

void f(void) {
  goto Label1;    /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid goto statement [automotive-avoid-goto]
Label1:
  (void)0;
}

void g(void) {
Label2:
  goto Label2;    /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: avoid goto statement [automotive-avoid-goto]
}
