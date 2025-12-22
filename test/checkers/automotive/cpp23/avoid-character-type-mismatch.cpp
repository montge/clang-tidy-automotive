// RUN: %check_clang_tidy %s automotive-cpp23-req-7.0 %t -- -- -std=c++17
// Test for automotive-cpp23-req-7.0
// Related MISRA C++:2023 Rule: 7.0

// This test verifies that assignments between different character types are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_char_mismatch() {
    char c = 'a';
    // Note: Character literals like 'b' are type char in C++,
    // so initializing signed/unsigned char with them is also a mismatch
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: initialization with different character type: variable is 'signed char' but initializer is 'char'
    signed char sc = 'b';
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: initialization with different character type: variable is 'unsigned char' but initializer is 'char'
    unsigned char uc = 'c';

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: assignment between different character types: 'char' and 'signed char'
    c = sc;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: assignment between different character types: 'char' and 'unsigned char'
    c = uc;

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: assignment between different character types: 'signed char' and 'char'
    sc = c;

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: assignment between different character types: 'signed char' and 'unsigned char'
    sc = uc;

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: assignment between different character types: 'unsigned char' and 'char'
    uc = c;

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: assignment between different character types: 'unsigned char' and 'signed char'
    uc = sc;
}

void test_init_mismatch() {
    char c = 'a';
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: initialization with different character type: variable is 'signed char' but initializer is 'char'
    signed char sc = 'b';

    // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: initialization with different character type: variable is 'char' but initializer is 'signed char'
    char c2 = sc;

    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: initialization with different character type: variable is 'unsigned char' but initializer is 'char'
    unsigned char uc2 = c;

    (void)c2;
    (void)uc2;
    (void)sc;
}

void test_wide_char_mismatch() {
    wchar_t wc = L'a';
    char c = 'a';

    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: assignment between different character types: 'wchar_t' and 'char'
    wc = c;

    // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: assignment between different character types: 'char' and 'wchar_t'
    c = wc;
}

void test_char16_char32_mismatch() {
    char16_t c16 = u'a';
    char32_t c32 = U'a';
    char c = 'a';

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: assignment between different character types: 'char16_t' and 'char'
    c16 = c;

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: assignment between different character types: 'char32_t' and 'char'
    c32 = c;

    // CHECK-MESSAGES: :[[@LINE+1]]:9: warning: assignment between different character types: 'char16_t' and 'char32_t'
    c16 = c32;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_same_type() {
    // Use explicit casts for signed/unsigned char initialization to avoid warnings
    char c1 = 'a';
    char c2 = 'b';
    c1 = c2;  // Same type - OK

    signed char sc1 = static_cast<signed char>('a');
    signed char sc2 = static_cast<signed char>('b');
    sc1 = sc2;  // Same type - OK

    unsigned char uc1 = static_cast<unsigned char>('a');
    unsigned char uc2 = static_cast<unsigned char>('b');
    uc1 = uc2;  // Same type - OK

    wchar_t wc1 = L'a';
    wchar_t wc2 = L'b';
    wc1 = wc2;  // Same type - OK

    char16_t c16a = u'a';
    char16_t c16b = u'b';
    c16a = c16b;  // Same type - OK

    char32_t c32a = U'a';
    char32_t c32b = U'b';
    c32a = c32b;  // Same type - OK
}

void test_non_char_types() {
    int i = 42;
    short s = 10;
    i = s;  // Not character types - OK

    float f = 1.0f;
    double d = 2.0;
    f = static_cast<float>(d);  // Not character types - OK
}

void test_literal_with_correct_type() {
    // char literal is type char, so char is OK
    char c = 'a';  // Literal - OK (both char)
    wchar_t wc = L'x';  // Wide literal - OK (both wchar_t)
    char16_t c16 = u'y';  // char16_t literal - OK
    char32_t c32 = U'z';  // char32_t literal - OK

    // Use cast for signed/unsigned char
    signed char sc = static_cast<signed char>('b');  // With cast - OK
    unsigned char uc = static_cast<unsigned char>('c');  // With cast - OK

    (void)c;
    (void)sc;
    (void)uc;
    (void)wc;
    (void)c16;
    (void)c32;
}
