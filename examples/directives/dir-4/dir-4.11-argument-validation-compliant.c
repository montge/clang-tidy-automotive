// MISRA C:2025 Directive 4.11 - Validity of function arguments
// Category: Required
// Status: COMPLIANT
//
// All function arguments are validated before use.

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

// COMPLIANT: Validate all pointer arguments
bool copy_data(uint8_t *dest, const uint8_t *src, size_t len) {
    if ((dest == NULL) || (src == NULL)) {
        return false;
    }
    if (len == 0U) {
        return true;  // Nothing to copy, but not an error
    }
    (void)memcpy(dest, src, len);
    return true;
}

// COMPLIANT: Validate array bounds
bool get_element(const int *array, size_t array_size, size_t index, int *result) {
    if ((array == NULL) || (result == NULL)) {
        return false;
    }
    if (index >= array_size) {
        return false;  // Index out of bounds
    }
    *result = array[index];
    return true;
}

// COMPLIANT: Validate divisor is non-zero
bool safe_divide(int numerator, int denominator, int *result) {
    if (result == NULL) {
        return false;
    }
    if (denominator == 0) {
        return false;  // Division by zero
    }
    *result = numerator / denominator;
    return true;
}

// COMPLIANT: Validate buffer and size together
bool process_buffer(const char *buffer, size_t size) {
    if (buffer == NULL) {
        return false;
    }
    if (size == 0U) {
        return true;  // Empty buffer, nothing to process
    }

    for (size_t i = 0U; i < size; i++) {
        // Process buffer[i] safely
        (void)buffer[i];
    }
    return true;
}

// COMPLIANT: Validate enum value is in range
typedef enum { STATE_IDLE = 0, STATE_RUNNING = 1, STATE_ERROR = 2, STATE_COUNT = 3 } State;

const char *state_to_string(State state) {
    static const char *names[] = {"IDLE", "RUNNING", "ERROR"};
    static const char *unknown = "UNKNOWN";

    if ((int)state < 0 || state >= STATE_COUNT) {
        return unknown;  // Invalid enum value
    }
    return names[state];
}
