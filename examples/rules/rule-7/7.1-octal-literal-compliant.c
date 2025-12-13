// MISRA C:2025 Rule 7.1 - Octal constants shall not be used
// Category: Required
// Status: COMPLIANT
//
// This code complies with Rule 7.1 by using decimal or hexadecimal
// notation instead of octal. This makes the values immediately clear.

#include <stdio.h>

// Use decimal for array sizes
#define BUFFER_SIZE 100    // Clear: one hundred elements

// Use hexadecimal for bit patterns and permissions
#define PERMISSION  0x1A4  // 0644 octal = 420 decimal = 0x1A4 hex

void init_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }
}

int main(void) {
    int buffer[BUFFER_SIZE];

    // Use decimal for numeric values
    int value = 10;  // Clear: ten

    printf("Buffer size: %d\n", BUFFER_SIZE);  // Prints 100
    printf("Value: %d\n", value);               // Prints 10

    init_array(buffer, BUFFER_SIZE);
    return 0;
}
