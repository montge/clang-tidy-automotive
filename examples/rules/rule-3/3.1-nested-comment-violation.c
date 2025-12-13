// MISRA C:2025 Rule 3.1 - Comment markers within comments
// Category: Required
// Status: VIOLATION
//
// This code violates Rule 3.1 because it contains comment markers
// (/* or //) within a comment, which can cause confusion.
//
// Expected diagnostic:
//   warning: '/*' within block comment
//   [automotive-avoid-comment-within-comment]

#include <stdio.h>

/* This is a comment that contains /* another comment marker */
// which violates MISRA Rule 3.1

int get_value(void) {
    /* The following code was disabled:
       /* int temp = calculate(); */
       return temp;
    */
    return 42;  // VIOLATION: nested comment markers above
}

// This comment contains // another C++ comment marker  // VIOLATION

int main(void) {
    printf("Value: %d\n", get_value());
    return 0;
}
