// MISRA C:2025 Rule 7.1 - Octal constants shall not be used
// Category: Required
// Status: VIOLATION
//
// This code violates Rule 7.1 because it uses octal literal constants.
// Octal notation (numbers starting with 0) can be confused with decimal,
// leading to unexpected values.
//
// Expected diagnostic:
//   warning: octal literal detected; use decimal or hexadecimal notation
//   [automotive-avoid-octal-literal]

#include <stdio.h>

// Array sizes using octal (likely unintended)
#define BUFFER_SIZE 0100   // VIOLATION: This is 64 in decimal, not 100!
#define PERMISSION  0644   // VIOLATION: Octal file permission

void init_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

int main(void) {
    int buffer[BUFFER_SIZE];

    // Another octal literal
    int value = 010;  // VIOLATION: This is 8 in decimal, not 10!

    printf("Buffer size: %d\n", BUFFER_SIZE);  // Prints 64
    printf("Value: %d\n", value);               // Prints 8

    init_array(buffer, BUFFER_SIZE);
    return 0;
}
