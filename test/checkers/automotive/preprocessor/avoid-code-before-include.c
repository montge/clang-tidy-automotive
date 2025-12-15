// RUN: %check_clang_tidy %s automotive-avoid-code-before-include %t

// Test: Code before #include directives (MISRA Rule 20.1)

// Violation - code before include
int global_var = 0;
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: code appears before #include directive

#include <stdio.h>

// Compliant - includes at the beginning of file would not trigger
// (but in this test file we already have code above)

// Additional code after includes is fine
int another_var = 1;

void use_vars(void) {
    (void)global_var;
    (void)another_var;
}
