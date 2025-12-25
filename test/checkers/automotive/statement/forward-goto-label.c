// RUN: %check_clang_tidy %s automotive-forward-goto-label %t -- -- -std=c11
// Test for automotive-forward-goto-label: goto shall only jump forward

void test_backward_goto_violation(void) {
start:
  int x = 0;
  x++;
  if (x < 10) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: goto statement jumps backward to label 'start'
    goto start;
  }
}

void test_forward_goto_compliant(int condition) {
  if (condition) {
    goto end;  // OK - forward jump
  }

  // Some code here
  int x = 0;
  (void)x;

end:
  return;
}

void test_multiple_backward_gotos(void) {
loop_start:
  int i = 0;
  i++;

  if (i < 5) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: goto statement jumps backward to label 'loop_start'
    goto loop_start;
  }

  if (i < 10) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: goto statement jumps backward to label 'loop_start'
    goto loop_start;
  }
}

void test_forward_error_handling(int err) {
  if (err == 1) {
    goto cleanup;  // OK - forward jump for error handling
  }
  if (err == 2) {
    goto cleanup;  // OK - forward jump for error handling
  }

  // Normal operation
  return;

cleanup:
  // Cleanup code
  return;
}
