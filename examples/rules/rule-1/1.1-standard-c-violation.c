// MISRA C:2025 Rule 1.1 - Standard C conformance
// Category: Required
// Status: VIOLATION
//
// Code shall conform to the standard C syntax and constraints.
// Non-standard extensions should not be used.

// VIOLATION: Using GCC-specific typeof extension
#define MAX(a, b) ({            \
    typeof(a) _a = (a);         \
    typeof(b) _b = (b);         \
    _a > _b ? _a : _b;          \
})

// VIOLATION: Using zero-length array (GCC extension)
struct message {
    int length;
    char data[0];  // Non-standard zero-length array
};

// VIOLATION: Using nested functions (GCC extension)
int outer_function(int x) {
    int inner(int y) {  // Non-standard nested function
        return y * 2;
    }
    return inner(x);
}

// VIOLATION: Using __attribute__ (compiler extension)
void deprecated_func(void) __attribute__((deprecated));

// VIOLATION: Binary literals (C23 feature, not C11/C17)
int binary_value = 0b10101010;  // Not standard until C23

// VIOLATION: Using asm keyword directly (use __asm__ for portability)
void inline_asm_example(void) {
    asm("nop");
}
