// MISRA C:2025 Directive 4.7 - Error information testing
// Category: Required
// Status: VIOLATION
//
// Error information from functions should be tested.
// This code ignores return values and error indicators.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// VIOLATION: Ignoring return value from fopen
void read_config_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    // No check if fopen failed (returns NULL on error)
    char buffer[256];
    fgets(buffer, sizeof(buffer), fp);  // Undefined behavior if fp is NULL
    fclose(fp);
}

// VIOLATION: Ignoring return value from malloc
void allocate_buffer(size_t size) {
    int *buffer = malloc(size * sizeof(int));
    // No check if malloc failed (returns NULL on error)
    buffer[0] = 42;  // Undefined behavior if buffer is NULL
    free(buffer);
}

// VIOLATION: Ignoring return value indicating success/failure
int perform_operation(int *result);  // Returns 0 on success, -1 on failure

void process_data(void) {
    int value;
    perform_operation(&value);  // Return value ignored
    // Using value without knowing if operation succeeded
    printf("Value: %d\n", value);
}

// VIOLATION: Ignoring errno after library call
void parse_number(const char *str) {
    long value = strtol(str, NULL, 10);
    // errno not checked - overflow/underflow not detected
    printf("Parsed: %ld\n", value);
}
