// RUN: %check_clang_tidy %s automotive-c25-req-22.19 %t

// MISRA C:2025 Rule 22.19: A condition variable shall be associated with
// at most one mutex object

typedef int cnd_t;
typedef int mtx_t;
typedef int pthread_cond_t;
typedef int pthread_mutex_t;

int cnd_wait(cnd_t *cond, mtx_t *mtx);
int cnd_timedwait(cnd_t *cond, mtx_t *mtx, const void *ts);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mtx);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mtx, const void *ts);

cnd_t g_cond;
mtx_t g_mutex1;
mtx_t g_mutex2;

// Non-compliant: condition variable used with two different mutexes
void multiple_mutexes_c11(void) {
  cnd_wait(&g_cond, &g_mutex1);
  cnd_wait(&g_cond, &g_mutex2);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: condition variable 'g_cond' is used with multiple mutexes
}

pthread_cond_t g_pcond;
pthread_mutex_t g_pmutex1;
pthread_mutex_t g_pmutex2;

// Non-compliant: pthread condition variable with multiple mutexes
void multiple_mutexes_pthread(void) {
  pthread_cond_wait(&g_pcond, &g_pmutex1);
  pthread_cond_wait(&g_pcond, &g_pmutex2);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: condition variable 'g_pcond' is used with multiple mutexes
}

// Compliant: same mutex used consistently
void same_mutex(void) {
  cnd_wait(&g_cond, &g_mutex1);
  cnd_timedwait(&g_cond, &g_mutex1, (void*)0);  // OK - same mutex
}

// Compliant: different condition variables with different mutexes
void different_condvars(void) {
  cnd_t cond_a, cond_b;
  mtx_t mutex_a, mutex_b;
  cnd_wait(&cond_a, &mutex_a);  // OK
  cnd_wait(&cond_b, &mutex_b);  // OK - different condvar
}

// Non-compliant: timedwait with different mutex than previous wait
cnd_t g_cond2;
mtx_t g_mtx_a, g_mtx_b;
void timedwait_different_mutex(void) {
  cnd_wait(&g_cond2, &g_mtx_a);
  cnd_timedwait(&g_cond2, &g_mtx_b, (void*)0);  // CHECK-MESSAGES: :[[@LINE]]:3: warning: condition variable 'g_cond2' is used with multiple mutexes
}
