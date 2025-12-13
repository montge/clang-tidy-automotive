// MISRA C:2025 Rule 2.6 - Unused label declarations
// Category: Advisory
// Status: VIOLATION
//
// This code violates Rule 2.6 because it contains a label that is
// declared but never referenced by a goto statement.
//
// Expected diagnostic:
//   warning: label 'cleanup' is defined but never used
//   [automotive-unused-label]

#include <stdio.h>

int process_data(int value) {
    int result = 0;

    if (value < 0) {
        result = -1;
        // Programmer intended to use goto but forgot
        return result;
    }

    result = value * 2;

cleanup:  // VIOLATION: Label is never used
    printf("Processing complete\n");
    return result;
}

int main(void) {
    int r = process_data(5);
    return (r < 0) ? 1 : 0;
}
