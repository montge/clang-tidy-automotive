// MISRA C:2025 Rule 20.4 - Macro named as keyword
// Category: Required
// Status: COMPLIANT
//
// No macros are defined with C keyword names.

#include <stdint.h>
#include <stdbool.h>

// COMPLIANT: Macros with unique, descriptive names
#define MAX_BUFFER_SIZE 256
#define CONFIG_ENABLE_LOGGING 1
#define SENSOR_TIMEOUT_MS 1000

// COMPLIANT: Type-defining macros with unique names
#define UINT8 uint8_t
#define SINT32 int32_t

// COMPLIANT: Function-like macros with clear names
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ABS(x) (((x) < 0) ? -(x) : (x))

// COMPLIANT: Boolean constants use standard stdbool.h
// true, false, bool are provided by <stdbool.h>

// COMPLIANT: Conditional compilation macros
#define DEBUG_MODE 0
#define PLATFORM_ARM 1

// Best practice: Use uppercase for macros to distinguish
// from C keywords and regular identifiers
void example(void) {
    int32_t value = MIN(10, 20);
    bool enabled = DEBUG_MODE != 0;
    (void)value;
    (void)enabled;
}
