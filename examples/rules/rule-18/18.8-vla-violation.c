// MISRA C:2025 Rule 18.8 - Variable-length array types shall not be used
// Category: Required
// Status: VIOLATION
//
// This code violates Rule 18.8 because it uses variable-length arrays (VLAs).
// VLAs can cause stack overflow in constrained environments and have
// unpredictable memory usage.
//
// Expected diagnostic:
//   warning: variable length array declared
//   [automotive-avoid-variable-length-array]

#include <stdio.h>

void process_matrix(int rows, int cols) {
    int matrix[rows][cols];  // VIOLATION: VLA

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j;
        }
    }

    printf("Matrix[0][0] = %d\n", matrix[0][0]);
}

void process_buffer(int size) {
    char buffer[size];  // VIOLATION: VLA

    for (int i = 0; i < size; i++) {
        buffer[i] = (char)('A' + (i % 26));
    }
    buffer[size - 1] = '\0';

    printf("Buffer: %s\n", buffer);
}

int main(void) {
    process_matrix(3, 4);
    process_buffer(100);
    return 0;
}
