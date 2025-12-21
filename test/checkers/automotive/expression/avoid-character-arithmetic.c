// Test file for: automotive-avoid-character-arithmetic
// Related MISRA C:2025 Rule: 10.2
//
// This file tests the detection of arithmetic operations between character types

// RUN: %check_clang_tidy %s automotive-c23-req-10.2 %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_char_arithmetic_violations(void) {
    char c1 = 'A';
    char c2 = 'B';
    signed char sc1 = 10;
    signed char sc2 = 20;
    unsigned char uc1 = 30;
    unsigned char uc2 = 40;

    // Addition between two char variables
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    char result1 = c1 + c2;

    // Subtraction between two char variables
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    char result2 = c1 - c2;

    // Addition with signed char
    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    signed char result3 = sc1 + sc2;

    // Subtraction with signed char
    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    signed char result4 = sc1 - sc2;

    // Addition with unsigned char
    // CHECK-MESSAGES: :[[@LINE+1]]:33: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    unsigned char result5 = uc1 + uc2;

    // Subtraction with unsigned char
    // CHECK-MESSAGES: :[[@LINE+1]]:33: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    unsigned char result6 = uc1 - uc2;

    // Compound assignment with addition
    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    c1 += c2;

    // Compound assignment with subtraction
    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    sc1 -= sc2;

    // Expression with character literals
    // Suppress clang diagnostic for constant conversion
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wconstant-conversion"
    char result7 = 'X' + 'Y';
    #pragma clang diagnostic pop

    char result8 = 'Z' - 'A';
}

void test_mixed_char_types(void) {
    char c = 'A';
    signed char sc = 10;
    unsigned char uc = 20;

    // char + signed char
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    char result1 = c + sc;

    // char + unsigned char
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    char result2 = c + uc;

    // signed char + unsigned char
    // CHECK-MESSAGES: :[[@LINE+1]]:30: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    signed char result3 = sc + uc;
}

void test_in_expressions(void) {
    char a = 'A';
    char b = 'B';
    char c = 'C';

    // Character arithmetic in conditional
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    if (a + b > c) {
        // Do something
    }

    // In array index
    char arr[100];
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    arr[a + b] = 0;

    // In function call
    int value;
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: addition or subtraction between two expressions of essentially character type is not allowed [automotive-c23-req-10.2]
    value = (int)(a + b);
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_operations(void) {
    char c1 = 'A';
    char c2 = 'B';
    int i1 = 10;
    int i2 = 20;

    // Character with integer - allowed
    char result1 = c1 + i1;
    char result2 = c2 - i2;

    // Integer arithmetic
    int result3 = i1 + i2;
    int result4 = i1 - i2;

    // Character assignment (not arithmetic)
    char result5 = c1;
    char result6 = 'X';

    // Comparison (not arithmetic)
    if (c1 > c2) {
        // OK
    }
    if (c1 == 'A') {
        // OK
    }

    // Bitwise operations (not arithmetic + or -)
    char result7 = c1 & c2;  // Bitwise AND - OK
    char result8 = c1 | c2;  // Bitwise OR - OK
    char result9 = c1 ^ c2;  // Bitwise XOR - OK
}

void test_integer_addition(void) {
    int a = 65;  // ASCII 'A'
    int b = 66;  // ASCII 'B'

    // Integer addition is fine
    int result = a + b;

    // Cast to char after arithmetic
    char c = (char)(a + b);
}

void test_pointer_arithmetic(void) {
    char str[] = "Hello";
    char *ptr = str;

    // Pointer arithmetic with integer - allowed
    ptr = ptr + 1;
    ptr = ptr - 1;

    // Pointer difference - allowed (result is ptrdiff_t)
    long diff = &str[5] - &str[0];
}

void test_array_indexing(void) {
    char arr[100];
    int index = 10;
    char ch = 'A';

    // Array indexing with integer
    arr[index] = ch;

    // Reading array element
    ch = arr[index];

    // Incrementing integer index
    index = index + 1;
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_edge_cases(void) {
    char c = 'A';
    int i = 1;

    // Unary operations on char (not binary arithmetic)
    char result1 = +c;  // Unary plus
    char result2 = -c;  // Unary minus
    c++;  // Increment - OK
    c--;  // Decrement - OK
    ++c;  // Pre-increment - OK
    --c;  // Pre-decrement - OK

    // Multiplication and division (not + or -)
    char result3 = c * 2;  // Multiplication with int - OK
    char result4 = c / 2;  // Division with int - OK
    char result5 = c % 2;  // Modulo with int - OK

    // Shift operations (not + or -)
    char result6 = c << 1;  // Left shift - OK
    char result7 = c >> 1;  // Right shift - OK
}

void test_casts_and_conversions(void) {
    char c1 = 'A';
    char c2 = 'B';

    // Explicit cast to int before arithmetic
    int result1 = (int)c1 + (int)c2;  // OK - operating on ints

    // Implicit promotion in larger expression
    int result2 = c1 + 100;  // c1 promoted to int - OK

    // Cast result back to char
    char result3 = (char)(c1 + 100);  // Arithmetic is with int - OK
}

// Define wchar_t for testing (typically provided by stddef.h or wchar.h)
typedef int wchar_t;

void test_wide_characters(void) {
    wchar_t wc1 = L'A';
    wchar_t wc2 = L'B';

    // Wide character arithmetic - may trigger if implementation treats as char type
    // Behavior depends on whether wchar_t is considered a character type
    wchar_t result = wc1 + wc2;
}

void test_char16_char32(void) {
    // C11 char16_t and char32_t
    // These are typically not considered character types in the same way
    unsigned short c16_1 = u'A';
    unsigned short c16_2 = u'B';

    unsigned int c32_1 = U'A';
    unsigned int c32_2 = U'B';

    // These use integer types, not char types - should be OK
    unsigned short result1 = c16_1 + c16_2;
    unsigned int result2 = c32_1 + c32_2;
}
