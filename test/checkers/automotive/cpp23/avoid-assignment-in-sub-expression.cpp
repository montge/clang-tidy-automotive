// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-6.2 %t -- -- -std=c++17
// Test for automotive-cpp23-req-6.2: assignments shall not be used as sub-expressions

void test_assignment_in_condition_violation(int x) {
  int y;
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: assignment used in sub-expression
  if (y = x) {
    // action
  }
}

void test_assignment_in_while_violation(int x) {
  int y;
  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: assignment used in sub-expression
  while (y = x) {
    x--;
  }
}

void test_assignment_in_for_condition_violation(int n) {
  int i;
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: assignment used in sub-expression
  for (i = 0; i = n; i++) {
    n--;
  }
}

void test_chained_assignment_violation(int x) {
  int a, b;
  // CHECK-MESSAGES: warning: assignment used in sub-expression
  a = b = x;
}

void test_assignment_in_return_violation(int x) {
  int y;
  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: assignment used in sub-expression
  return y = x;
}

void test_separate_assignment_compliant(int x) {
  // OK - assignment is a statement, not a sub-expression
  int y;
  y = x;
  if (y) {
    // action
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
