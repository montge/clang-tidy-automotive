// MISRA C:2025 Rule 12.6 - Compliant Examples
// Structure and union members of atomic objects shall not be directly accessed
//
// Check: automotive-c25-req-12.6

#include <stdatomic.h>

// Compliant: Using atomic operations on the entire object
struct Point {
    int x;
    int y;
};

_Atomic struct Point atomic_point;

void compliant_atomic_access(void) {
    // Compliant: Load the entire atomic struct
    struct Point temp = atomic_load(&atomic_point);
    int x_value = temp.x;  // Access member from non-atomic copy

    // Compliant: Modify non-atomic copy and store entire struct atomically
    temp.y = 20;
    atomic_store(&atomic_point, temp);
}

// Compliant: Non-atomic struct access
struct Point regular_point;

void compliant_regular_access(void) {
    // Compliant: Direct access to non-atomic struct
    regular_point.x = 10;
    int y_value = regular_point.y;
}

// Compliant: Atomic operations on simple types (not struct/union)
_Atomic int atomic_int;

void compliant_atomic_simple(void) {
    // Compliant: Direct operations on atomic simple types are allowed
    atomic_int = 42;
    int value = atomic_int;

    // Or using explicit atomic operations
    atomic_store(&atomic_int, 100);
    value = atomic_load(&atomic_int);
}

// Compliant: Using atomic_compare_exchange for struct updates
void compliant_atomic_compare_exchange(void) {
    struct Point expected = {0, 0};
    struct Point desired = {10, 20};

    // Compliant: Atomic compare-exchange on entire struct
    atomic_compare_exchange_strong(&atomic_point, &expected, desired);
}
