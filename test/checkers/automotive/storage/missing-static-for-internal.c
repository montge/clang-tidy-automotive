// RUN: %check_clang_tidy %s automotive-c23-adv-8.7 %t
// Test: Functions and objects with internal linkage should use static (MISRA Rule 8.7)
// Check ID: automotive-c23-adv-8.7

// ============= Function Violations =============

// Violation - function without static that has no external declaration
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function 'internal_function' has no external declaration
void internal_function(void) {
}

// Violation - another internal function
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function 'helper_function' has no external declaration
int helper_function(int x) {
    return x * 2;
}

// Violation - function returning pointer
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function 'get_pointer' has no external declaration
int *get_pointer(void) {
    static int value = 0;
    return &value;
}

// Violation - function with struct return
struct Data { int x; int y; };
// CHECK-MESSAGES: :[[@LINE+1]]:13: warning: function 'create_data' has no external declaration
struct Data create_data(int a, int b) {
    struct Data d = {a, b};
    return d;
}

// ============= Compliant Functions =============

// Compliant - static function (explicit internal linkage)
static void static_internal_function(void) {
}

// Compliant - static function with parameters
static int static_helper(int x, int y) {
    return x + y;
}

// Compliant - function with external declaration
extern void declared_extern_function(void);
void declared_extern_function(void) {
    // Has external declaration, so no warning
}

// Compliant - inline function (may need external linkage)
inline void inline_function(void) {
}

// Compliant - main function (special case)
int main(void) {
    internal_function();
    (void)helper_function(5);
    static_internal_function();
    declared_extern_function();
    (void)get_pointer();
    (void)create_data(1, 2);
    (void)static_helper(1, 2);
    return 0;
}

// ============= Variable Violations =============

// Violation - file-scope variable without static
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: file-scope object 'internal_var' has no external declaration
int internal_var = 0;

// Violation - another file-scope variable
// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: file-scope object 'another_var' has no external declaration
float another_var = 3.14f;

// Violation - file-scope array without static
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: file-scope object 'internal_array' has no external declaration
int internal_array[10];

// ============= Compliant Variables =============

// Compliant - static file-scope variable
static int static_var = 0;

// Compliant - static array
static char static_buffer[100];

// Compliant - extern variable (has external linkage)
extern int external_var;

// In C, const variables still have external linkage (unlike C++)
// So this should be flagged
// CHECK-MESSAGES: :[[@LINE+1]]:11: warning: file-scope object 'const_var' has no external declaration
const int const_var = 42;

// Helper to use variables - should also be static since no external declaration
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function 'use_variables' has no external declaration
void use_variables(void) {
    (void)internal_var;
    (void)another_var;
    (void)internal_array[0];
    (void)static_var;
    (void)static_buffer[0];
    (void)const_var;
}
