// RUN: %check_clang_tidy %s automotive-cpp23-adv-4.1 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-4.1
// Related MISRA C++:2023 Rule: 4.1

// This test verifies that comparisons between different character types are detected.

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_char_comparison_mismatch() {
    char c = 'a';
    signed char sc = static_cast<signed char>('b');
    unsigned char uc = static_cast<unsigned char>('c');

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: comparison between different character types: 'char' and 'signed char'
    if (c == sc) {}

    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: comparison between different character types: 'char' and 'unsigned char'
    if (c != uc) {}

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: comparison between different character types: 'signed char' and 'unsigned char'
    if (sc < uc) {}

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: comparison between different character types: 'unsigned char' and 'char'
    if (uc > c) {}
}

void test_wide_char_mismatch() {
    wchar_t wc = L'a';
    char c = 'a';

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: comparison between different character types: 'wchar_t' and 'char'
    if (wc == c) {}
}

void test_char16_char32_mismatch() {
    char16_t c16 = u'a';
    char32_t c32 = U'a';

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: comparison between different character types: 'char16_t' and 'char32_t'
    if (c16 == c32) {}
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_same_type_comparison() {
    char c1 = 'a';
    char c2 = 'b';
    if (c1 == c2) {}  // Same type - OK
    if (c1 < c2) {}   // Same type - OK

    signed char sc1 = static_cast<signed char>('a');
    signed char sc2 = static_cast<signed char>('b');
    if (sc1 == sc2) {}  // Same type - OK

    wchar_t wc1 = L'a';
    wchar_t wc2 = L'b';
    if (wc1 == wc2) {}  // Same type - OK

    char16_t c16a = u'a';
    char16_t c16b = u'b';
    if (c16a == c16b) {}  // Same type - OK

    char32_t c32a = U'a';
    char32_t c32b = U'b';
    if (c32a == c32b) {}  // Same type - OK
}

void test_literal_comparison() {
    char c = 'a';
    if (c == 'b') {}  // char compared to char literal - OK

    wchar_t wc = L'x';
    if (wc == L'y') {}  // wchar_t compared to wchar_t literal - OK
}

void test_non_char_comparison() {
    int i = 42;
    int j = 43;
    if (i == j) {}  // Not character types - OK

    float f = 1.0f;
    float g = 2.0f;
    if (f < g) {}  // Not character types - OK
}
