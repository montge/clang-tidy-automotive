// MISRA C:2025 Rule 18.8 - Variable-length array types shall not be used
// Category: Required
// Status: COMPLIANT
//
// This code complies with Rule 18.8 by using fixed-size arrays with
// compile-time constants. This ensures predictable memory usage.

#include <stdio.h>
#include <stdbool.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MAX_BUFFER_SIZE 256

bool process_matrix(int rows, int cols) {
    if (rows > MAX_ROWS || cols > MAX_COLS) {
        return false;  // Size validation
    }

    int matrix[MAX_ROWS][MAX_COLS];  // Fixed-size array

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }

    printf("Matrix[0][0] = %d\n", matrix[0][0]);
    return true;
}

bool process_buffer(int size) {
    if (size > MAX_BUFFER_SIZE || size < 1) {
        return false;  // Size validation
    }

    char buffer[MAX_BUFFER_SIZE];  // Fixed-size array

    for (int i = 0; i < size; i++) {
        buffer[i] = (char)('A' + (i % 26));
    }
    buffer[size - 1] = '\0';

    printf("Buffer: %s\n", buffer);
    return true;
}

int main(void) {
    (void)process_matrix(3, 4);
    (void)process_buffer(100);
    return 0;
}
