// RUN: %check_clang_tidy %s automotive-cpp23-req-9.4.1 %t -- -- -std=c++17
// Test for automotive-cpp23-req-9.4.1: if-else-if chains shall end with else

void test_missing_else(int x) {
  if (x == 0) {
    return;
  } else if (x == 1) {
    return;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: if...else if chain should terminate with an else clause [automotive-cpp23-req-9.4.1]
  }  // Missing final else
}

void compliant_function(int x) {
  // OK - single if statement
  if (x == 0) {
    return;
  }

  // OK - if-else-if with final else
  if (x == 1) {
    return;
  } else if (x == 2) {
    return;
  } else {
    return;
  }
}
