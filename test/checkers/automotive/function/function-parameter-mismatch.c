// RUN: %check_clang_tidy %s automotive-function-declaration-mismatch %t

// Test: Function declaration/definition parameter name mismatch (MISRA Rule 8.3)

#include <stdint.h>

// Violation - parameter names swapped between declaration and definition
extern int16_t func(int16_t num, int16_t den);
// CHECK-MESSAGES: :[[@LINE+2]]:22: warning: parameter name 'den' mismatch with 'num' in function 'func' [automotive-function-declaration-mismatch]
// CHECK-MESSAGES: :[[@LINE+1]]:35: warning: parameter name 'num' mismatch with 'den' in function 'func' [automotive-function-declaration-mismatch]
int16_t func(int16_t den, int16_t num) {
    return num / den;
}

// Compliant - parameter names match
extern int add(int a, int b);
int add(int a, int b) {
    return a + b;
}

// Compliant - function with void parameter
extern void process(void);
void process(void) {
}

// Compliant - function with pointer parameter
extern int* get_ptr(int* input);
int* get_ptr(int* input) {
    return input;
}
