// MISRA C:2025 Rule 15.1 - The goto statement should not be used
// Category: Advisory
// Status: VIOLATION
//
// This code violates Rule 15.1 because it uses the goto statement,
// which can lead to unstructured code that is difficult to understand
// and maintain.
//
// Expected diagnostic:
//   warning: goto statement detected; consider using structured
//   control flow [automotive-avoid-goto]

#include <stdio.h>

int process_data(int *data, int size) {
    int result = 0;

    for (int i = 0; i < size; i++) {
        if (data[i] < 0) {
            goto error;  // VIOLATION: goto statement
        }
        result += data[i];
    }

    return result;

error:
    printf("Error: negative value found\n");
    return -1;
}

int main(void) {
    int data[] = {1, 2, -3, 4, 5};
    int result = process_data(data, 5);
    return (result < 0) ? 1 : 0;
}
