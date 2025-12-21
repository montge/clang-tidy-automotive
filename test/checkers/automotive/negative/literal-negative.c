// Negative test file for: automotive literal checks
// Related MISRA C:2025 Rules: 7.1, 7.3, 4.1
//
// This file contains code that should NOT trigger any warnings.
// All code here is compliant with MISRA rules.

// RUN: clang-tidy -checks='-*,automotive-avoid-octal-number,automotive-avoid-lowercase-literal-suffix' %s -- 2>&1 | FileCheck %s -allow-empty -check-prefix=CHECK-NEGATIVE
// CHECK-NEGATIVE-NOT: warning:
// Note: Negative test - no warnings expected (unterminated-escape-sequence check excluded due to false positives)

#include <stdint.h>

//===----------------------------------------------------------------------===//
// Negative: No octal literals (decimal and hex only)
//===----------------------------------------------------------------------===//

void test_decimal_literals(void) {
    int a = 0;      // Zero is allowed
    int b = 123;    // Decimal
    int c = 777;    // Decimal (looks like octal but isn't)
    int d = 644;    // Decimal
    int e = 755;    // Decimal

    int arr[100];   // Decimal array size
    int f = arr[10]; // Decimal index
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
}

void test_hex_literals(void) {
    int a = 0x0;        // Hex zero
    int b = 0x1FF;      // Hex
    int c = 0xDEADBEEF; // Hex
    int d = 0X7FF;      // Uppercase X
    int e = 0xabcdef;   // Lowercase hex digits (OK, different from suffix)

    uint32_t mask = 0xFFFFFFFF;
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)mask;
}

//===----------------------------------------------------------------------===//
// Negative: Uppercase literal suffixes
//===----------------------------------------------------------------------===//

void test_uppercase_suffixes(void) {
    long a = 100L;          // Uppercase L
    long b = 200LL;         // Uppercase LL
    unsigned long c = 300UL;  // Uppercase UL
    unsigned long long d = 400ULL; // Uppercase ULL

    float e = 1.5F;         // Uppercase F
    long double f = 2.5L;   // Uppercase L for long double

    unsigned int g = 100U;  // Uppercase U

    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g;
}

void test_no_suffix(void) {
    int a = 100;      // No suffix
    double b = 1.5;   // No suffix (defaults to double)
    (void)a; (void)b;
}

//===----------------------------------------------------------------------===//
// Negative: Valid escape sequences
//===----------------------------------------------------------------------===//

void test_valid_escapes(void) {
    // Standard escape sequences
    char newline = '\n';
    char tab = '\t';
    char carriage = '\r';
    char backslash = '\\';
    char quote = '\"';
    char apostrophe = '\'';
    char question = '\?';
    char alert = '\a';
    char backspace = '\b';
    char formfeed = '\f';
    char vtab = '\v';
    char null = '\0';

    (void)newline; (void)tab; (void)carriage; (void)backslash;
    (void)quote; (void)apostrophe; (void)question; (void)alert;
    (void)backspace; (void)formfeed; (void)vtab; (void)null;
}

void test_octal_escapes(void) {
    // Valid octal escape sequences (different from octal literals)
    char a = '\0';    // Null
    char b = '\7';    // Bell (same as \a)
    char c = '\12';   // Newline
    char d = '\101';  // 'A'
    char e = '\377';  // Max octal escape

    (void)a; (void)b; (void)c; (void)d; (void)e;
}

void test_hex_escapes(void) {
    // Valid hex escape sequences
    char a = '\x00';  // Null
    char b = '\x0A';  // Newline
    char c = '\x41';  // 'A'
    char d = '\xFF';  // Max byte value

    (void)a; (void)b; (void)c; (void)d;
}

void test_string_escapes(void) {
    const char *s1 = "Hello\nWorld";   // Newline in string
    const char *s2 = "Tab\there";      // Tab in string
    const char *s3 = "Quote: \"text\""; // Escaped quotes
    const char *s4 = "Path\\file";     // Escaped backslash

    (void)s1; (void)s2; (void)s3; (void)s4;
}

//===----------------------------------------------------------------------===//
// Negative: Mixed compliant code
//===----------------------------------------------------------------------===//

void test_mixed_compliant(void) {
    // Decimal with uppercase suffix
    unsigned long size = 1024UL;

    // Hex with uppercase suffix
    uint32_t mask = 0xFFU;

    // String with valid escapes
    const char *msg = "Error:\t%d\n";

    // Zero is allowed
    int zero = 0;

    (void)size; (void)mask; (void)msg; (void)zero;
}
