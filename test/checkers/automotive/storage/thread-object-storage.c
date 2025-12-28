// RUN: %check_clang_tidy %s automotive-c25-req-22.13 %t -- -- -std=c11

// Test for thread object storage duration requirements

// Simulated thread types (mimic C11 threads.h)
typedef struct { int id; } thrd_t;
typedef struct { int lock; } mtx_t;
typedef struct { int flag; } cnd_t;
typedef struct { void *data; } tss_t;

// Compliant: static storage duration (file scope)
thrd_t global_thread;
mtx_t global_mutex;
cnd_t global_cond;
tss_t global_tss;

void test_automatic_storage(void) {
  // Non-compliant: automatic storage duration
  mtx_t local_mutex;
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: thread synchronization object 'local_mutex' has automatic storage duration; use static storage to ensure lifetime across thread operations [automotive-c25-req-22.13]

  thrd_t local_thread;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: thread synchronization object 'local_thread' has automatic storage duration; use static storage to ensure lifetime across thread operations [automotive-c25-req-22.13]

  cnd_t local_cond;
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: thread synchronization object 'local_cond' has automatic storage duration; use static storage to ensure lifetime across thread operations [automotive-c25-req-22.13]

  tss_t local_tss;
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: thread synchronization object 'local_tss' has automatic storage duration; use static storage to ensure lifetime across thread operations [automotive-c25-req-22.13]

  (void)local_mutex;
  (void)local_thread;
  (void)local_cond;
  (void)local_tss;
}

void test_static_local(void) {
  // Compliant: static storage duration
  static mtx_t static_mutex;
  static thrd_t static_thread;
  static cnd_t static_cond;
  static tss_t static_tss;

  (void)static_mutex;
  (void)static_thread;
  (void)static_cond;
  (void)static_tss;
}

void test_thread_local(void) {
  // Non-compliant: thread storage duration
  _Thread_local static mtx_t thread_mutex;
  // CHECK-MESSAGES: :[[@LINE-1]]:30: warning: thread synchronization object 'thread_mutex' has thread storage duration; use static storage to ensure shared access across threads [automotive-c25-req-22.13]

  _Thread_local static thrd_t thread_thrd;
  // CHECK-MESSAGES: :[[@LINE-1]]:31: warning: thread synchronization object 'thread_thrd' has thread storage duration; use static storage to ensure shared access across threads [automotive-c25-req-22.13]

  (void)thread_mutex;
  (void)thread_thrd;
}

// Compliant: file scope with extern
extern mtx_t extern_mutex;
extern cnd_t extern_cond;

// Non-compliant: file scope with _Thread_local
_Thread_local thrd_t tls_thread;
// CHECK-MESSAGES: :[[@LINE-1]]:22: warning: thread synchronization object 'tls_thread' has thread storage duration; use static storage to ensure shared access across threads [automotive-c25-req-22.13]

_Thread_local mtx_t tls_mutex;
// CHECK-MESSAGES: :[[@LINE-1]]:21: warning: thread synchronization object 'tls_mutex' has thread storage duration; use static storage to ensure shared access across threads [automotive-c25-req-22.13]
