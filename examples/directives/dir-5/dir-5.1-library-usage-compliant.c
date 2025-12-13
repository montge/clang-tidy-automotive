// MISRA C:2025 Directive 5.1 - C standard library usage
// Category: Required
// Status: COMPLIANT
//
// Library functions are used within their defined specifications.
// All preconditions are checked before calling library functions.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>

// COMPLIANT: Use memmove for potentially overlapping buffers
void shift_buffer(char *buffer, size_t size) {
    if ((buffer == NULL) || (size <= 1U)) {
        return;
    }
    // memmove handles overlapping regions correctly
    (void)memmove(buffer, buffer + 1, size - 1U);
}

// COMPLIANT: Validate argument is in valid range for ctype functions
bool is_alpha_safe(int ch) {
    // ctype functions require: EOF or value representable as unsigned char
    if ((ch != EOF) && ((ch < 0) || (ch > UCHAR_MAX))) {
        return false;  // Invalid input
    }
    return isalpha(ch) != 0;
}

// COMPLIANT: Validate pointers before string operations
bool safe_string_copy(char *dest, size_t dest_size, const char *src) {
    if ((dest == NULL) || (src == NULL) || (dest_size == 0U)) {
        return false;
    }

    size_t src_len = strlen(src);
    if (src_len >= dest_size) {
        return false;  // Destination too small
    }

    (void)strcpy(dest, src);
    return true;
}

// COMPLIANT: Handle INT_MIN case for absolute value
int get_absolute_safe(int value, bool *overflow) {
    if (overflow != NULL) {
        *overflow = false;
    }

    // Check for overflow case: abs(INT_MIN) is undefined
    if (value == INT_MIN) {
        if (overflow != NULL) {
            *overflow = true;
        }
        return INT_MAX;  // Return maximum representable value
    }

    return abs(value);
}

// COMPLIANT: Proper comparison function for qsort
int proper_compare(const void *a, const void *b) {
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;

    // Return negative, zero, or positive for proper ordering
    if (*ia < *ib) {
        return -1;
    }
    if (*ia > *ib) {
        return 1;
    }
    return 0;
}

// Example usage of qsort with proper comparison
void sort_array(int *array, size_t count) {
    if ((array != NULL) && (count > 1U)) {
        qsort(array, count, sizeof(int), proper_compare);
    }
}
