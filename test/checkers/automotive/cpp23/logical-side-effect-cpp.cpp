// RUN: %check_clang_tidy %s automotive-cpp23-req-8.14 %t -- -- -std=c++17
// Test for automotive-cpp23-req-8.14: logical operator right operand shall not have side effects

int x = 0;

void test_logical_side_effect() {
  bool a = true;
  bool b = false;

  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: right operand of '&&' operator contains side effects that may not be evaluated due to short-circuit evaluation [automotive-cpp23-req-8.14]
  if (a && (x++ > 0)) {}

  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: right operand of '||' operator contains side effects that may not be evaluated due to short-circuit evaluation [automotive-cpp23-req-8.14]
  if (a || x++) {}

  // OK - no side effects
  if (a && b) {}
  if (a || (x > 0)) {}
}
