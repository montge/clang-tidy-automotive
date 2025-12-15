// RUN: %check_clang_tidy %s automotive-c23-adv-8.7 %t

// Test: Functions and objects with internal linkage should use static (MISRA Rule 8.7)

// Violation - function without static that has no external declaration
void internal_function(void) {
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'internal_function' has no external declaration; consider declaring it static for internal linkage
}

// Violation - another internal function
int helper_function(int x) {
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: function 'helper_function' has no external declaration; consider declaring it static for internal linkage
    return x * 2;
}

// Compliant - static function (explicit internal linkage)
static void static_internal_function(void) {
}

// Compliant - function with external declaration (assumed in header)
extern void declared_extern_function(void);

void declared_extern_function(void) {
    // Has external declaration, so no warning
}

// Compliant - main function (special case)
int main(void) {
    internal_function();
    (void)helper_function(5);
    static_internal_function();
    declared_extern_function();
    return 0;
}

// File-scope variable tests

// Violation - file-scope variable without static
int internal_var = 0;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: file-scope object 'internal_var' has no external declaration; consider declaring it static for internal linkage

// Compliant - static file-scope variable
static int static_var = 0;

// Compliant - extern variable (has external linkage)
extern int external_var;

// Compliant - const variable (may have internal linkage in C++)
const int const_var = 42;

// Helper to use variables
void use_variables(void) {
    (void)internal_var;
    (void)static_var;
    (void)const_var;
}
