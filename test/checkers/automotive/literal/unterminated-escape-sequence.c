// Test file for: automotive-unterminated-escape-sequence
// Related MISRA C:2025 Rule: 4.1
//
// This file tests the detection of invalid escape sequences

// RUN: %check_clang_tidy %s automotive-unterminated-escape-sequence %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_escape_violations(void) {
    // Invalid escape sequences
    // Note: These may cause compiler warnings too

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unterminated escape sequence
    char *s1 = "test\q";  // \q is not a valid escape

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: unterminated escape sequence
    char *s2 = "bad\";    // Backslash at end
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    // Valid escape sequences
    char *s1 = "hello\n";      // newline
    char *s2 = "hello\t";      // tab
    char *s3 = "hello\\";      // backslash
    char *s4 = "hello\"";      // quote
    char *s5 = "hello\'";      // single quote
    char *s6 = "\x41";         // hex escape (A)
    char *s7 = "\101";         // octal escape (A)
    char *s8 = "hello\0world"; // null character
    char *s9 = "\a\b\f\r\v";   // other valid escapes

    // Character literals
    char c1 = '\n';
    char c2 = '\t';
    char c3 = '\\';
}
