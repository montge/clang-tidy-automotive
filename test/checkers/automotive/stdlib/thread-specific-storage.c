// RUN: %check_clang_tidy %s automotive-c25-req-22.15 %t

// MISRA C:2025 Rule 22.15
// Thread-specific storage pointers shall be retrieved from the same thread

#include <threads.h>

tss_t global_key;

void test_tss_usage() {
  tss_t key;
  tss_create(&key, NULL);

  // Warning: retrieving TSS value
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: thread-specific storage key 'key' retrieved; ensure this is called from the same thread that set the value (MISRA C:2025 Rule 22.15)
  tss_get(key);

  tss_set(key, (void*)42);

  // Warning: retrieving TSS value
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: thread-specific storage key 'key' retrieved
  tss_get(key);

  tss_delete(key);
}

void test_global_key() {
  // Warning: retrieving from global key
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: thread-specific storage key 'global_key' retrieved
  tss_get(global_key);
}

// Note: This check provides best-effort warnings since full thread tracking
// is not feasible in static analysis. Manual review is required to ensure
// TSS keys are accessed from the correct threads.
