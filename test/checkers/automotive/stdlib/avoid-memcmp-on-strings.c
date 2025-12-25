// RUN: %check_clang_tidy %s automotive-c25-req-21.14 %t -- -- -std=c11
// Test for automotive-c25-req-21.14: memcmp shall not compare strings

#include <string.h>

void test_memcmp_on_strings(void) {
  char str1[] = "hello";
  char str2[] = "world";
  
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: memcmp shall not be used to compare null-terminated strings
  memcmp(str1, str2, 5);
  
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: memcmp shall not be used to compare null-terminated strings
  memcmp("hello", "world", 5);
  
  // OK - comparing binary data
  int arr1[] = {1, 2, 3};
  int arr2[] = {1, 2, 3};
  memcmp(arr1, arr2, sizeof(arr1));
}
