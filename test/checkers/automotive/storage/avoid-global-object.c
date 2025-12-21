// RUN: %check_clang_tidy %s automotive-c23-adv-8.9 %t

// Test: File-scope objects should be at block scope if only used in one function (MISRA Rule 8.9)

// Violation - static variable only used in one function
// CHECK-MESSAGES: :[[@LINE+1]]:12: warning: file-scope object 'single_use_var' is only used in function 'use_single_var'; consider defining it at block scope [automotive-c23-adv-8.9]
static int single_use_var = 0;

void use_single_var(void) {
    single_use_var++;
}

// Compliant - static variable used in multiple functions
static int multi_use_var = 0;

void func1(void) {
    multi_use_var++;
}

void func2(void) {
    multi_use_var += 2;
}

// Compliant - const static variable (intentional file-scope constant)
static const int CONSTANT_VALUE = 42;

void use_constant(void) {
    int x = CONSTANT_VALUE;
    (void)x;
}

// Violation - static variable only used in use_vars function
// CHECK-MESSAGES: :[[@LINE+1]]:12: warning: file-scope object 'single_func_var' is only used in function 'use_vars'; consider defining it at block scope [automotive-c23-adv-8.9]
static int single_func_var = 0;

// Compliant - non-static file-scope (external linkage)
int external_var = 0;

void use_external(void) {
    external_var++;
}

// Violation - static variable used in only one function (multiple times)
// CHECK-MESSAGES: :[[@LINE+1]]:12: warning: file-scope object 'same_func_multi_use' is only used in function 'use_multiple_times'; consider defining it at block scope [automotive-c23-adv-8.9]
static int same_func_multi_use = 0;

void use_multiple_times(void) {
    same_func_multi_use++;
    same_func_multi_use *= 2;
    same_func_multi_use--;
}

// Compliant - local static variable (already at block scope)
void func_with_local_static(void) {
    static int local_static = 0;
    local_static++;
}

// Helper to avoid unused variable warnings
void use_vars(void) {
    (void)single_func_var;
    (void)external_var;
}
