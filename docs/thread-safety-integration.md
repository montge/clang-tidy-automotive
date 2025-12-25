# Thread Safety Integration Guide

This guide explains how to maximize thread safety coverage by combining clang-tidy automotive checks with Clang's built-in Thread Safety Analysis (TSA).

## Overview

MISRA C:2025 Chapter 22 contains several thread safety rules. Some are **decidable** (can be checked statically) while others are **undecidable** (require runtime analysis or annotations).

### Decidable Rules (Implemented as clang-tidy checks)

| Rule | Description | Check Name |
|------|-------------|------------|
| 22.8 | errno shall be zero before call | `automotive-c25-req-22.8` |
| 22.9 | errno shall be tested after call | `automotive-c25-req-22.9` |
| 22.11 | Thread join/detach lifecycle | `automotive-c25-req-22.11` |
| 22.13 | Thread object storage duration | `automotive-c25-req-22.13` |
| 22.18 | Non-recursive mutex recursion | `automotive-c25-mand-22.18` |
| 22.19 | Condition variable/mutex association | `automotive-c25-req-22.19` |

### Undecidable Rules (Require TSA or TSan)

| Rule | Description | Recommended Tool |
|------|-------------|------------------|
| 22.1 | Lock shall be held when accessing shared data | TSA annotations |
| 22.2 | Unlock shall only occur when lock is held | TSA annotations |
| 22.3 | Lock ordering to prevent deadlock | TSA + manual review |
| 22.4 | No data races on shared objects | ThreadSanitizer |
| 22.5 | Atomic operations consistency | TSA annotations |
| 22.16 | Mutex unlock from same thread | ThreadSanitizer |

## Using Clang Thread Safety Analysis (TSA)

TSA is a compile-time static analyzer that uses annotations to verify correct lock usage.

### Enable TSA Warnings

Add these flags to your build:

```bash
clang -Wthread-safety -Wthread-safety-negative your_file.c
```

Or use the provided clang-tidy config:

```bash
clang-tidy -config-file=config/clang-tidy-thread-safety.yaml your_file.c --
```

### Annotate Your Code

```c
#include <pthread.h>

// Define capability attribute for older compilers
#define CAPABILITY(x) __attribute__((capability(x)))
#define GUARDED_BY(x) __attribute__((guarded_by(x)))
#define REQUIRES(x) __attribute__((requires_capability(x)))
#define ACQUIRE(x) __attribute__((acquire_capability(x)))
#define RELEASE(x) __attribute__((release_capability(x)))
#define NO_THREAD_SAFETY_ANALYSIS __attribute__((no_thread_safety_analysis))

// Annotate your mutex
pthread_mutex_t data_mutex CAPABILITY("mutex");

// Annotate protected data
int shared_counter GUARDED_BY(data_mutex);

// Annotate functions that acquire/release locks
void lock_data(void) ACQUIRE(data_mutex) {
    pthread_mutex_lock(&data_mutex);
}

void unlock_data(void) RELEASE(data_mutex) {
    pthread_mutex_unlock(&data_mutex);
}

// Annotate functions that require lock to be held
void increment_counter(void) REQUIRES(data_mutex) {
    shared_counter++;  // TSA verifies lock is held
}

// Usage
void safe_increment(void) {
    lock_data();
    increment_counter();  // OK: lock is held
    unlock_data();
}

void unsafe_increment(void) {
    increment_counter();  // TSA WARNING: lock not held!
}
```

### TSA with C11 Threads

```c
#include <threads.h>

#define GUARDED_BY(x) __attribute__((guarded_by(x)))
#define REQUIRES(x) __attribute__((requires_capability(x)))

mtx_t mutex __attribute__((capability("mutex")));
int data GUARDED_BY(mutex);

void safe_access(void) REQUIRES(mutex) {
    data = 42;
}
```

## Using ThreadSanitizer (TSan)

For runtime detection of data races (MISRA 22.4, 22.16), use ThreadSanitizer:

```bash
# Compile with TSan
clang -fsanitize=thread -g your_file.c -o your_program

# Run - TSan will report any data races at runtime
./your_program
```

TSan catches:
- Data races between threads
- Lock order inversions (potential deadlocks)
- Use of uninitialized mutex
- Double lock/unlock errors
- Wrong thread unlock

## Recommended Workflow

1. **Static Analysis First** - Run clang-tidy with automotive checks:
   ```bash
   clang-tidy -config-file=config/clang-tidy-automotive-full.yaml src/*.c --
   ```

2. **Add TSA Annotations** - Annotate mutexes and protected data

3. **Enable TSA Warnings** - Build with `-Wthread-safety`

4. **Runtime Validation** - Test with ThreadSanitizer enabled

5. **CI Integration** - Run both static and runtime checks in CI

## Configuration Files

| File | Purpose |
|------|---------|
| `config/clang-tidy-automotive-full.yaml` | All MISRA checks + TSA warnings |
| `config/clang-tidy-thread-safety.yaml` | Thread-focused checks only |

## Limitations

- **Clang-tidy checks (22.11, 22.18, 22.19)**: Intra-procedural only, may miss cross-function issues
- **TSA**: Requires manual annotation, no runtime checking
- **TSan**: Runtime only, requires good test coverage

For ASIL-D compliance, use all three approaches together.
