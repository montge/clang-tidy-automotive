// MISRA C:2025 Directive 2.1 - Assembly language isolation
// Category: Required
// Status: COMPLIANT
//
// Assembly language is properly isolated in dedicated modules
// with well-documented interfaces. This file shows the application
// code using abstract interfaces instead of inline assembly.

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// Hardware Abstraction Layer (HAL) - Public Interface
// All assembly is isolated in platform-specific implementation files
// ============================================================================

// Forward declarations - implementations in platform-specific .c files
// See: hal_arm_cortex_m4.c, hal_arm_cortex_a53.c, hal_x86.c

typedef uint32_t irq_state_t;

// Interrupt control abstraction
irq_state_t hal_irq_disable(void);
void hal_irq_restore(irq_state_t state);

// Memory barrier abstraction
void hal_memory_barrier(void);
void hal_data_sync_barrier(void);

// System register abstraction
uint32_t hal_read_system_control(void);

// ============================================================================
// Application Code - Pure C, no assembly
// ============================================================================

// COMPLIANT: Critical section using abstract interface
void critical_operation(void) {
    irq_state_t saved_state = hal_irq_disable();

    // Critical section - pure C code
    // ... perform critical operations ...

    hal_irq_restore(saved_state);
}

// COMPLIANT: Sensor processing using abstract memory barriers
void process_sensor_data(uint32_t *data, int count) {
    for (int i = 0; i < count; i++) {
        hal_memory_barrier();      // Abstract barrier
        data[i] = data[i] * 2;     // Pure C processing
        hal_data_sync_barrier();   // Abstract barrier
    }
}

// COMPLIANT: System configuration using abstract interface
bool check_system_config(void) {
    uint32_t ctrl = hal_read_system_control();
    return (ctrl & 0x01U) != 0U;  // Check enable bit
}

// ============================================================================
// Note: The HAL implementation file (hal_arm_cortex_m4.c) would contain:
//
// irq_state_t hal_irq_disable(void) {
//     irq_state_t state;
//     __asm__ volatile("mrs %0, primask" : "=r"(state));
//     __asm__ volatile("cpsid i");
//     return state;
// }
//
// All assembly is in ONE dedicated file, not scattered through the codebase.
// ============================================================================
