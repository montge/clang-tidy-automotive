// MISRA C:2025 Rule 12.6 - Violation Examples
// Structure and union members of atomic objects shall not be directly accessed
//
// Check: automotive-c25-req-12.6

#include <stdatomic.h>

struct Point {
    int x;
    int y;
};

_Atomic struct Point atomic_point;

void non_compliant_atomic_access(void) {
    // Non-compliant: Direct member access breaks atomicity
    int x = atomic_point.x;  // WARNING: Direct member access on atomic struct

    // Non-compliant: Direct member assignment breaks atomicity
    atomic_point.y = 20;  // WARNING: Direct member access on atomic struct
}

// Non-compliant: Pointer to atomic struct member access
_Atomic struct Point *atomic_ptr;

void non_compliant_pointer_access(void) {
    // Non-compliant: Using -> operator on atomic struct pointer
    int x = atomic_ptr->x;  // WARNING: Direct member access on atomic struct

    atomic_ptr->y = 30;  // WARNING: Direct member access on atomic struct
}

// Non-compliant: Atomic union member access
union Data {
    int i;
    float f;
};

_Atomic union Data atomic_data;

void non_compliant_union_access(void) {
    // Non-compliant: Direct union member access breaks atomicity
    int i = atomic_data.i;  // WARNING: Direct member access on atomic union

    atomic_data.f = 3.14f;  // WARNING: Direct member access on atomic union
}

// Rationale:
// Accessing individual members of an atomic composite type (struct/union)
// circumvents the atomicity guarantees. The entire object must be accessed
// atomically using atomic_load/atomic_store to ensure thread-safe operations.
//
// This is undefined behavior per C11 standard 7.17.6:
// "Atomic operations on objects of atomic type shall be lock-free if the
// object's size is lock-free; otherwise, such operations may not be lock-free."
//
// Direct member access may result in:
// - Race conditions in multi-threaded environments
// - Torn reads/writes where only part of the struct is updated
// - Memory ordering issues
// - Undefined behavior
