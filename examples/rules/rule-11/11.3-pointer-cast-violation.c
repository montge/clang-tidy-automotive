// MISRA C:2025 Rule 11.3 - Pointer type casts
// Category: Required
// Status: VIOLATION
//
// A cast shall not be performed between a pointer to object type
// and a pointer to a different object type.

#include <stdint.h>

// VIOLATION: Cast between different pointer types
void type_punning(void) {
    uint32_t value = 0x12345678U;
    uint8_t *bytes = (uint8_t *)&value;  // Pointer type cast
    (void)bytes[0];  // Aliasing violation possible
}

// VIOLATION: Cast from char* to int*
void alignment_issue(char *buffer) {
    int32_t *int_ptr = (int32_t *)buffer;  // May cause alignment fault
    (void)*int_ptr;
}

// VIOLATION: Cast between struct pointers
typedef struct { int a; } StructA;
typedef struct { int b; } StructB;

void struct_cast(StructA *a) {
    StructB *b = (StructB *)a;  // Different struct types
    (void)b;
}

// VIOLATION: Cast function pointer to data pointer
typedef void (*func_ptr)(void);

void func_data_cast(func_ptr fn) {
    void *data = (void *)fn;  // Function to data pointer
    (void)data;
}

// VIOLATION: Cast through void* to bypass type system
void type_bypass(int32_t *int_ptr) {
    void *v = int_ptr;
    float *float_ptr = (float *)v;  // Type aliasing
    (void)*float_ptr;
}
