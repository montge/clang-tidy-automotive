// MISRA C:2025 Directive 5.1 - C standard library usage
// Category: Required
// Status: VIOLATION
//
// Only defined library functions should be used.
// Library functions should be used according to their specification.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// VIOLATION: Using undefined behavior with standard library functions

// memcpy with overlapping buffers (undefined behavior)
void shift_buffer(char *buffer, size_t size) {
    // Source and destination overlap - use memmove instead
    memcpy(buffer, buffer + 1, size - 1);
}

// VIOLATION: Using library function outside its defined domain
int classify_char(int ch) {
    // isalpha requires ch to be EOF or representable as unsigned char
    // Passing arbitrary int values is undefined behavior
    return isalpha(ch);  // ch might not be in valid range
}

// VIOLATION: Using library function with NULL argument
void unsafe_string_op(void) {
    char *src = NULL;
    char dest[32];
    // strcpy with NULL source is undefined behavior
    strcpy(dest, src);
}

// VIOLATION: Using abs() with INT_MIN (undefined behavior on 2's complement)
#include <limits.h>
int get_absolute(int value) {
    // abs(INT_MIN) is undefined when |INT_MIN| > INT_MAX
    return abs(value);
}

// VIOLATION: Using qsort with inconsistent comparison function
int bad_compare(const void *a, const void *b) {
    // Comparison function must define a total ordering
    // This doesn't handle all cases correctly
    return *(const int *)a > *(const int *)b;  // Missing equality/less-than
}
