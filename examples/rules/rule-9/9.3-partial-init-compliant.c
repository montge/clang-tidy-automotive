// MISRA C:2025 Rule 9.3 - Partial array initialization
// Category: Required
// Status: COMPLIANT
//
// Arrays are either fully initialized or zero-initialized.

#include <stdint.h>

// COMPLIANT: Explicit zero-initialization (all elements)
int32_t zero_array[10] = {0};

// COMPLIANT: Full initialization
int32_t full_array[5] = {1, 2, 3, 4, 5};

// COMPLIANT: Designated initializers with explicit zeros
int32_t data[5] = {
    [0] = 1,
    [1] = 0,  // Explicit zero
    [2] = 3,
    [3] = 0,  // Explicit zero
    [4] = 0   // Explicit zero
};

// COMPLIANT: Struct array fully initialized
typedef struct {
    int32_t x;
    int32_t y;
} Point;

Point points[3] = {
    {1, 2},
    {3, 4},
    {0, 0}  // Explicit initialization of third element
};

// COMPLIANT: Multi-dimensional array fully initialized
int32_t matrix[3][3] = {
    {1, 2, 3},
    {4, 5, 0},
    {0, 0, 0}
};

// COMPLIANT: Use runtime initialization if needed
void init_array(int32_t arr[], size_t size, int32_t value) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = value;
    }
}
