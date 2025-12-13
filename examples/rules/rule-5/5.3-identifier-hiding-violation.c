// MISRA C:2025 Rule 5.3 - Identifier hiding
// Category: Required
// Status: VIOLATION
//
// An identifier in an inner scope shall not hide an identifier
// in an outer scope.
//
// Expected diagnostic:
//   warning: declaration shadows a variable [-Wshadow]

#include <stdint.h>

int32_t counter = 0;  // File scope

// VIOLATION: Parameter hides file-scope variable
void increment(int32_t counter) {  // Shadows outer 'counter'
    counter++;  // Modifies parameter, not file-scope variable
}

// VIOLATION: Local variable hides file-scope variable
void process(void) {
    int32_t counter = 10;  // Shadows outer 'counter'
    counter++;
}

// VIOLATION: Nested scope hiding
void nested_example(void) {
    int32_t value = 1;
    {
        int32_t value = 2;  // Shadows outer 'value'
        (void)value;
    }
    (void)value;
}

// VIOLATION: Loop variable hiding
void loop_example(void) {
    int32_t i = 100;
    for (int32_t i = 0; i < 10; i++) {  // Shadows outer 'i'
        (void)i;
    }
    (void)i;
}
