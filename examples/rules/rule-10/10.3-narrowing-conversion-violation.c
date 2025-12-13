// MISRA C:2025 Rule 10.3 - Value assignment narrowing
// Category: Required
// Status: VIOLATION
//
// The value of an expression shall not be assigned to an object
// with a narrower essential type.
//
// Expected diagnostic:
//   warning: implicit conversion loses precision [-Wimplicit-conversion]

#include <stdint.h>

// VIOLATION: Assigning wider type to narrower type
void narrowing_examples(void) {
    int32_t wide = 100000;
    int16_t narrow = wide;   // May lose data
    int8_t tiny = narrow;    // May lose data
    (void)tiny;

    uint32_t big = 0x12345678U;
    uint8_t byte = big;      // Loses upper 24 bits
    (void)byte;
}

// VIOLATION: Float to integer conversion
void float_to_int(void) {
    float f = 3.7f;
    int32_t i = f;  // Loses fractional part
    (void)i;
}

// VIOLATION: Double to float
void double_to_float(void) {
    double d = 1e100;
    float f = d;    // May overflow or lose precision
    (void)f;
}

// VIOLATION: Unsigned to signed of same size
void unsigned_to_signed(void) {
    uint32_t u = 0xFFFFFFFFU;
    int32_t s = u;  // Becomes -1 (implementation-defined)
    (void)s;
}

// VIOLATION: Function return narrowing
int16_t get_value(void) {
    int32_t result = calculate_something();
    return result;  // Implicit narrowing
}

int32_t calculate_something(void) { return 50000; }
