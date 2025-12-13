// MISRA C:2025 Directive 4.7 - Error information testing
// Category: Required
// Status: COMPLIANT
//
// All error information from functions is properly tested
// and handled appropriately.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

// COMPLIANT: Check return value from fopen
bool read_config_file(const char *filename, char *buffer, size_t buf_size) {
    if ((filename == NULL) || (buffer == NULL) || (buf_size == 0U)) {
        return false;
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        // Error: file could not be opened
        return false;
    }

    char *result = fgets(buffer, (int)buf_size, fp);
    (void)fclose(fp);  // Cast to void if return value intentionally ignored

    return (result != NULL);
}

// COMPLIANT: Check return value from malloc
int *allocate_buffer(size_t count) {
    if (count == 0U) {
        return NULL;
    }

    int *buffer = malloc(count * sizeof(int));
    if (buffer == NULL) {
        // Error: allocation failed
        return NULL;
    }

    buffer[0] = 42;
    return buffer;
}

// COMPLIANT: Check return value indicating success/failure
int perform_operation(int *result);

bool process_data(int *output) {
    if (output == NULL) {
        return false;
    }

    int value;
    int status = perform_operation(&value);
    if (status != 0) {
        // Error: operation failed
        return false;
    }

    *output = value;
    return true;
}

// COMPLIANT: Check errno after library call
bool parse_number(const char *str, long *result) {
    if ((str == NULL) || (result == NULL)) {
        return false;
    }

    char *endptr;
    errno = 0;  // Reset errno before call

    long value = strtol(str, &endptr, 10);

    // Check for conversion errors
    if ((errno != 0) || (endptr == str) || (*endptr != '\0')) {
        return false;
    }

    *result = value;
    return true;
}
