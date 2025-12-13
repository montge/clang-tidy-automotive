// MISRA C:2025 Rule 11.9 - NULL pointer constant
// Category: Required
// Status: COMPLIANT
//
// NULL is used consistently for null pointer constants.

#include <stddef.h>  // For NULL
#include <stdbool.h>

// COMPLIANT: Using NULL for initialization
void null_init(void) {
    int *ptr = NULL;
    (void)ptr;
}

// COMPLIANT: Comparing pointer to NULL
bool is_null(const int *ptr) {
    return ptr == NULL;
}

// COMPLIANT: Using NULL in conditional
void check_pointer(int *ptr) {
    if (ptr != NULL) {
        // Safe to use ptr
        *ptr = 42;
    }
}

// COMPLIANT: Returning NULL
int *find_element(const int *array, size_t size, int value) {
    if (array == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        if (array[i] == value) {
            return (int *)&array[i];  // Found
        }
    }
    return NULL;  // Not found
}

// COMPLIANT: Function pointer with NULL
typedef void (*callback_fn)(void);

void register_callback(callback_fn cb) {
    if (cb != NULL) {
        cb();
    }
}

// Note: In C23, nullptr can also be used as a null pointer constant.
// For C11/C17, NULL is the standard macro.
