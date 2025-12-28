// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-req-21.26 %t -- -- -std=c11
// Test for automotive-c25-req-21.26: mtx_timedlock mutex type check

#include <threads.h>
#include <time.h>

mtx_t mutex_plain;
mtx_t mutex_timed;
mtx_t mutex_timed_recursive;
mtx_t mutex_unknown;

void init_mutexes(void) {
  mtx_init(&mutex_plain, mtx_plain);
  mtx_init(&mutex_timed, mtx_timed);
  mtx_init(&mutex_timed_recursive, mtx_timed | mtx_recursive);
}

void test_mtx_timedlock_with_plain_mutex(void) {
  struct timespec ts;
  ts.tv_sec = 1;
  ts.tv_nsec = 0;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: mtx_timedlock called on mutex 'mutex_plain' that does not support timeout; mutex must be initialized with mtx_timed or (mtx_timed | mtx_recursive)
  mtx_timedlock(&mutex_plain, &ts);
}

void test_mtx_timedlock_with_timed_mutex(void) {
  struct timespec ts;
  ts.tv_sec = 1;
  ts.tv_nsec = 0;

  // OK - mtx_timedlock with mtx_timed
  mtx_timedlock(&mutex_timed, &ts);
}

void test_mtx_timedlock_with_timed_recursive_mutex(void) {
  struct timespec ts;
  ts.tv_sec = 1;
  ts.tv_nsec = 0;

  // OK - mtx_timedlock with mtx_timed | mtx_recursive
  mtx_timedlock(&mutex_timed_recursive, &ts);
}

void test_mtx_timedlock_with_unknown_mutex(void) {
  struct timespec ts;
  ts.tv_sec = 1;
  ts.tv_nsec = 0;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: mtx_timedlock called on mutex 'mutex_unknown' whose type cannot be verified; ensure mutex is initialized with mtx_timed or (mtx_timed | mtx_recursive)
  mtx_timedlock(&mutex_unknown, &ts);
}
