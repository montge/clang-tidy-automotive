// RUN: %check_clang_tidy %s automotive-cpp23-req-8.14 %t -- -- -std=c++17
// Test for automotive-cpp23-req-8.14: logical operator right operand shall not have side effects

int x = 0;

void test_logical_side_effect() {
  bool a = true;
  bool b = false;
  
  // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: right-hand operand of logical operator has side effects
  if (a && (x++ > 0)) {}
  
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: right-hand operand of logical operator has side effects
  if (a || x++) {}
  
  // OK - no side effects
  if (a && b) {}
  if (a || (x > 0)) {}
}
