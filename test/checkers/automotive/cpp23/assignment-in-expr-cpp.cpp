// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-adv-7.4.2 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-7.4.2: assignment in sub-expression

void test_assignment_in_expr() {
  int x = 0;
  int y = 0;
  
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: assignment operator in sub-expression
  if ((x = 5) != 0) {}
  
  // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: assignment operator in sub-expression
  while ((x = y = 10) > 0) { break; }
  
  // OK - assignment as statement
  x = 5;
  y = x;
}
