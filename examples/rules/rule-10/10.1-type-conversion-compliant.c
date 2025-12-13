// MISRA C:2025 Rule 10.1 - Operand type restrictions
// Category: Required
// Status: COMPLIANT
//
// Operands are of appropriate essential types.

#include <stdint.h>
#include <stdbool.h>

// COMPLIANT: Convert boolean to integer explicitly
void bool_to_int(bool flag) {
    int32_t result = flag ? 1 : 0;
    result = result + 1;
    (void)result;
}

// COMPLIANT: Use integer for arithmetic
void int_arithmetic(int32_t value) {
    int32_t result = value * 2;
    (void)result;
}

// COMPLIANT: Compare same enum types
typedef enum { RED = 0, GREEN = 1, BLUE = 2 } Color;

void enum_compare(Color c1, Color c2) {
    if (c1 == c2) {
        // Same enum type comparison
    }
}

// COMPLIANT: Explicit cast for mixed sign operations
void sign_compare(int32_t signed_val, uint32_t unsigned_val) {
    // Ensure signed value is non-negative before comparison
    if ((signed_val >= 0) && ((uint32_t)signed_val < unsigned_val)) {
        // Safe comparison
    }
}

// COMPLIANT: Use union for type punning (implementation-defined but explicit)
typedef union {
    float f;
    uint32_t u;
} FloatBits;

uint32_t get_float_bits(float f) {
    FloatBits fb;
    fb.f = f;
    return fb.u;
}

// COMPLIANT: Bitwise operations on unsigned integer
void bitwise_example(uint32_t value) {
    uint32_t masked = value & 0xFFU;
    (void)masked;
}
