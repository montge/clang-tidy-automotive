// RUN: %check_clang_tidy %s automotive-c23-req-18.1 %t -- -- -std=c11
// Test for automotive-c23-req-18.1: relational operator applied to pointers

void test_ptr_relational_in_loop(void) {
  int arr[10];
  // CHECK-MESSAGES: :[[@LINE+1]]:28: warning: relational operator applied to pointers; ensure they address elements of the same array [automotive-c23-req-18.1]
  for (int *ptr = arr; ptr < arr + 10; ptr++) {
    *ptr = 0;
  }
}

void test_ptr_relational_separate_pointers(void) {
  int arr1[10];
  int arr2[10];
  int *p1 = arr1;
  int *p2 = arr2;

  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: relational operator applied to pointers; ensure they address elements of the same array [automotive-c23-req-18.1]
  if (p1 < p2) {  // Comparing pointers to different arrays
    (void)p1;
  }
  (void)p2;
}

void test_ptr_comparison_same_array_compliant(void) {
  int arr[10];
  int *p1 = &arr[2];
  int *p2 = &arr[5];

  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: relational operator applied to pointers; ensure they address elements of the same array [automotive-c23-req-18.1]
  if (p1 < p2) {  // Flagged - developer should verify same array
    (void)p1;
  }
  (void)p2;
}

void test_ptr_equality_compliant(void) {
  int arr[10];
  int *p1 = &arr[2];
  int *p2 = &arr[5];

  // OK - equality comparison is allowed
  if (p1 == p2) {
    (void)p1;
  }
  if (p1 != p2) {
    (void)p2;
  }
}
