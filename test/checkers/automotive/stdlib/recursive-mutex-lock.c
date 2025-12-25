// RUN: %check_clang_tidy %s automotive-c25-mand-22.18 %t

// MISRA C:2025 Rule 22.18: A non-recursive mutex shall not be recursively locked

typedef int mtx_t;
typedef int pthread_mutex_t;

int mtx_lock(mtx_t *mtx);
int mtx_unlock(mtx_t *mtx);
int mtx_trylock(mtx_t *mtx);
int mtx_timedlock(mtx_t *mtx, const void *ts);

int pthread_mutex_lock(pthread_mutex_t *mtx);
int pthread_mutex_unlock(pthread_mutex_t *mtx);
int pthread_mutex_trylock(pthread_mutex_t *mtx);

mtx_t g_mutex;
pthread_mutex_t g_pmutex;

// Non-compliant: recursive lock of same mutex
void recursive_lock_c11(void) {
  mtx_lock(&g_mutex);
  mtx_lock(&g_mutex);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: non-recursive mutex 'g_mutex' is being locked recursively
  mtx_unlock(&g_mutex);
  mtx_unlock(&g_mutex);
}

// Non-compliant: recursive lock using pthread
void recursive_lock_pthread(void) {
  pthread_mutex_lock(&g_pmutex);
  pthread_mutex_lock(&g_pmutex);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: non-recursive mutex 'g_pmutex' is being locked recursively
  pthread_mutex_unlock(&g_pmutex);
  pthread_mutex_unlock(&g_pmutex);
}

// Compliant: different mutexes
void different_mutexes(void) {
  mtx_t local_mutex;
  mtx_lock(&g_mutex);
  mtx_lock(&local_mutex);  // OK - different mutex
  mtx_unlock(&local_mutex);
  mtx_unlock(&g_mutex);
}

// Compliant: lock after unlock
void lock_after_unlock(void) {
  mtx_lock(&g_mutex);
  mtx_unlock(&g_mutex);
  mtx_lock(&g_mutex);  // OK - was unlocked first
  mtx_unlock(&g_mutex);
}

// Non-compliant: trylock after lock
void trylock_after_lock(void) {
  mtx_lock(&g_mutex);
  mtx_trylock(&g_mutex);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: non-recursive mutex 'g_mutex' is being locked recursively
  mtx_unlock(&g_mutex);
}

// Non-compliant: timedlock after lock
void timedlock_after_lock(void) {
  mtx_lock(&g_mutex);
  mtx_timedlock(&g_mutex, (void*)0);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: non-recursive mutex 'g_mutex' is being locked recursively
  mtx_unlock(&g_mutex);
}

// Compliant: nested locks with different mutexes (lock ordering)
void proper_lock_ordering(void) {
  mtx_t mutex_a, mutex_b;
  mtx_lock(&mutex_a);
  mtx_lock(&mutex_b);  // OK - different mutex
  mtx_unlock(&mutex_b);
  mtx_unlock(&mutex_a);
}
