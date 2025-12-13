// MISRA C:2025 Rule 2.1 - Unreachable code
// Category: Required
// Status: VIOLATION
//
// A project shall not contain unreachable code.
//
// Expected diagnostic:
//   warning: unreachable code [clang-diagnostic-unreachable-code]

#include <stdbool.h>

// VIOLATION: Code after return is unreachable
int calculate(int x) {
    return x * 2;
    x = x + 1;  // Unreachable
}

// VIOLATION: Code after infinite loop
void process_forever(void) {
    while (true) {
        // Process
    }
    cleanup();  // Unreachable
}

// VIOLATION: Code in always-false condition
void conditional_dead_code(int x) {
    if (false) {
        x = x + 1;  // Unreachable
    }
}

// VIOLATION: Unreachable case in switch
int switch_example(int cmd) {
    switch (cmd) {
        case 1:
            return 1;
            break;  // Unreachable - after return
        case 2:
            return 2;
        default:
            return 0;
    }
}

// VIOLATION: Code after abort/exit
#include <stdlib.h>
void fatal_error(void) {
    abort();
    log_error();  // Unreachable
}
