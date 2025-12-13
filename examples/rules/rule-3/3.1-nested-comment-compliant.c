// MISRA C:2025 Rule 3.1 - Comment markers within comments
// Category: Required
// Status: COMPLIANT
//
// This code complies with Rule 3.1 by avoiding comment markers
// within comments. Use preprocessor directives for disabling code.

#include <stdio.h>

/* This is a properly written comment without nested markers */

int get_value(void) {
    /* Use preprocessor directives to disable code instead of
       commenting it out with nested comments */
#if 0
    int temp = calculate();
    return temp;
#endif
    return 42;
}

/* Single-line descriptions are clear and unambiguous */

int main(void) {
    printf("Value: %d\n", get_value());
    return 0;
}
