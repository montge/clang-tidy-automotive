// MISRA C:2025 Rule 4.1 - Escape sequences
// Category: Required
// Status: VIOLATION
//
// Only defined escape sequences shall be used.
//
// Expected diagnostic:
//   warning: undefined escape sequence [automotive-unterminated-escape-sequence]

#include <stdio.h>

void escape_examples(void) {
    // VIOLATION: Undefined escape sequences
    char *path1 = "\p\a\t\h";     // \p is undefined
    char *msg1 = "Hello\qWorld";  // \q is undefined

    // VIOLATION: Octal escape not complete (could be ambiguous)
    char *str1 = "\1789";  // \17 is octal, 89 are separate chars

    // VIOLATION: Hex escape runs into next chars
    char *str2 = "\xABCDEF";  // Hex value too large

    // VIOLATION: Using \e for escape (common but non-standard)
    char *ansi = "\e[31m";  // \e is a GCC extension

    (void)path1;
    (void)msg1;
    (void)str1;
    (void)str2;
    (void)ansi;
}
