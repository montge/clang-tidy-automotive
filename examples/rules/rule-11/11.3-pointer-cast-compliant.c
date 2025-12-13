// MISRA C:2025 Rule 11.3 - Pointer type casts
// Category: Required
// Status: COMPLIANT
//
// Pointer casts are avoided or done through character types.

#include <stdint.h>
#include <string.h>

// COMPLIANT: Use memcpy instead of type punning
uint8_t get_byte(uint32_t value, int index) {
    uint8_t bytes[4];
    (void)memcpy(bytes, &value, sizeof(value));
    return bytes[index];
}

// COMPLIANT: Use union for type representation (explicit)
typedef union {
    uint32_t as_uint32;
    uint8_t as_bytes[4];
} Uint32Bytes;

uint8_t get_byte_union(uint32_t value, int index) {
    Uint32Bytes u;
    u.as_uint32 = value;
    return u.as_bytes[index];
}

// COMPLIANT: Access through char* is allowed (special case)
void copy_bytes(void *dest, const void *src, size_t n) {
    // char* can alias any type (special rule)
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
}

// COMPLIANT: Same pointer type
void same_type(int32_t *a, int32_t *b) {
    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

// COMPLIANT: Use const correctly
void const_correct(const int32_t *input, int32_t *output) {
    *output = *input * 2;
}

// COMPLIANT: Pointer to compatible type (const addition)
void add_const(int32_t *mutable_ptr) {
    const int32_t *const_ptr = mutable_ptr;  // Adding const is allowed
    (void)*const_ptr;
}
