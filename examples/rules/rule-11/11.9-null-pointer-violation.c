// MISRA C:2025 Rule 11.9 - NULL pointer constant
// Category: Required
// Status: VIOLATION
//
// The macro NULL shall be the only permitted form of null pointer constant.
//
// Expected diagnostic:
//   warning: use NULL for null pointer constant [automotive-wrong-null-pointer-value]

#include <stdint.h>

// VIOLATION: Using 0 as null pointer
void zero_as_null(void) {
    int *ptr = 0;  // Should use NULL
    (void)ptr;
}

// VIOLATION: Using (void*)0 instead of NULL
void explicit_cast_zero(void) {
    int *ptr = (void*)0;  // Should use NULL
    (void)ptr;
}

// VIOLATION: Comparing pointer to 0
int is_null_check(int *ptr) {
    return ptr == 0;  // Should compare to NULL
}

// VIOLATION: Using 0 in conditional
void conditional_zero(int *ptr) {
    if (ptr != 0) {  // Should use NULL
        // use ptr
    }
}

// VIOLATION: Returning 0 as pointer
int *get_nothing(void) {
    return 0;  // Should return NULL
}

// VIOLATION: Using integer literal
void assign_null(void) {
    void *p = 0x0;  // Should use NULL
    (void)p;
}
