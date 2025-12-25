// RUN: %check_clang_tidy %s automotive-cpp23-req-9.5.3 %t -- -- -std=c++17
// Test for automotive-cpp23-req-9.5.3: switch statements shall have default

void test_missing_default(int x) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: missing default statement in switch statement [automotive-cpp23-req-9.5.3]
  switch (x) {
  case 0:
    x = 1;
    break;
  case 1:
    x = 2;
    break;
  }
}

void compliant_switch(int x) {
  switch (x) {
  case 0:
    x = 1;
    break;
  default:
    break;
  }
}
