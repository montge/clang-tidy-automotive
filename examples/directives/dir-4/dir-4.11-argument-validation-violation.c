// MISRA C:2025 Directive 4.11 - Validity of function arguments
// Category: Required
// Status: VIOLATION
//
// Function arguments should be validated before use.
// This code assumes all arguments are valid without checking.

#include <stdint.h>
#include <string.h>

// VIOLATION: No validation of pointer argument
void copy_data(uint8_t *dest, const uint8_t *src, size_t len) {
    // dest or src could be NULL - undefined behavior
    memcpy(dest, src, len);
}

// VIOLATION: No validation of array index
int get_element(const int *array, int index) {
    // index could be negative or out of bounds
    return array[index];
}

// VIOLATION: No validation of divisor
int safe_divide(int numerator, int denominator) {
    // denominator could be zero - undefined behavior
    return numerator / denominator;
}

// VIOLATION: No validation of size parameter
void process_buffer(const char *buffer, size_t size) {
    // size could be larger than actual buffer
    // No way to validate without additional context
    for (size_t i = 0; i < size; i++) {
        // Process buffer[i]
        (void)buffer[i];
    }
}

// VIOLATION: No validation of enum value
typedef enum { STATE_IDLE, STATE_RUNNING, STATE_ERROR } State;

const char *state_to_string(State state) {
    // state could be an invalid enum value
    static const char *names[] = {"IDLE", "RUNNING", "ERROR"};
    return names[state];  // Out of bounds if state > 2
}
