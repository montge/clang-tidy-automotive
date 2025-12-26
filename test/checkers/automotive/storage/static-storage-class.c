// RUN: %check_clang_tidy %s automotive-c25-req-8.8 %t

// Test: Static storage class specifier shall be used in all declarations
// of objects and functions that have internal linkage (MISRA C:2025 Rule 8.8)

// ============= Function Violations =============

// Violation - first declaration has static, but definition does not
static int helper_function(int x);
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: function 'helper_function' has internal linkage but is missing the static storage class specifier [automotive-c25-req-8.8]
int helper_function(int x) {
    return x * 2;
}

// Violation - first declaration has static, second redeclaration without static
static void process_data(int value);
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: function 'process_data' has internal linkage but is missing the static storage class specifier [automotive-c25-req-8.8]
void process_data(int value);
static void process_data(int value) {
    (void)value;
}

// ============= Compliant Code =============

// Compliant - all declarations have static
static int compliant_function(int x);
static int compliant_function(int x) {
    return x + 1;
}

// Compliant - static only (no redeclaration)
static int single_static_function(void) {
    return 1;
}

// Compliant - static variable with no prior declaration
static int single_static_var = 40;

// Compliant - external linkage (no static needed)
extern int external_function(void);
int external_function(void) {
    return 2;
}

// Compliant - function with only external declarations (no static)
void public_function(void);
void public_function(void) {
    // Has external linkage
}

// Compliant - main function (special case)
int main(void) {
    (void)helper_function(5);
    process_data(10);
    (void)compliant_function(3);
    (void)single_static_function();
    (void)single_static_var;
    (void)external_function();
    public_function();
    return 0;
}
