// Test file for: automotive-unterminated-escape-sequence
// Related MISRA C:2025 Rule: 4.1
//
// This check detects problematic escape sequences that may be ambiguous
// or cause unexpected behavior.

// RUN: %check_clang_tidy %s automotive-unterminated-escape-sequence %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_escape_violations(void) {
    // Octal escape followed by digit - ambiguous
    // \12 is octal (newline), then followed by digit 9
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: octal escape sequence '\12' is followed by digit '9'
    char *s1 = "\129";

    // Octal escape \77 followed by digit 8 (8 is not octal, so it's ambiguous)
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: octal escape sequence '\77' is followed by digit '8'
    char *s2 = "\778";
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    // Valid escape sequences - no warnings
    char *s1 = "hello\n";      // newline
    char *s2 = "hello\t";      // tab
    char *s3 = "hello\\";      // backslash
    char *s4 = "hello\"";      // quote
    char *s5 = "hello\'";      // single quote
    char *s6 = "\x41";         // hex escape (A) - 2 digits OK
    char *s7 = "\101";         // octal escape (A) - 3 octal digits
    char *s8 = "hello\0world"; // null character followed by letter (OK)
    char *s9 = "\a\b\f\r\v";   // other valid escapes

    // Octal followed by non-digit letter - OK
    char *s10 = "\0abc";       // \0 followed by 'a' - clear

    // IP addresses, paths - no escapes here, no warnings
    char *ip = "127.0.0.1";
    char *path = "/usr/bin";
    char *env = "SIMULATION";
}
