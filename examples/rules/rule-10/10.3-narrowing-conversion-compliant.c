// MISRA C:2025 Rule 10.3 - Value assignment narrowing
// Category: Required
// Status: COMPLIANT
//
// Values are not assigned to narrower types without explicit handling.

#include <stdint.h>
#include <stdbool.h>

// COMPLIANT: Use same-width types or explicitly cast with validation
bool safe_narrow_to_int16(int32_t wide, int16_t *result) {
    if ((wide < INT16_MIN) || (wide > INT16_MAX)) {
        return false;  // Would overflow
    }
    *result = (int16_t)wide;  // Safe explicit cast
    return true;
}

// COMPLIANT: Extract byte with explicit masking
uint8_t get_low_byte(uint32_t value) {
    return (uint8_t)(value & 0xFFU);  // Explicit extraction
}

// COMPLIANT: Float to integer with explicit rounding
int32_t round_to_int(float f) {
    // Explicit rounding behavior
    return (int32_t)(f + 0.5f);
}

// COMPLIANT: Keep same precision
void same_precision(void) {
    float f1 = 3.14f;
    float f2 = f1 * 2.0f;  // Same type
    (void)f2;

    double d1 = 1e100;
    double d2 = d1 / 2.0;  // Same type
    (void)d2;
}

// COMPLIANT: Explicit conversion with range check
bool uint_to_int32(uint32_t u, int32_t *result) {
    if (u > (uint32_t)INT32_MAX) {
        return false;
    }
    *result = (int32_t)u;
    return true;
}

// COMPLIANT: Function returns appropriate type
int32_t get_value(void) {
    int32_t result = calculate_something();
    return result;  // Same type, no narrowing
}

int32_t calculate_something(void) { return 50000; }
