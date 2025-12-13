// MISRA C:2025 Directive 4.12 - Dynamic memory avoidance
// Category: Required
// Status: VIOLATION
//
// Dynamic memory allocation should not be used in safety-critical code.
// Dynamic memory introduces unpredictability (fragmentation, allocation
// failure, timing variability).

#include <stdlib.h>
#include <string.h>

// VIOLATION: Using malloc for variable-size allocation
int *create_array(size_t size) {
    return malloc(size * sizeof(int));
}

// VIOLATION: Using calloc
int *create_zeroed_array(size_t count) {
    return calloc(count, sizeof(int));
}

// VIOLATION: Using realloc
int *resize_array(int *arr, size_t new_size) {
    return realloc(arr, new_size * sizeof(int));
}

// VIOLATION: Using strdup (implicitly allocates)
char *duplicate_string(const char *str) {
    return strdup(str);
}

// VIOLATION: Dynamic allocation in safety-critical function
typedef struct {
    int *data;
    size_t size;
} DynamicBuffer;

DynamicBuffer *create_buffer(size_t initial_size) {
    DynamicBuffer *buf = malloc(sizeof(DynamicBuffer));
    if (buf != NULL) {
        buf->data = malloc(initial_size * sizeof(int));
        buf->size = initial_size;
    }
    return buf;
}

// Problems with dynamic memory in safety-critical systems:
// 1. Allocation can fail at runtime
// 2. Memory fragmentation over time
// 3. Non-deterministic timing
// 4. Potential memory leaks
// 5. Difficult to verify worst-case memory usage
