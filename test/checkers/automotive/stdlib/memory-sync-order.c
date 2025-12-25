// RUN: %check_clang_tidy %s automotive-c25-req-21.25 %t
// Test for automotive-c25-req-21.25
// Related MISRA C:2025 Rule: 21.25

#include <stdatomic.h>
#include <stdbool.h>

atomic_int global_counter = 0;
atomic_bool flag = false;

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_violations(void) {
    int value;
    int expected = 5;
    int desired = 10;

    // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: atomic operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_load_explicit(&global_counter, memory_order_relaxed);

    // CHECK-MESSAGES: :[[@LINE+1]]:27: warning: atomic operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_store_explicit(&global_counter, 42, memory_order_release);

    // CHECK-MESSAGES: :[[@LINE+1]]:30: warning: atomic operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_exchange_explicit(&global_counter, 100, memory_order_acquire);

    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: atomic operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_fetch_add_explicit(&global_counter, 1, memory_order_relaxed);

    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: atomic operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_fetch_sub_explicit(&global_counter, 1, memory_order_release);

    // CHECK-MESSAGES: :[[@LINE+1]]:30: warning: atomic operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_fetch_or_explicit(&global_counter, 0xFF, memory_order_acquire);

    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: atomic operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_fetch_and_explicit(&global_counter, 0xF0, memory_order_acq_rel);

    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: atomic operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_fetch_xor_explicit(&global_counter, 0x0F, memory_order_consume);

    // CHECK-MESSAGES: :[[@LINE+2]]:45: warning: atomic operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    // CHECK-MESSAGES: :[[@LINE+1]]:45: warning: atomic operation shall use memory_order_seq_cst for failure ordering [automotive-c25-req-21.25]
    atomic_compare_exchange_strong_explicit(&global_counter, &expected, desired,
                                            memory_order_acquire, memory_order_relaxed);

    // CHECK-MESSAGES: :[[@LINE+1]]:43: warning: atomic operation shall use memory_order_seq_cst for failure ordering [automotive-c25-req-21.25]
    atomic_compare_exchange_weak_explicit(&global_counter, &expected, desired,
                                          memory_order_seq_cst, memory_order_relaxed);

    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: atomic fence operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_thread_fence(memory_order_acquire);

    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: atomic fence operation shall use memory_order_seq_cst [automotive-c25-req-21.25]
    atomic_signal_fence(memory_order_release);

    (void)value;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    int value;
    int expected = 5;
    int desired = 10;

    // Compliant: Using memory_order_seq_cst explicitly
    value = atomic_load_explicit(&global_counter, memory_order_seq_cst);

    atomic_store_explicit(&global_counter, 42, memory_order_seq_cst);

    value = atomic_exchange_explicit(&global_counter, 100, memory_order_seq_cst);

    value = atomic_fetch_add_explicit(&global_counter, 1, memory_order_seq_cst);

    value = atomic_fetch_sub_explicit(&global_counter, 1, memory_order_seq_cst);

    value = atomic_fetch_or_explicit(&global_counter, 0xFF, memory_order_seq_cst);

    value = atomic_fetch_and_explicit(&global_counter, 0xF0, memory_order_seq_cst);

    value = atomic_fetch_xor_explicit(&global_counter, 0x0F, memory_order_seq_cst);

    // Compliant: Both success and failure orderings are seq_cst
    atomic_compare_exchange_strong_explicit(&global_counter, &expected, desired,
                                            memory_order_seq_cst, memory_order_seq_cst);

    atomic_compare_exchange_weak_explicit(&global_counter, &expected, desired,
                                          memory_order_seq_cst, memory_order_seq_cst);

    // Compliant: Using seq_cst for fences
    atomic_thread_fence(memory_order_seq_cst);

    atomic_signal_fence(memory_order_seq_cst);

    // Compliant: Implicit seq_cst ordering (non-explicit functions)
    value = atomic_load(&global_counter);
    atomic_store(&global_counter, 42);
    value = atomic_exchange(&global_counter, 100);
    value = atomic_fetch_add(&global_counter, 1);
    value = atomic_fetch_sub(&global_counter, 1);
    atomic_compare_exchange_strong(&global_counter, &expected, desired);
    atomic_compare_exchange_weak(&global_counter, &expected, desired);

    (void)value;
}

// Compliant: Non-atomic operations are not affected
void test_non_atomic(void) {
    int regular_counter = 0;
    regular_counter++;
    regular_counter += 5;
    (void)regular_counter;
}
