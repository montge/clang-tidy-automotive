// RUN: %check_clang_tidy %s automotive-c25-req-18.1 %t -- -- -std=c11
// Test for automotive-c25-req-18.1: pointer arithmetic shall not exceed array bounds

void test_ptr_arithmetic_overflow_violation(void) {
  int arr[10];
  int *ptr = arr;

  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: pointer arithmetic may exceed array bounds
  ptr = ptr + 15;  // Exceeds array bounds
  (void)ptr;
}

void test_negative_ptr_arithmetic_violation(void) {
  int arr[10];
  int *ptr = &arr[5];

  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: pointer arithmetic may exceed array bounds
  ptr = ptr - 10;  // Goes before array start
  (void)ptr;
}

void test_ptr_increment_loop_violation(void) {
  int arr[5];
  // CHECK-MESSAGES: :[[@LINE+3]]:5: warning: pointer arithmetic may exceed array bounds
  for (int *ptr = arr; ptr < arr + 10; ptr++) {
    // Loop goes beyond array bounds
    *ptr = 0;
  }
}

void test_safe_ptr_arithmetic_compliant(void) {
  int arr[10];
  int *ptr = arr;

  // OK - within bounds
  ptr = ptr + 5;
  ptr = ptr - 3;
  (void)ptr;
}

void test_ptr_to_one_past_end_compliant(void) {
  int arr[10];
  int *ptr = arr;

  // OK - pointer to one past the end is allowed
  ptr = arr + 10;  // Valid one-past-end pointer
  (void)ptr;
}

void test_safe_loop_compliant(void) {
  int arr[10];

  // OK - loop stays within bounds
  for (int *ptr = arr; ptr < arr + 10; ptr++) {
    *ptr = 0;
  }
}
