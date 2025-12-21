// RUN: %check_clang_tidy %s automotive-unused-label %t

void f(void) {
Label1:        /* Not compliant */
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: unused label 'Label1' [automotive-unused-label]
  (void)0;  // Prevent C23 extension warning about label at end of compound statement
}

void g(void) {
  goto Label2;
Label2:        /* Compliant */
  (void)0;  // Prevent C23 extension warning
}
