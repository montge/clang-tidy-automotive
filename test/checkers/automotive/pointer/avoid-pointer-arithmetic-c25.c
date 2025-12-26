// RUN: %check_clang_tidy %s automotive-c25-adv-18.4 %t -- -- -std=c11
// Test for automotive-c25-adv-18.4: pointer arithmetic shall not be used (MISRA C:2025)

#include <stddef.h>

void test_ptr_addition_violation(void) {
  int arr[10];
  int *ptr = arr;

  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: pointer arithmetic using '+' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
  ptr = ptr + 5;
  (void)ptr;
}

void test_ptr_subtraction_violation(void) {
  int arr[10];
  int *ptr = &arr[9];

  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: pointer arithmetic using '-' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
  ptr = ptr - 3;
  (void)ptr;
}

void test_ptr_increment_not_detected(void) {
  // Note: check does not currently detect increment/decrement operators
  int arr[10];
  int *ptr = arr;

  ptr++;
  (void)ptr;
}

void test_ptr_decrement_not_detected(void) {
  // Note: check does not currently detect increment/decrement operators
  int arr[10];
  int *ptr = &arr[5];

  ptr--;
  (void)ptr;
}

void test_ptr_compound_add_violation(void) {
  int arr[10];
  int *ptr = arr;

  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: compound assignment '+=' operator used on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
  ptr += 2;
  (void)ptr;
}

void test_ptr_difference_violation(void) {
  int arr[10];
  int *start = arr;
  int *end = &arr[9];

  // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: pointer difference using '-' operator on pointer types 'int *' and 'int *' [automotive-c25-adv-18.4]
  ptrdiff_t diff = end - start;
  (void)diff;
}

void test_array_subscript_compliant(void) {
  // OK - array subscript is allowed
  int arr[10];
  arr[5] = 42;
  int x = arr[3];
  (void)x;
}

void test_ptr_assignment_compliant(void) {
  // OK - pointer assignment without arithmetic
  int arr[10];
  int *ptr = arr;
  int *ptr2 = &arr[5];  // OK - taking address of array element
  (void)ptr;
  (void)ptr2;
}
