// MISRA C:2025 Rule 8.2 - Function prototypes
// Category: Required
// Status: COMPLIANT
//
// All function types are in prototype form with named parameters.

#include <stdint.h>

// COMPLIANT: (void) for no parameters
int32_t get_value(void);

// COMPLIANT: All parameters named
void process(int32_t first_value, int32_t second_value);

// COMPLIANT: Modern function definition
int32_t add_values(int32_t a, int32_t b) {
    return a + b;
}

// COMPLIANT: Function pointer with prototype
typedef int32_t (*callback_fn)(int32_t value);

// COMPLIANT: All parameters named
void all_params_named(int32_t x, float ratio, char code);

// COMPLIANT: Definition matches declaration
int32_t get_value(void) {
    return 42;
}

void process(int32_t first_value, int32_t second_value) {
    (void)first_value;
    (void)second_value;
}

void all_params_named(int32_t x, float ratio, char code) {
    (void)x;
    (void)ratio;
    (void)code;
}
