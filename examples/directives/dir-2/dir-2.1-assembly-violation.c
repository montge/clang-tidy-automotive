// MISRA C:2025 Directive 2.1 - Assembly language isolation
// Category: Required
// Status: VIOLATION
//
// Assembly language code should be isolated and encapsulated.
// This code mixes inline assembly with regular C code without
// proper isolation, making it difficult to maintain and port.

#include <stdint.h>

// VIOLATION: Inline assembly scattered throughout application code
// No isolation, no documentation, no portability layer
uint32_t read_control_register(void) {
    uint32_t value;
    // Inline assembly mixed with regular code
    __asm__ volatile("mrc p15, 0, %0, c1, c0, 0" : "=r"(value));
    return value;
}

// VIOLATION: Assembly embedded in business logic
void process_sensor_data(uint32_t *data, int count) {
    for (int i = 0; i < count; i++) {
        // Business logic mixed with platform-specific assembly
        __asm__ volatile("dmb" ::: "memory");  // Memory barrier
        data[i] = data[i] * 2;
        __asm__ volatile("dsb" ::: "memory");  // Data sync barrier
    }
}

// VIOLATION: No abstraction for platform-specific operations
void enable_interrupts(void) {
    __asm__ volatile("cpsie i");
}

void disable_interrupts(void) {
    __asm__ volatile("cpsid i");
}

// Application code directly using assembly
void critical_operation(void) {
    disable_interrupts();
    // Critical section
    enable_interrupts();
}
