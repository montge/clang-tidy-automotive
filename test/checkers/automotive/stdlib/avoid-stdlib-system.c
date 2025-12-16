// RUN: %check_clang_tidy %s automotive-c23-req-21.8 %t

// Test: system() and related functions shall not be used (MISRA Rule 21.8)

#include <stdlib.h>

// Violation - using system()
void test_system(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: use of system
    system("ls");
}

// Compliant - other stdlib functions
void test_compliant(void) {
    int x = abs(-5);
    (void)x;
}

// Use function to avoid unused warnings
void use_functions(void) {
    test_compliant();
}
