// MISRA C:2025 Directive 1.1 - Implementation-defined behavior
// Category: Required
// Status: VIOLATION
//
// This code relies on implementation-defined behavior without documentation.
// The right-shift of a negative value is implementation-defined.
//
// Note: This directive primarily requires manual review and documentation.
// Static analysis cannot verify that all implementation-defined behavior
// is properly documented.

#include <stdint.h>

// VIOLATION: Relies on implementation-defined sign extension behavior
// without documenting the expected behavior for the target platform.
int32_t arithmetic_shift(int32_t value, int shift) {
    // Right-shift of negative integers is implementation-defined
    return value >> shift;
}

// VIOLATION: Assumes specific size for int without documentation
int calculate_bits(void) {
    // sizeof(int) is implementation-defined
    return sizeof(int) * 8;
}

// VIOLATION: Assumes specific struct padding
struct SensorData {
    uint8_t  status;
    uint32_t value;    // Padding before this field is implementation-defined
};

int get_struct_size(void) {
    return sizeof(struct SensorData);  // Size depends on padding
}
