// MISRA C:2025 Rule 10.1 - Operand type restrictions
// Category: Required
// Status: VIOLATION
//
// Operands shall not be of an inappropriate essential type.

#include <stdint.h>
#include <stdbool.h>

// VIOLATION: Boolean used in arithmetic
void bool_arithmetic(bool flag) {
    int result = flag + 1;  // Boolean in arithmetic
    (void)result;
}

// VIOLATION: Character used in arithmetic
void char_arithmetic(char c) {
    int value = c * 2;  // Character in multiplication
    (void)value;
}

// VIOLATION: Enum used with different enum type
typedef enum { RED, GREEN, BLUE } Color;
typedef enum { SMALL, MEDIUM, LARGE } Size;

void enum_mix(Color c, Size s) {
    if (c == s) {  // Comparing different enum types
        // ...
    }
}

// VIOLATION: Signed/unsigned comparison
void sign_compare(int32_t signed_val, uint32_t unsigned_val) {
    if (signed_val < unsigned_val) {  // Mixed sign comparison
        // Implicit conversion may cause unexpected results
    }
}

// VIOLATION: Float used for bitwise operation
void float_bitwise(void) {
    float f = 3.14f;
    int bits = *(int*)&f;  // Type punning
    bits = bits & 0xFF;    // Bitwise on float representation
    (void)bits;
}
