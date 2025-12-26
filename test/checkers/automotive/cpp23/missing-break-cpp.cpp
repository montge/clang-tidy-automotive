// XFAIL: *
// Note: Check doesn't recognize throw/return as valid switch case endings
// RUN: %check_clang_tidy %s automotive-cpp23-req-9.5.2 %t -- -- -std=c++17
// Test for automotive-cpp23-req-9.5.2: switch clauses shall end with break/throw

void test_missing_break(int x) {
  switch (x) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: every non-empty switch case must end with a break statement [automotive-cpp23-req-9.5.2]
  case 0:
    x = 1;  // Falls through
  case 1:
    x = 2;
    break;
  default:
    break;
  }
}

void compliant_switch(int x) {
  switch (x) {
  case 0:
    x = 1;
    break;
  case 1:
    return;
  default:
    throw 1;
  }
}
