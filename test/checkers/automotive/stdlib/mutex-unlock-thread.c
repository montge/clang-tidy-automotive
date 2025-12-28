// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-req-22.17 %t

// MISRA C:2025 Rule 22.17
// A mutex shall be unlocked from the same thread that locked it

#include <threads.h>

mtx_t global_mutex;

void lock_function() {
  mtx_lock(&global_mutex);
}

void unlock_function() {
  // CHECK-MESSAGES: :[[@LINE+2]]:3: warning: mutex 'global_mutex' locked in function 'lock_function' but unlocked in function 'unlock_function'; ensure lock and unlock occur in the same thread (MISRA C:2025 Rule 22.17)
  // CHECK-MESSAGES: :[[@LINE-6]]:3: note: mutex locked here
  mtx_unlock(&global_mutex);
}

void proper_usage() {
  mtx_t local_mutex;
  mtx_init(&local_mutex, mtx_plain);

  mtx_lock(&local_mutex);
  // ... critical section ...
  mtx_unlock(&local_mutex);  // No warning - same function

  mtx_destroy(&local_mutex);
}

void missing_unlock() {
  mtx_t mutex;
  mtx_init(&mutex, mtx_plain);

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: mutex 'mutex' locked in function 'missing_unlock' but not unlocked in the same translation unit
  mtx_lock(&mutex);

  // Missing unlock - will be reported at end of TU
}

void test_trylock() {
  mtx_t mutex;
  mtx_init(&mutex, mtx_plain);

  if (mtx_trylock(&mutex) == thrd_success) {
    mtx_unlock(&mutex);  // No warning - same function
  }

  mtx_destroy(&mutex);
}

// Note: This check provides best-effort detection. Full enforcement requires
// runtime thread tracking. Manual review is needed to verify correct usage.
