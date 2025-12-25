// RUN: %check_clang_tidy %s automotive-cpp23-req-9.5.1 %t

// MISRA C++:2023 Rule 9.5.1
// A for-loop iteration variable should not be modified in the body of the loop.

void test_loop_var_modification() {
  // Non-compliant: loop variable modified with increment operator
  for (int i = 0; i < 10; i++) {
    i++;  // CHECK-MESSAGES: :[[@LINE]]:5: warning: loop variable 'i' should not be modified in the loop body [automotive-cpp23-req-9.5.1]
  }

  // Non-compliant: loop variable modified with decrement operator
  for (int j = 10; j > 0; j--) {
    j--;  // CHECK-MESSAGES: :[[@LINE]]:5: warning: loop variable 'j' should not be modified in the loop body [automotive-cpp23-req-9.5.1]
  }

  // Non-compliant: loop variable modified with assignment
  for (int k = 0; k < 10; k++) {
    k = k + 2;  // CHECK-MESSAGES: :[[@LINE]]:7: warning: loop variable 'k' should not be modified in the loop body [automotive-cpp23-req-9.5.1]
  }

  // Non-compliant: loop variable modified with compound assignment
  for (int m = 0; m < 10; m++) {
    m += 2;  // CHECK-MESSAGES: :[[@LINE]]:7: warning: loop variable 'm' should not be modified in the loop body [automotive-cpp23-req-9.5.1]
  }

  // Non-compliant: prefix increment
  for (int n = 0; n < 10; n++) {
    ++n;  // CHECK-MESSAGES: :[[@LINE]]:5: warning: loop variable 'n' should not be modified in the loop body [automotive-cpp23-req-9.5.1]
  }
}

void test_compliant_loops() {
  int value = 0;

  // Compliant: loop variable not modified in body
  for (int i = 0; i < 10; i++) {
    value++;
  }

  // Compliant: modification only in increment expression
  for (int i = 0; i < 10; i += 2) {
    value = i * 2;
  }

  // Compliant: different variable modified
  for (int i = 0; i < 10; i++) {
    int j = i + 1;
    j++;
  }

  // Compliant: using loop variable in expressions without modifying it
  for (int i = 0; i < 10; i++) {
    value = i * 2;
  }
}

void test_nested_loops() {
  // Non-compliant: outer loop variable modified
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      // Compliant: inner loop variable not modified
    }
    i++;  // CHECK-MESSAGES: :[[@LINE]]:5: warning: loop variable 'i' should not be modified in the loop body [automotive-cpp23-req-9.5.1]
  }

  // Compliant: only inner loop variable modified (and caught)
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 5; j++) {
      j++;  // CHECK-MESSAGES: :[[@LINE]]:7: warning: loop variable 'j' should not be modified in the loop body [automotive-cpp23-req-9.5.1]
    }
  }
}
