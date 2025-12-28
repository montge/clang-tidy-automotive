// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-req-22.16 %t

// MISRA C:2025 Rule 22.16
// An object shall not be accessed from competing threads without synchronization

#include <stdatomic.h>
#include <threads.h>

int global_counter = 0;  // Potentially shared
static int static_var = 0;  // Potentially shared

_Atomic int atomic_counter = 0;  // Safe - atomic
const int const_value = 42;  // Safe - read-only
_Thread_local int thread_local_var = 0;  // Safe - thread-local

void increment() {
  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: potential unsynchronized access to shared variable 'global_counter'; ensure proper synchronization (mutex, atomic operations) if accessed from multiple threads (MISRA C:2025 Rule 22.16)
  // CHECK-MESSAGES: :[[@LINE-9]]:5: note: variable declared here
  global_counter++;

  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: potential unsynchronized access to shared variable 'static_var'
  // CHECK-MESSAGES: :[[@LINE-12]]:12: note: variable declared here
  static_var++;

  // No warning - atomic operations are synchronized
  atomic_fetch_add(&atomic_counter, 1);

  // No warning - thread-local storage
  thread_local_var++;

  // No warning - const is read-only (though this line wouldn't compile)
  // const_value = 10;
}

void test_assignment() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: potential unsynchronized access to shared variable 'global_counter'
  global_counter = 100;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: potential unsynchronized access to shared variable 'static_var'
  static_var = 200;
}

void safe_access() {
  mtx_t mutex;
  mtx_init(&mutex, mtx_plain);

  // Still warns - check doesn't track mutex protection
  // (full dataflow analysis would be needed)
  mtx_lock(&mutex);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: potential unsynchronized access to shared variable 'global_counter'
  global_counter++;
  mtx_unlock(&mutex);

  mtx_destroy(&mutex);
}

// Note: This check provides heuristic warnings. Manual review is required
// to verify that proper synchronization is in place for multi-threaded access.
