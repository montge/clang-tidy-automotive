// MISRA C:2025 Rule 1.1 - Standard C conformance
// Category: Required
// Status: COMPLIANT
//
// Code conforms to standard C syntax and constraints.
// All extensions are avoided or properly encapsulated.

#include <stdint.h>

// COMPLIANT: Standard-conforming macro (though less type-safe)
#define MAX_INT(a, b) ((a) > (b) ? (a) : (b))

// COMPLIANT: Using flexible array member (standard since C99)
struct message {
    int length;
    char data[];  // Standard flexible array member
};

// COMPLIANT: Using function pointer instead of nested function
typedef int (*transform_fn)(int);

static int double_value(int y) {
    return y * 2;
}

int outer_function(int x, transform_fn transform) {
    return transform(x);
}

// Example usage: outer_function(5, double_value);

// COMPLIANT: Deprecation handled via documentation or static analysis
// Instead of __attribute__((deprecated)), document in comments:
// @deprecated This function is deprecated, use new_func() instead
void deprecated_func(void);

// COMPLIANT: Using hexadecimal instead of binary literals
int hex_value = 0xAA;  // Equivalent to 0b10101010

// COMPLIANT: Assembly in separate platform-specific files
// Or using standard-ish __asm__ volatile if absolutely required
// See Dir 2.1 examples for proper assembly isolation
