// RUN: %check_clang_tidy %s automotive-cpp23-adv-7.4.2 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-7.4.2: assignment in sub-expression

void test_assignment_in_expr() {
  int x = 0;
  int y = 0;

  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: Avoid using the result of an assignment operator '=' [automotive-cpp23-adv-7.4.2]
  if ((x = 5) != 0) {}

  // CHECK-MESSAGES: :[[@LINE+2]]:13: warning: Avoid using the result of an assignment operator '=' [automotive-cpp23-adv-7.4.2]
  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: Avoid using the result of an assignment operator '=' [automotive-cpp23-adv-7.4.2]
  while ((x = y = 10) > 0) { break; }

  // OK - assignment as statement
  x = 5;
  y = x;
}
