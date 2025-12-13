// MISRA C:2025 Rule 4.1 - Escape sequences
// Category: Required
// Status: COMPLIANT
//
// Only standard escape sequences are used:
// \' \" \? \\ \a \b \f \n \r \t \v
// \ooo (octal) \xhh (hexadecimal)

#include <stdio.h>

void escape_examples(void) {
    // COMPLIANT: Standard escape sequences
    char newline = '\n';
    char tab = '\t';
    char backslash = '\\';
    char quote = '\"';
    char alert = '\a';
    char backspace = '\b';
    char formfeed = '\f';
    char carriage = '\r';
    char vtab = '\v';

    // COMPLIANT: Octal escapes (1-3 digits)
    char octal1 = '\0';    // Null character
    char octal2 = '\12';   // Same as \n (newline)
    char octal3 = '\101';  // 'A'

    // COMPLIANT: Hex escapes with clear boundaries
    char hex1 = '\x0A';          // Newline
    char hex2 = '\x41';          // 'A'
    char *str = "\x41" "BCD";    // String concatenation for clarity

    // COMPLIANT: For ANSI escapes, use explicit value
    char *ansi = "\x1B[31m";     // ESC character is 0x1B (27)

    (void)newline;
    (void)tab;
    (void)backslash;
    (void)quote;
    (void)alert;
    (void)backspace;
    (void)formfeed;
    (void)carriage;
    (void)vtab;
    (void)octal1;
    (void)octal2;
    (void)octal3;
    (void)hex1;
    (void)hex2;
    (void)str;
    (void)ansi;
}
