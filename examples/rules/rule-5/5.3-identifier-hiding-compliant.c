// MISRA C:2025 Rule 5.3 - Identifier hiding
// Category: Required
// Status: COMPLIANT
//
// Identifiers in inner scopes do not hide identifiers in outer scopes.

#include <stdint.h>

static int32_t g_counter = 0;  // Use prefix for file-scope

// COMPLIANT: Unique parameter name
void increment(int32_t delta) {
    g_counter += delta;
}

// COMPLIANT: Unique local variable name
void process(void) {
    int32_t local_counter = 10;
    local_counter++;
    (void)local_counter;
}

// COMPLIANT: Unique names in nested scopes
void nested_example(void) {
    int32_t outer_value = 1;
    {
        int32_t inner_value = 2;
        (void)inner_value;
    }
    (void)outer_value;
}

// COMPLIANT: Unique loop variable names
void loop_example(void) {
    int32_t saved_index = 100;
    for (int32_t loop_idx = 0; loop_idx < 10; loop_idx++) {
        (void)loop_idx;
    }
    (void)saved_index;
}

// Best practices:
// - Use prefixes: g_ for global, s_ for static, m_ for module
// - Use descriptive names that indicate scope/purpose
// - Compile with -Wshadow to detect hiding
