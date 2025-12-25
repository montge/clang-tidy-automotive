// MISRA C:2025 Rule 20.15 Violation Examples
// #define and #undef shall not be used on a reserved identifier or reserved macro name

// Violation: Defining identifier starting with underscore + uppercase
#define _Reserved 1

// Violation: Defining identifier starting with double underscore
#define __internal 2

// Violation: Redefining standard library macro
#define NULL 0

// Violation: Redefining standard library identifier
#define errno 5

// Violation: Redefining standard I/O identifiers
#define stdin ((void*)0)
#define stdout ((void*)0)
#define stderr ((void*)0)

// Violation: Undefining reserved identifier
#undef NULL

void example_function(void) {
    int x = _Reserved;
    (void)x;
}
