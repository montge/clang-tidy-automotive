// Test file for: automotive-avoid-lowercase-literal-suffix
// Related MISRA C:2025 Rule: 7.3
//
// This file tests the detection of lowercase literal suffixes

// RUN: %check_clang_tidy %s automotive-avoid-lowercase-literal-suffix %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_lowercase_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: lowercase literal suffix
    long a = 100l;  // lowercase 'l' looks like '1'

    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: lowercase literal suffix
    unsigned int b = 100u;  // lowercase 'u'

    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: lowercase literal suffix
    unsigned long c = 100ul;  // lowercase 'u' and 'l'
    // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: lowercase literal suffix

    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: lowercase literal suffix
    float d = 1.0f;  // lowercase 'f'

    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: lowercase literal suffix
    long long e = 100ll;  // lowercase 'll'
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant(void) {
    // Uppercase suffixes
    long a = 100L;
    unsigned int b = 100U;
    unsigned long c = 100UL;
    float d = 1.0F;
    long long e = 100LL;
    unsigned long long f = 100ULL;

    // No suffix
    int g = 100;
    double h = 1.0;
}
