// MISRA C:2025 Rule 8.1 - Explicit types
// Category: Required
// Status: VIOLATION
//
// Types shall be explicitly stated.
//
// Expected diagnostic:
//   warning: implicit int type [automotive-implicit-int]

// VIOLATION: Implicit int return type (K&R style)
calculate(int x) {  // Missing return type - defaults to int
    return x * 2;
}

// VIOLATION: Implicit int for function parameter (K&R style)
void process(value) {  // Missing type for 'value'
    (void)value;
}

// VIOLATION: const without type
const MAX_VALUE = 100;  // Implicit int

// VIOLATION: static without type
static counter = 0;  // Implicit int

// VIOLATION: extern without type
extern global_flag;  // Implicit int

// Note: Implicit int was allowed in C89/C90 but is
// obsolescent in C99 and removed in C11.
