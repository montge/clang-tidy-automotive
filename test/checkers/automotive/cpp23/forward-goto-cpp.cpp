// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-9.6.3 %t -- -- -std=c++17
// Test for automotive-cpp23-req-9.6.3: goto shall only jump forward

void test_backward_goto() {
label:
  int x = 0;
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: goto jumps backward to label 'label'
  goto label;
  (void)x;
}

void compliant_goto() {
  int x = 0;
  goto end;  // OK - forward jump
end:
  (void)x;
}
