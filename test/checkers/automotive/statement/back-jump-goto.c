// RUN: %check_clang_tidy %s automotive-forward-goto-label %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-forward-goto-label %t -- -- -std=c11

void f(void) {
  goto Label1;    /* Compliant */
Label1:
  (void)0;
}

void g(void) {
Label2:
  goto Label2;    /* Non compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: goto statement jumps backward to label 'Label2' [automotive-forward-goto-label]
}
