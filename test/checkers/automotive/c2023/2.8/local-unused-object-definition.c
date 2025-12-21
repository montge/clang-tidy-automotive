// RUN: %check_clang_tidy %s automotive-c23-adv-2.8 %t -- -- -std=c99
// RUN: %check_clang_tidy %s automotive-c23-adv-2.8 %t -- -- -std=c11

// MISRA C:2025 Rule 2.8 (Advisory)
// An object defined at block scope should have its value used.

void f() {
  int x;       /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: unused object definition 'x' [automotive-c23-adv-2.8]

  int y = 10;  /* Not compliant */
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: unused object definition 'y' [automotive-c23-adv-2.8]

  int z = 20;  /* Compliant - z is used by a and should not be flagged */
  int a = z;

  if (a > 0) {
  }
}
