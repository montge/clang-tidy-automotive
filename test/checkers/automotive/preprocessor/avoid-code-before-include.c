// RUN: %check_clang_tidy %s automotive-avoid-code-before-include %t

// Test: Code before #include directives (MISRA Rule 20.1)
// Note: This check may not be fully implemented or may require specific conditions

// Potential violation - code before include
int global_var = 0;

#include <stdio.h>

// Compliant - includes at the beginning of file would not trigger
// (but in this test file we already have code above)

// Additional code after includes is fine
int another_var = 1;

void use_vars(void) {
    (void)global_var;
    (void)another_var;
}
