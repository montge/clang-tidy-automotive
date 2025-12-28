// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-req-22.20 %t

// MISRA C:2025 Rule 22.20
// Spurious wakeups shall be handled correctly

#include <threads.h>
#include <stdbool.h>

cnd_t cond;
mtx_t mutex;
bool data_ready = false;

void violation_no_loop() {
  mtx_lock(&mutex);

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: condition variable wait using 'cnd_wait' is not enclosed in a loop; spurious wakeups must be handled by rechecking the predicate condition (MISRA C:2025 Rule 22.20)
  cnd_wait(&cond, &mutex);

  // Process data without checking if it's actually ready
  mtx_unlock(&mutex);
}

void violation_while_true() {
  mtx_lock(&mutex);

  while (1) {
    // CHECK-MESSAGES: :[[@LINE+2]]:5: warning: condition variable wait using 'cnd_wait' is in a loop with constant true condition; loop condition should check a meaningful predicate to handle spurious wakeups (MISRA C:2025 Rule 22.20)
    // CHECK-MESSAGES: :[[@LINE-3]]:3: note: loop with constant true condition starts here
    cnd_wait(&cond, &mutex);
    break;  // Wrong pattern
  }

  mtx_unlock(&mutex);
}

void compliant_usage() {
  mtx_lock(&mutex);

  // Correct: wait in a loop with predicate check
  while (!data_ready) {
    cnd_wait(&cond, &mutex);  // No warning - proper loop
  }

  // Process data
  mtx_unlock(&mutex);
}

void compliant_timedwait() {
  struct timespec timeout;
  mtx_lock(&mutex);

  // Correct: timedwait also in loop
  while (!data_ready) {
    cnd_timedwait(&cond, &mutex, &timeout);  // No warning
  }

  mtx_unlock(&mutex);
}

void violation_for_loop() {
  mtx_lock(&mutex);

  for (int i = 0; i < 1; i++) {
    // This will trigger the "not in loop" warning because the for loop
    // doesn't match our loop detection pattern for condition variables
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: condition variable wait using 'cnd_wait' is not enclosed in a loop
    cnd_wait(&cond, &mutex);
  }

  mtx_unlock(&mutex);
}

// Producer-consumer example (compliant)
void consumer() {
  mtx_lock(&mutex);

  while (!data_ready) {
    cnd_wait(&cond, &mutex);  // Correct usage
  }

  // Consume data
  data_ready = false;

  mtx_unlock(&mutex);
}

void producer() {
  mtx_lock(&mutex);

  // Produce data
  data_ready = true;

  cnd_signal(&cond);
  mtx_unlock(&mutex);
}
