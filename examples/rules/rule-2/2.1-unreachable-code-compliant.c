// MISRA C:2025 Rule 2.1 - Unreachable code
// Category: Required
// Status: COMPLIANT
//
// All code in the project is reachable.

#include <stdbool.h>
#include <stdint.h>

// COMPLIANT: No code after return
int calculate(int x) {
    int result = x * 2;
    return result;
}

// COMPLIANT: Loop has exit condition, code after is reachable
void process_until_done(volatile bool *done) {
    while (!(*done)) {
        // Process
    }
    cleanup();  // Reachable when done becomes true
}

// COMPLIANT: Condition may be true at runtime
void conditional_code(int x) {
    if (x > 0) {
        x = x + 1;  // Reachable when x > 0
    }
}

// COMPLIANT: No code after return in switch cases
int switch_example(int cmd) {
    int result;
    switch (cmd) {
        case 1:
            result = 1;
            break;
        case 2:
            result = 2;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

// COMPLIANT: Cleanup before exit
void fatal_error(void) {
    log_error();  // Executed before abort
    cleanup_resources();
    abort();
    // No code after abort
}

// Helper declarations
void cleanup(void);
void log_error(void);
void cleanup_resources(void);
