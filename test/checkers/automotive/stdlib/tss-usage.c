// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-req-21.23 %t -- -- -std=c11
// Test for automotive-c25-req-21.23: Thread-specific storage functions

#include <threads.h>
#include <stdlib.h>

void cleanup(void* data) {
  free(data);
}

tss_t key1;
tss_t key2;

void test_tss_create_null_destructor(void) {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: tss_create called with NULL destructor; consider providing a cleanup function to prevent resource leaks
  tss_create(&key1, NULL);
}

void test_tss_create_with_destructor(void) {
  // OK - tss_create with destructor
  tss_create(&key2, cleanup);
}

void test_tss_set_with_local_variable(void) {
  int local_var = 42;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: tss_set called with pointer to local variable; this may lead to undefined behavior when the variable goes out of scope
  tss_set(key1, &local_var);
}

void test_tss_set_with_heap_memory(void) {
  void* data = malloc(sizeof(int));

  // OK - tss_set with heap-allocated memory
  tss_set(key2, data);
}

void test_tss_get(void) {
  // OK - tss_get with valid key
  void* data = tss_get(key1);
}

void test_tss_delete(void) {
  // OK - tss_delete
  tss_delete(key1);
}
