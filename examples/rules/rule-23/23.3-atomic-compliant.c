// MISRA C:2025 Rule 23.3 - Atomic object access
// Category: Required
// Status: COMPLIANT
//
// Atomic objects are accessed using proper atomic operations.

#include <stdatomic.h>
#include <stdint.h>

static _Atomic int32_t atomic_counter = ATOMIC_VAR_INIT(0);

// COMPLIANT: Using atomic_store for writes
void atomic_write(int32_t value) {
    atomic_store(&atomic_counter, value);
}

// COMPLIANT: Using atomic_store with memory order
void atomic_write_relaxed(int32_t value) {
    atomic_store_explicit(&atomic_counter, value, memory_order_relaxed);
}

// COMPLIANT: Using atomic_load for reads
int32_t atomic_read(void) {
    return atomic_load(&atomic_counter);
}

// COMPLIANT: Using atomic_load with memory order
int32_t atomic_read_acquire(void) {
    return atomic_load_explicit(&atomic_counter, memory_order_acquire);
}

// COMPLIANT: Using atomic_fetch_add for increment
void atomic_increment(void) {
    (void)atomic_fetch_add(&atomic_counter, 1);
}

// COMPLIANT: Using atomic_fetch_add for compound operations
int32_t atomic_add(int32_t delta) {
    return atomic_fetch_add(&atomic_counter, delta);
}

// COMPLIANT: Using atomic_compare_exchange for CAS operations
_Bool atomic_increment_if_less(int32_t limit) {
    int32_t expected = atomic_load(&atomic_counter);
    while (expected < limit) {
        if (atomic_compare_exchange_weak(&atomic_counter, &expected, expected + 1)) {
            return 1;  // Success
        }
        // expected is updated on failure, loop continues
    }
    return 0;  // Counter already at or above limit
}

// COMPLIANT: Atomic pointer with proper operations
static _Atomic(int32_t *) atomic_ptr = NULL;

void atomic_ptr_write(int32_t *new_ptr) {
    atomic_store(&atomic_ptr, new_ptr);
}

int32_t *atomic_ptr_read(void) {
    return atomic_load(&atomic_ptr);
}
