// RUN: %check_clang_tidy %s automotive-c23-req-21.24 %t
// Test for automotive-c23-req-21.24
// Related MISRA C:2025 Rule: 21.24

// This test verifies that random number generator functions from <stdlib.h>
// are detected and flagged.

// Forward declarations to avoid -nostdinc++ issues
void srand(unsigned int seed);
int rand(void);

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_random_violations(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: Avoid 'srand' call from stdlib [automotive-c23-req-21.24]
    srand(42);

    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: Avoid 'srand' call from stdlib [automotive-c23-req-21.24]
    srand(12345);

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid 'rand' call from stdlib [automotive-c23-req-21.24]
    int x = rand();

    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: Avoid 'rand' call from stdlib [automotive-c23-req-21.24]
    int y = rand() % 100;

    // Using rand() result in expression
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: Avoid 'rand' call from stdlib [automotive-c23-req-21.24]
    int z = 10 + (rand() % 50);

    (void)x;
    (void)y;
    (void)z;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Custom random implementation is allowed
static unsigned int seed = 12345;

unsigned int custom_rand(void) {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}

void custom_srand(unsigned int new_seed) {
    seed = new_seed;
}

void test_compliant(void) {
    // Custom implementations with similar names are allowed
    custom_srand(42);
    int x = custom_rand();
    (void)x;
}
