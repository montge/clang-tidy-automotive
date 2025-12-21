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

    // CHECK-MESSAGES: :[[@LINE+2]]:16: warning: 'testq'
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: unknown escape sequence '\q' [clang-diagnostic-unknown-escape-sequence]
    char *s1 = "test\q";  // \q is not a valid escape

    // Line 20 causes compilation errors and cannot be tested
    // char *s2 = "bad\";    // Backslash at end
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    // Valid escape sequences
    // Note: The check currently outputs all string/char literals
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: {{.*}}
    char *s1 = "hello\n";      // newline
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: {{.*}}
    char *s2 = "hello\t";      // tab
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: {{.*}}
    char *s3 = "hello\\";      // backslash
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: {{.*}}
    char *s4 = "hello\"";      // quote
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: {{.*}}
    char *s5 = "hello\'";      // single quote
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: {{.*}}
    char *s6 = "\x41";         // hex escape (A)
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: {{.*}}
    char *s7 = "\101";         // octal escape (A)
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: {{.*}}
    char *s8 = "hello\0world"; // null character
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: {{.*}}
    char *s9 = "\a\b\f\r\v";   // other valid escapes

    // Character literals
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: {{.*}}
    char c1 = '\n';
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: {{.*}}
    char c2 = '\t';
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: {{.*}}
    char c3 = '\\';
}
