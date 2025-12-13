// MISRA C:2025 Rule 2.6 - Unused label declarations
// Category: Advisory
// Status: COMPLIANT
//
// This code complies with Rule 2.6 - no unused labels are present.
// The code uses structured control flow instead of labels.

#include <stdio.h>

int process_data(int value) {
    int result;

    if (value < 0) {
        result = -1;
    } else {
        result = value * 2;
    }

    printf("Processing complete\n");
    return result;
}

int main(void) {
    int r = process_data(5);
    return (r < 0) ? 1 : 0;
}
