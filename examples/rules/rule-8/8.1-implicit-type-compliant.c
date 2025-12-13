// MISRA C:2025 Rule 8.1 - Explicit types
// Category: Required
// Status: COMPLIANT
//
// All types are explicitly stated.

#include <stdint.h>

// COMPLIANT: Explicit return type
int32_t calculate(int32_t x) {
    return x * 2;
}

// COMPLIANT: Explicit parameter type
void process(int32_t value) {
    (void)value;
}

// COMPLIANT: Explicit type for const
static const int32_t MAX_VALUE = 100;

// COMPLIANT: Explicit type for static
static int32_t counter = 0;

// COMPLIANT: Explicit type for extern
extern int32_t global_flag;

// Best practice: Use fixed-width types from <stdint.h>
// for predictable behavior across platforms:
// int8_t, int16_t, int32_t, int64_t
// uint8_t, uint16_t, uint32_t, uint64_t
