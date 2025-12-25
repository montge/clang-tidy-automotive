// RUN: %check_clang_tidy %s automotive-cpp23-adv-9.6.4 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-9.6.4: goto statement should not be used

void test_goto() {
  int x = 0;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: avoid goto statement
  goto label;
label:
  x = 1;
  (void)x;
}

void compliant_function() {
  // OK - no goto
  for (int i = 0; i < 10; i++) {
    if (i == 5) break;
  }
}
