// Test file for: automotive-avoid-lowercase-literal-suffix
// Related MISRA C:2025 Rule: 7.3
//
// This file tests the detection of lowercase literal suffixes

// RUN: %check_clang_tidy %s automotive-avoid-lowercase-literal-suffix %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_lowercase_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: avoid lowercase 'l' in a literal suffix
    long a = 100l;  // lowercase 'l' looks like '1'

    unsigned int b = 100u;  // lowercase 'u' - not flagged

    // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: avoid lowercase 'l' in a literal suffix
    unsigned long c = 100ul;  // lowercase 'u' and 'l'

    float d = 1.0f;  // lowercase 'f' - not flagged

    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: avoid lowercase 'l' in a literal suffix
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
