// MISRA C:2025 Directive 4.12 - Dynamic memory avoidance
// Category: Required
// Status: COMPLIANT
//
// All memory is statically allocated at compile time.
// This ensures deterministic behavior in safety-critical systems.

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// COMPLIANT: Fixed-size array with compile-time size
#define MAX_ARRAY_SIZE 100

typedef struct {
    int data[MAX_ARRAY_SIZE];
    size_t used;
} StaticArray;

void init_array(StaticArray *arr) {
    arr->used = 0U;
}

bool add_element(StaticArray *arr, int value) {
    if (arr->used >= MAX_ARRAY_SIZE) {
        return false;  // Array full
    }
    arr->data[arr->used] = value;
    arr->used++;
    return true;
}

// COMPLIANT: Static buffer pool instead of dynamic allocation
#define MAX_BUFFERS 10
#define BUFFER_SIZE 256

typedef struct {
    uint8_t data[BUFFER_SIZE];
    bool in_use;
} Buffer;

static Buffer buffer_pool[MAX_BUFFERS];

Buffer *allocate_buffer(void) {
    for (size_t i = 0U; i < MAX_BUFFERS; i++) {
        if (!buffer_pool[i].in_use) {
            buffer_pool[i].in_use = true;
            return &buffer_pool[i];
        }
    }
    return NULL;  // No buffers available
}

void release_buffer(Buffer *buf) {
    if (buf != NULL) {
        buf->in_use = false;
    }
}

// COMPLIANT: Fixed-size string buffer
#define MAX_STRING_LENGTH 64

typedef struct {
    char data[MAX_STRING_LENGTH];
    size_t length;
} FixedString;

bool copy_string(FixedString *dest, const char *src) {
    if ((dest == NULL) || (src == NULL)) {
        return false;
    }

    size_t len = strlen(src);
    if (len >= MAX_STRING_LENGTH) {
        return false;  // String too long
    }

    (void)memcpy(dest->data, src, len + 1U);
    dest->length = len;
    return true;
}

// Benefits of static allocation:
// 1. Memory usage known at compile time
// 2. No allocation failures at runtime
// 3. No fragmentation
// 4. Deterministic timing
// 5. Easier to verify and certify
