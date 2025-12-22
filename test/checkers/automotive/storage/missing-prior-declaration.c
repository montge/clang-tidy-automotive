// RUN: %check_clang_tidy %s automotive-c23-req-8.4 %t

// MISRA C:2012 Rule 8.4: A compatible declaration shall be visible when
// an object or function with external linkage is defined.

// Non-compliant: No prior declaration
int global_no_decl = 42;
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: external variable 'global_no_decl' defined without a prior declaration

void func_no_decl(void) {
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: external function 'func_no_decl' defined without a prior declaration
}

// Compliant: Has prior declaration
extern int global_with_decl;
int global_with_decl = 100;

extern void func_with_decl(void);
void func_with_decl(void) {
}

// Compliant: Static (internal linkage) - not subject to this rule
static int static_var = 1;
static void static_func(void) {
}

// Compliant: main() is exempt
int main(void) {
    (void)global_no_decl;
    (void)global_with_decl;
    (void)static_var;
    func_no_decl();
    func_with_decl();
    static_func();
    return 0;
}
