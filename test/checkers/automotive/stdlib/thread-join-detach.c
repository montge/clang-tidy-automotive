// RUN: %check_clang_tidy %s automotive-c25-req-22.11 %t

// MISRA C:2025 Rule 22.11: A thread that was previously joined or detached
// shall not be subsequently joined or detached

typedef unsigned long thrd_t;
typedef unsigned long pthread_t;

int thrd_create(thrd_t *thr, void *(*func)(void*), void *arg);
int thrd_join(thrd_t thr, int *res);
int thrd_detach(thrd_t thr);

int pthread_create(pthread_t *thr, const void *attr, void *(*func)(void*), void *arg);
int pthread_join(pthread_t thr, void **res);
int pthread_detach(pthread_t thr);

void *thread_func(void *arg) { return 0; }

// Non-compliant: double join
void double_join_c11(void) {
  thrd_t t;
  thrd_create(&t, thread_func, (void*)0);
  thrd_join(t, (void*)0);
  thrd_join(t, (void*)0);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: thread 't' has already been joined
}

// Non-compliant: double join with pthread
void double_join_pthread(void) {
  pthread_t t;
  pthread_create(&t, (void*)0, thread_func, (void*)0);
  pthread_join(t, (void**)0);
  pthread_join(t, (void**)0);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: thread 't' has already been joined
}

// Non-compliant: double detach
void double_detach(void) {
  thrd_t t;
  thrd_create(&t, thread_func, (void*)0);
  thrd_detach(t);
  thrd_detach(t);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: thread 't' has already been detached
}

// Non-compliant: join after detach
void join_after_detach(void) {
  thrd_t t;
  thrd_create(&t, thread_func, (void*)0);
  thrd_detach(t);
  thrd_join(t, (void*)0);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: thread 't' was detached; joining a detached thread causes undefined behavior
}

// Non-compliant: detach after join
void detach_after_join(void) {
  thrd_t t;
  thrd_create(&t, thread_func, (void*)0);
  thrd_join(t, (void*)0);
  thrd_detach(t);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: thread 't' was joined; detaching a joined thread causes undefined behavior
}

// Compliant: different threads
void different_threads(void) {
  thrd_t t1, t2;
  thrd_create(&t1, thread_func, (void*)0);
  thrd_create(&t2, thread_func, (void*)0);
  thrd_join(t1, (void*)0);  // OK
  thrd_detach(t2);          // OK - different thread
}

// Compliant: single join or detach
void single_operation(void) {
  thrd_t t1, t2;
  thrd_create(&t1, thread_func, (void*)0);
  thrd_create(&t2, thread_func, (void*)0);
  thrd_join(t1, (void*)0);   // OK - single join
  thrd_detach(t2);           // OK - single detach
}
