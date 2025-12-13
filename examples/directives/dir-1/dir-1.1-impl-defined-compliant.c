// MISRA C:2025 Directive 1.1 - Implementation-defined behavior
// Category: Required
// Status: COMPLIANT
//
// This code properly documents all implementation-defined behavior
// and uses portable constructs where possible.
//
// Target Platform: ARM Cortex-M4 (32-bit, little-endian)
// Compiler: GCC 12.x with -fno-strict-aliasing

#include <stdint.h>
#include <limits.h>

// COMPLIANT: Uses unsigned type to avoid implementation-defined shift behavior
uint32_t logical_shift(uint32_t value, unsigned int shift) {
    // Unsigned right-shift is well-defined (zero-fill)
    return value >> shift;
}

// COMPLIANT: Uses portable method to determine bit width
// Documentation: CHAR_BIT is guaranteed to be at least 8 by the standard
int calculate_bits_portable(void) {
    return sizeof(int) * CHAR_BIT;
}

// COMPLIANT: Uses fixed-width types with explicit documentation
// Platform Assumption: Compiler uses standard LP32/ILP32 data model
// Struct Packing: Using compiler pragma to ensure predictable layout
#pragma pack(push, 1)
struct SensorDataPacked {
    uint8_t  status;
    uint32_t value;
};
#pragma pack(pop)

// Documentation of implementation-defined behavior:
// - sizeof(struct SensorDataPacked) = 5 bytes (packed)
// - No padding between members when packed
int get_packed_struct_size(void) {
    return sizeof(struct SensorDataPacked);
}

// COMPLIANT: Document assumed behavior in project documentation
// Implementation-Defined Behavior Register:
// 1. Right-shift of signed integers: Not used (use unsigned)
// 2. Struct padding: Controlled via #pragma pack
// 3. Integer sizes: Use <stdint.h> fixed-width types
// 4. Byte order: Little-endian (ARM Cortex-M4)
