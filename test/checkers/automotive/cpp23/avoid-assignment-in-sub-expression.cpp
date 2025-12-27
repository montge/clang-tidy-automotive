// RUN: %check_clang_tidy %s automotive-c23-adv-13.4 %t
// Test for MISRA C:2023 Rule 13.4: The result of an assignment operator
// shall not be used

void test_assignment_in_condition_violation(int x) {
  int y;
  // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: Avoid using the result of an assignment operator '='
  if (y = x) {
  }
}

void test_assignment_in_while_violation(int x) {
  int y;
  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: Avoid using the result of an assignment operator '='
  while (y = x) {
    x--;
  }
}

void test_assignment_in_for_condition_violation(int n) {
  int i;
  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: Avoid using the result of an assignment operator '='
  for (i = 0; i = n; i++) {
    n--;
  }
}

void test_chained_assignment_violation(int x) {
  int a, b;
  // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: Avoid using the result of an assignment operator '='
  a = b = x;
}

int test_assignment_in_return_violation(int x) {
  int y;
  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: Avoid using the result of an assignment operator '='
  return y = x;
}

void test_separate_assignment_compliant(int x) {
  // OK - assignment is a statement, not a sub-expression
  int y;
  y = x;
  if (y) {
  }
}

void test_initialization_compliant(int x) {
  // OK - this is initialization, not assignment
  int y = x;
  (void)y;
}

void test_compound_assignment_compliant(int x) {
  // OK - compound assignment as statement
  int y = 10;
  y += x;
  (void)y;
}
