// MISRA C:2025 Rule 15.1 - The goto statement should not be used
// Category: Advisory
// Status: COMPLIANT
//
// This code complies with Rule 15.1 by using structured control flow
// instead of goto statements. Error handling is performed using
// return values and conditional logic.

#include <stdio.h>
#include <stdbool.h>

int process_data(int *data, int size, bool *error) {
    int result = 0;
    *error = false;

    for (int i = 0; i < size; i++) {
        if (data[i] < 0) {
            *error = true;
            printf("Error: negative value found\n");
            return -1;  // Early return instead of goto
        }
        result += data[i];
    }

    return result;
}

int main(void) {
    int data[] = {1, 2, -3, 4, 5};
    bool error;
    int result = process_data(data, 5, &error);
    return error ? 1 : 0;
}
