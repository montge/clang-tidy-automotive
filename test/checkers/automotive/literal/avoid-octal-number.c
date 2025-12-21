// Test file for: automotive-avoid-octal-number
// Related MISRA C:2025 Rule: 7.1
//
// This file tests the detection of octal literal constants

// RUN: %check_clang_tidy %s automotive-avoid-octal-number %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_octal_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid octal number
    int a = 0123;  // Octal 123 = decimal 83

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid octal number
    int b = 0777;  // Octal 777 = decimal 511

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid octal number
    int c = 010;   // Octal 10 = decimal 8

    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: Avoid octal number
    int arr[] = { 0755, 0644 };  // Multiple octals
    // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: Avoid octal number
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    // Zero is allowed
    int zero = 0;

    // Decimal literals
    int dec1 = 123;
    int dec2 = 777;
    int dec3 = 10;

    // Hexadecimal literals
    int hex1 = 0x123;
    int hex2 = 0xFF;
    int hex3 = 0xDEADBEEF;

    // Binary literals (C23)
    // int bin = 0b1010;
}
