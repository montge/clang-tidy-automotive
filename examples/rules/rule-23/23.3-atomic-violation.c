// MISRA C:2025 Rule 23.3 - Atomic object access
// Category: Required
// Status: VIOLATION
//
// Atomic objects shall be accessed using atomic operations.

#include <stdatomic.h>
#include <stdint.h>

static _Atomic int32_t atomic_counter = 0;

// VIOLATION: Direct assignment to atomic object
void direct_write(int32_t value) {
    atomic_counter = value;  // Should use atomic_store
}

// VIOLATION: Direct read from atomic object
int32_t direct_read(void) {
    return atomic_counter;   // Should use atomic_load
}

// VIOLATION: Non-atomic increment
void direct_increment(void) {
    atomic_counter++;        // Should use atomic_fetch_add
}

// VIOLATION: Non-atomic compound assignment
void direct_compound(int32_t delta) {
    atomic_counter += delta; // Should use atomic_fetch_add
}

// VIOLATION: Mixing atomic and non-atomic access
int32_t mixed_access(void) {
    int32_t local = atomic_counter;  // Non-atomic read
    atomic_store(&atomic_counter, local + 1);  // Atomic write
    // Race condition between read and write
    return local;
}

// VIOLATION: Atomic pointer accessed non-atomically
static _Atomic(int32_t *) atomic_ptr = NULL;

void ptr_violation(int32_t *new_ptr) {
    atomic_ptr = new_ptr;    // Should use atomic_store
}
