// MISRA C:2025 Rule 9.1 - Automatic variables initialization
// Category: Mandatory
// Status: VIOLATION
//
// Automatic variables shall be initialized before use.
//
// Expected diagnostic:
//   warning: variable may be uninitialized [-Wuninitialized]

#include <stdint.h>
#include <stdbool.h>

// VIOLATION: Using uninitialized variable
int32_t get_squared(void) {
    int32_t value;  // Not initialized
    return value * value;  // Undefined behavior
}

// VIOLATION: Conditional initialization may leave variable uninitialized
int32_t conditional_init(bool condition) {
    int32_t result;  // Not initialized
    if (condition) {
        result = 42;
    }
    // If condition is false, result is uninitialized
    return result;
}

// VIOLATION: Array not fully initialized before use
void array_example(void) {
    int32_t data[10];  // Uninitialized array
    int32_t sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += data[i];  // Reading uninitialized values
    }
    (void)sum;
}

// VIOLATION: Pointer used before initialization
void pointer_example(void) {
    int32_t *ptr;  // Uninitialized pointer
    *ptr = 42;     // Undefined behavior
}
