// RUN: %check_clang_tidy %s automotive-c25-req-17.6 %t -- -- -std=c11
// Test for automotive-c25-req-17.6: static in array parameter shall be honored

#include <stddef.h>

// CHECK-MESSAGES: :[[@LINE+2]]:32: warning: array parameter with static qualifier
void func_with_static_array(int arr[static 10]) {
  (void)arr;
}

// CHECK-MESSAGES: :[[@LINE+2]]:35: warning: array parameter with static qualifier
void func_with_static_array_2d(int arr[static 5][10]) {
  (void)arr;
}

void test_call_with_smaller_array(void) {
  int small_arr[5];
  // The check should flag that we're passing smaller array than declared
  func_with_static_array(small_arr);  // Violation at call site
}

void test_call_with_null(void) {
  // The check should flag that we're passing NULL for static array
  func_with_static_array(NULL);  // Violation - NULL for static array
}

// Compliant - no static qualifier
void func_normal_array(int arr[], size_t size) {
  (void)arr;
  (void)size;
}

// Compliant - static with sufficient size
void test_call_with_sufficient_size(void) {
  int large_arr[20];
  func_with_static_array(large_arr);  // OK - array is large enough
}

void test_call_normal_array(void) {
  int arr[5];
  func_normal_array(arr, 5);  // OK - no static constraint
}

// Pointer parameter - not affected by this rule
void func_pointer(int *ptr) {
  (void)ptr;
}

void test_call_pointer(void) {
  int arr[5];
  func_pointer(arr);  // OK - pointer parameter
  func_pointer(NULL);  // OK - NULL is valid for pointer
}
