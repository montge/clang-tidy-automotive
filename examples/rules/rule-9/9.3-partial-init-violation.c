// MISRA C:2025 Rule 9.3 - Partial array initialization
// Category: Required
// Status: VIOLATION
//
// Arrays shall not be partially initialized.
//
// Expected diagnostic:
//   warning: partial array initialization [automotive-avoid-partial-array-init]

#include <stdint.h>

// VIOLATION: Partial initialization of array
int32_t partial_array[10] = {1, 2, 3};  // Only 3 of 10 elements explicit

// VIOLATION: Partial initialization with gaps implied
int32_t data[5] = {[0] = 1, [2] = 3};  // [1], [3], [4] implicitly zero

// VIOLATION: Struct array partially initialized
typedef struct {
    int32_t x;
    int32_t y;
} Point;

Point points[3] = {
    {1, 2},
    {3, 4}
    // Third element implicitly zero-initialized
};

// VIOLATION: Multi-dimensional array partial init
int32_t matrix[3][3] = {
    {1, 2, 3},
    {4, 5}      // Third column implicitly zero
    // Third row implicitly zero
};

// The concern: partial initialization can hide unintended
// missing initializers, making bugs harder to spot.
