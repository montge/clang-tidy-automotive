// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-mand-22.20 %t

// Mock declarations for threads.h
typedef struct { int __data; } mtx_t;
int mtx_lock(mtx_t *);
int mtx_unlock(mtx_t *);
int mtx_trylock(mtx_t *);

mtx_t mutex1;
mtx_t mutex2;

void nested_lock_example(void) {
  // Violation: nested synchronization calls
  mtx_lock(&mutex1);
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: nested call to thread synchronization function 'mtx_lock' detected; calls shall not be nested
  mtx_lock(&mutex2);
  // CHECK-MESSAGES: :[[@LINE-3]]:3: note: parent synchronization call is here

  mtx_unlock(&mutex2);
  mtx_unlock(&mutex1);
}

void proper_usage(void) {
  // Compliant: no nested locks
  mtx_lock(&mutex1);
  // Do work
  mtx_unlock(&mutex1);

  // Separate lock/unlock
  mtx_lock(&mutex2);
  // Do work
  mtx_unlock(&mutex2);
}
