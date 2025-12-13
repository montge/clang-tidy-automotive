// MISRA C:2025 Rule 9.1 - Automatic variables initialization
// Category: Mandatory
// Status: COMPLIANT
//
// All automatic variables are initialized before use.

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// COMPLIANT: Variable initialized at declaration
int32_t get_squared(void) {
    int32_t value = 5;
    return value * value;
}

// COMPLIANT: Variable initialized on all paths
int32_t conditional_init(bool condition) {
    int32_t result;
    if (condition) {
        result = 42;
    } else {
        result = 0;  // Initialize on all paths
    }
    return result;
}

// Alternative COMPLIANT: Initialize at declaration
int32_t conditional_init_v2(bool condition) {
    int32_t result = 0;  // Default value
    if (condition) {
        result = 42;
    }
    return result;
}

// COMPLIANT: Array initialized before use
void array_example(void) {
    int32_t data[10] = {0};  // Zero-initialized
    int32_t sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += data[i];
    }
    (void)sum;
}

// COMPLIANT: Pointer initialized before use
void pointer_example(void) {
    int32_t storage = 0;
    int32_t *ptr = &storage;  // Initialized to valid address
    *ptr = 42;
}

// COMPLIANT: Struct initialized
typedef struct {
    int32_t x;
    int32_t y;
} Point;

Point create_point(void) {
    Point p = {0, 0};  // All members initialized
    return p;
}
