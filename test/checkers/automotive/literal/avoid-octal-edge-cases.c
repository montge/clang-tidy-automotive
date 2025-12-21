// Test file for: automotive-avoid-octal-number (Edge Cases)
// Related MISRA C:2025 Rule: 7.1
//
// This file tests edge cases for octal literal detection

// RUN: %check_clang_tidy %s automotive-avoid-octal-number %t

//===----------------------------------------------------------------------===//
// Edge Case: Octal in array indices and sizes
//===----------------------------------------------------------------------===//

void test_octal_in_array(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid octal number
    int arr[0100];  // Array size as octal (64 decimal)

    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: Avoid octal number
    int x = arr[010];  // Array index as octal
}

//===----------------------------------------------------------------------===//
// Edge Case: Octal in expressions
//===----------------------------------------------------------------------===//

void test_octal_in_expressions(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid octal number
    int a = 0777 | 0x0F;  // Octal in bitwise OR

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid octal number
    int b = 0644 + 100;   // Octal in addition

    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: Avoid octal number
    int c = (0123) * 2;   // Octal in parentheses
}

//===----------------------------------------------------------------------===//
// Edge Case: Octal in macro definitions (if expanded)
//===----------------------------------------------------------------------===//

#define OCTAL_CONST 0755
#define DECIMAL_CONST 755

void test_macro_expansion(void) {
    // Note: Whether this triggers depends on macro expansion handling
    int perms = OCTAL_CONST;  // Macro containing octal
    int dec = DECIMAL_CONST;  // Macro containing decimal (no warning)
}

//===----------------------------------------------------------------------===//
// Edge Case: Octal escape sequences in strings (different from octal literals)
//===----------------------------------------------------------------------===//

void test_escape_sequences(void) {
    // These are escape sequences, NOT octal literals - should NOT warn
    char newline = '\012';   // Octal escape for newline
    char tab = '\011';       // Octal escape for tab
    char *str = "Hello\012"; // Octal escape in string
}

//===----------------------------------------------------------------------===//
// Edge Case: Single digit octals
//===----------------------------------------------------------------------===//

void test_single_digit(void) {
    // Single digit starting with 0 are technically octal
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid octal number
    int a = 01;

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid octal number
    int b = 07;

    // 0 alone is NOT octal - it's zero
    int zero = 0;  // Should NOT warn
}

//===----------------------------------------------------------------------===//
// Edge Case: Boundary values
//===----------------------------------------------------------------------===//

void test_boundary_values(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: Avoid octal number
    unsigned long max = 037777777777;  // Max 32-bit octal

    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: Avoid octal number
    int min = 00;  // This is still octal notation for zero
}

//===----------------------------------------------------------------------===//
// Compliant Cases
//===----------------------------------------------------------------------===//

void test_compliant_cases(void) {
    // Zero literal
    int zero = 0;

    // Decimal literals
    int dec1 = 777;
    int dec2 = 644;
    int dec3 = 100;

    // Hexadecimal literals
    int hex1 = 0x1FF;
    int hex2 = 0x0;    // Hex zero
    int hex3 = 0X7FF;  // Uppercase X

    // Float with exponent starting with 0
    float f = 1.0e01;  // Not octal, it's float exponent
}
